#include "meshManager.h"
#include "string_hash.h"
#define TINYGLTF3_IMPLEMENTATION

#include <print>


namespace Lumos
{
    size_t getComponentCount(int32_t type) {
        switch (type) {
            case TG3_TYPE_SCALAR: return 1;
            case TG3_TYPE_VEC2:   return 2;
            case TG3_TYPE_VEC3:   return 3;
            case TG3_TYPE_VEC4:   return 4;
            case TG3_TYPE_MAT2:   return 4;
            case TG3_TYPE_MAT3:   return 9;
            case TG3_TYPE_MAT4:   return 16;
            // Vector and Matrix fallbacks if your file uses generic types
            case TG3_TYPE_VECTOR: return 4; 
            case TG3_TYPE_MATRIX: return 16;
            default:              return 0;
        }
    }

    size_t getComponentSizeInBytes(int32_t componentType) {
        switch (componentType) {
            case 5120: return 1; // TG3_COMPONENT_TYPE_BYTE / GL_BYTE
            case 5121: return 1; // TG3_COMPONENT_TYPE_UNSIGNED_BYTE / GL_UNSIGNED_BYTE
            case 5122: return 2; // TG3_COMPONENT_TYPE_SHORT / GL_SHORT
            case 5123: return 2; // TG3_COMPONENT_TYPE_UNSIGNED_SHORT / GL_UNSIGNED_SHORT
            case 5125: return 4; // TG3_COMPONENT_TYPE_UNSIGNED_INT / GL_UNSIGNED_INT
            case 5126: return 4; // TG3_COMPONENT_TYPE_FLOAT / GL_FLOAT
            default:   return 0;
        }
    }

    bool isGltfFile(std::string_view file) {
        return file.ends_with(".gltf");
    }

    bool isGlbFile(std::string_view file) {
        return file.ends_with(".glb");
    }

    MeshManager::MeshManager(IGPUResourceFactory& resourcefactory, TextureManager& texturemanager)
        :m_texturemanager(texturemanager)
        ,m_resourcefactory(resourcefactory)
    {
    }

    AssetHandle MeshManager::load(const std::string& filepath)
    {
        uint64_t id = StringHash::hash(filepath);

        if (m_meshes.find(id) != m_meshes.end()) {
            m_metadata[id].refcount++;
            return AssetHandle{ id };
        }

        tg3_error_stack errors;
        tg3_model model = parsemodel(filepath, errors);
        std::vector<AssetHandle> texturehandles = loadTextures(model);
         
        std::vector<SubMeshData> accumulatedsubmeshes = buildSubMeshes(model, texturehandles);

        tg3_model_free(&model);
        tg3_error_stack_free(&errors);
        m_meshes[id] = m_resourcefactory.createMesh(accumulatedsubmeshes);
        m_metadata[id] = AssetRecord{ 1, filepath };
        return AssetHandle { id };
    }

    IMesh* MeshManager::get(AssetHandle handle)
    {
        auto it = m_meshes.find(handle.id);
        if (it != m_meshes.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void MeshManager::unloadUnused()
    {
        for (auto it = m_metadata.begin(); it != m_metadata.end();) {
            if (it->second.refcount == 0) {
                m_meshes.erase(it->first);
                it = m_metadata.erase(it);
            }
            else {
                ++it;
            }
        }

    }

    tg3_model MeshManager::parsemodel(const std::string& path, tg3_error_stack& errors)
    {
        tg3_parse_options opts;
        tg3_model model;

        tg3_parse_options_init(&opts);
        tg3_error_stack_init(&errors);

        tg3_error_code err;
        err = tg3_parse_file(&model, &errors, path.c_str(), path.length(), &opts);

        if (err != TG3_OK) {
            for (uint32_t i = 0; i < errors.count; i++) {
                std::println("{} {}", (int)errors.entries[i].severity,
                             errors.entries[i].message ? errors.entries[i].message : "null");
            }
            tg3_error_stack_free(&errors);
            return model;
        }

        return model;
    }

    std::vector<AssetHandle> MeshManager::loadTextures(const tg3_model& model)
    {
        std::vector<AssetHandle> texturehandles;
        texturehandles.resize(model.images_count);

        for (size_t i = 0; i < model.images_count; i++) {
            if (model.images[i].uri.data && model.images[i].uri.len > 0) {
                std::string texturepath = std::string(model.images[i].uri.data,
                        model.images[i].uri.len);
                texturehandles[i] = m_texturemanager.load(texturepath);
            }
        }
        return texturehandles;
    }

    std::vector<SubMeshData> MeshManager::buildSubMeshes(const tg3_model& model, const std::vector<AssetHandle>& textures)
    {
        std::vector<SubMeshData> accumulatedsubmeshes;
        for (size_t i = 0; i < model.meshes_count; i++) {
            const auto& mesh = model.meshes[i];
            for (size_t i = 0; i < mesh.primitives_count; i++) {
                const auto& primitive = mesh.primitives[i];
                SubMeshData submeshdata;

                if (primitive.material >= 0 && primitive.material < model.materials_count) {
                    const auto& material = model.materials[primitive.material];
                    int32_t basecoloridx = material.pbr_metallic_roughness.base_color_texture.index;
                    if (basecoloridx >= 0 && basecoloridx < static_cast<int32_t>(model.textures_count)) {
                        int32_t imgsource = model.textures[basecoloridx].source;
                        submeshdata.basecolorHandle = textures[imgsource];
                    }
                    int32_t normalidx = material.normal_texture.index;
                    if (normalidx >= 0 && normalidx < static_cast<int32_t>(model.textures_count)) {
                        int32_t imgsource = model.textures[normalidx].source;
                        submeshdata.normalHandle = textures[imgsource];
                    }
                    int32_t metrough = material.pbr_metallic_roughness.metallic_roughness_texture.index;
                    if (metrough >= 0 && metrough < static_cast<int32_t>(model.textures_count)) {
                        int32_t imgsource = model.textures[metrough].source;
                        submeshdata.metallicroughnessHandle = textures[imgsource];
                    }
                }

                uint64_t totalvertcount = 0;
                for (size_t v = 0; v < primitive.attributes_count; v++) {
                    std::string_view view(primitive.attributes[v].key.data, primitive.attributes[v].key.len);
                    if (view == "POSITION") {
                        totalvertcount = model.accessors[primitive.attributes[v].value].count;
                        break;
                    }
                }
                if (totalvertcount == 0) {
                    std::println("totalvertcount was 0");
                    continue;
                }
                submeshdata.vertices.resize(totalvertcount);
                for (size_t i = 0; i < primitive.attributes_count; i++) {
                    const auto& attribute = primitive.attributes[i];
                    std::string_view view(attribute.key.data, attribute.key.len);
                    int32_t accessorIndex = attribute.value;
                    const auto& accessor = model.accessors[accessorIndex];
                    const auto& bufferview = model.buffer_views[accessor.buffer_view];
                    const auto& buffer = model.buffers[bufferview.buffer];
                    const unsigned char* dataPtr = buffer.data.data + bufferview.byte_offset + accessor.byte_offset;
                    size_t stride = bufferview.byte_stride;
                    if (stride == 0) {
                        stride = getComponentCount(accessor.type) * getComponentSizeInBytes(accessor.component_type);
                    }
                    if (view == "POSITION") {
                        for (uint64_t v = 0; v < accessor.count; v++) {
                            const auto* vertex = reinterpret_cast<const float*>(dataPtr + (v * stride));
                            submeshdata.vertices[v].position[0] = vertex[0];
                            submeshdata.vertices[v].position[1] = vertex[1];
                            submeshdata.vertices[v].position[2] = vertex[2];
                        }
    
                    }
                    if (view == "TEXCOORD_0") {
                        for (uint64_t v = 0; v < accessor.count; v++) {
                            const auto* uv = reinterpret_cast<const float*>(dataPtr + (v * stride));
                            submeshdata.vertices[v].texcoords[0] = uv[0];
                            submeshdata.vertices[v].texcoords[1] = uv[1];
                        }
    
                    }
                    if (view == "NORMAL") {
                        for (uint64_t v = 0; v < accessor.count; v++) {
                            const auto* normals = reinterpret_cast<const float*>(dataPtr + (v * stride));
                            submeshdata.vertices[v].normal[0] = normals[0];
                            submeshdata.vertices[v].normal[1] = normals[1];
                            submeshdata.vertices[v].normal[2] = normals[2];
                        }
                    }
                    if (view == "TANGENT") {
                        for (uint64_t v = 0; v < accessor.count; v++) {
                            const auto* tangents = reinterpret_cast<const float*>(dataPtr + (v * stride));
                            submeshdata.vertices[v].tangents[0] = tangents[0];
                            submeshdata.vertices[v].tangents[1] = tangents[1];
                            submeshdata.vertices[v].tangents[2] = tangents[2];
                            submeshdata.vertices[v].tangents[3] = tangents[3];
                        }
                    }
                }

                if (primitive.indices > -1) {
                    const auto& accessor = model.accessors[primitive.indices];
                    const auto& bufferview = model.buffer_views[accessor.buffer_view];
                    const auto& buffer = model.buffers[bufferview.buffer];
                    const unsigned char* dataPtr = buffer.data.data + bufferview.byte_offset + accessor.byte_offset;
                    size_t stride = bufferview.byte_stride;
                    if (stride == 0) {
                        stride = getComponentCount(accessor.type) * getComponentSizeInBytes(accessor.component_type);
                    }
                    for (size_t i = 0; i < accessor.count; ++i) {
                        uint32_t index = 0;
                        if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_INT) {
                            index = *reinterpret_cast<const uint32_t*>(dataPtr + (i * stride));
                        } else if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_SHORT) {
                            index = *reinterpret_cast<const uint16_t*>(dataPtr + (i * stride));
                        } else if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_BYTE) {
                            index = *reinterpret_cast<const uint8_t*>(dataPtr + (i * stride));
                        }
                        submeshdata.indices.push_back(index);
                    }
                }
                accumulatedsubmeshes.push_back(std::move(submeshdata));
            }
        }

        return accumulatedsubmeshes;
    }
};

        // std::println("-------------------------------------------");
        // std::println("accessor count: {}", model.accessors_count);
        // std::println("anim count count: {}", model.animations_count);
        // std::println("buffer count: {}", model.buffers_count);
        // std::println("bufferviews count: {}", model.buffer_views_count);
        // std::println("meshes count count: {}", model.meshes_count);
        // std::println("nodes count: {}", model.nodes_count);
        // std::println("textures count: {}", model.textures_count);
        // std::println("images count: {}", model.images_count);
        // std::println("skins count: {}", model.skins_count);
        // std::println("cameras count: {}", model.cameras_count);
        // std::println("sampler count: {}", model.samplers_count);
        // std::println("scenes count: {}", model.scenes_count);
        // std::println("lights count: {}", model.lights_count);
        // std::println("audio emitter count: {}", model.audio_emitters_count);
        // std::println("audio source count: {}", model.audio_sources_count);
        // std::println("material count: {}", model.materials_count);
        // std::println("-------------------------------------------");
        // const auto& image = model.images[i];
        // std::println("-----------------------------");
        // std::println("pixeltype: {}",model.images[i].pixel_type);
        // std::println("bufferview: {}",model.images[i].buffer_view);
        // std::println("channels: {}",model.images[i].component);
        // std::println("bits per channel: {}",model.images[i].bits);
        // std::println("name: {}",std::string(model.images[i].name.data, model.images[i].name.len));
        // std::println("as is: {}",model.images[i].as_is);
        // // std::println("image.count: {}",model.images[i].image.count);
        // // std::println("image.data: {}",*model.images[i].image.data);
        // std::println("image uri data: {}", std::string(model.images[i].uri.data, model.images[i].uri.len));
        // std::println("width: {}", model.images[i].width);
        // std::println("height: {}", model.images[i].height);
        // std::println("mime type: {}", model.images[i].mime_type.data ? model.images[i].mime_type.data : "<null>");
        // std::println("-----------------------------");

