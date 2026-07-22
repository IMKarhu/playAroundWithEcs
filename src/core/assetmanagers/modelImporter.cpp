#include "modelImporter.h"
#include <print>
#include <string_view>
#define TINYGLTF3_IMPLEMENTATION

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

    ModelImporter::ModelImporter() {}

    ModelImporter::~ModelImporter()
    {
        destroyctx();
    }

    ModelImportData ModelImporter::import(const std::string& path)
    {
        m_model = parsemodel(path);

        ModelImportData data = parseSubMeshes(m_model);
        parseIndices(data, m_model);
        data.texsources = parseImages();
        parseMaterials(data);
        return data;
    }

    void ModelImporter::destroyctx()
    {
        tg3_model_free(&m_model);
        tg3_error_stack_free(&m_errors);
    }


    std::vector<TextureSource> ModelImporter::parseImages()
    {
        std::vector<TextureSource> sources;
        sources.resize(m_model.images_count);

        for (size_t i = 0; i < m_model.images_count; i++) {
            const auto& image = m_model.images[i];
            if (image.uri.data && image.uri.len > 0) {
                std::string texturepath = std::string(image.uri.data, image.uri.len);
                sources[i].cachekey = texturepath;
                sources[i].path = texturepath;
            }
        }
        return sources;
    }

    void ModelImporter::parseMaterials(ModelImportData& modeldata)
    {
        for (size_t i = 0; i < m_model.meshes_count; i++) {
            auto& mesh = m_model.meshes[i];
            for (size_t j = 0; j < mesh.primitives_count; j++) {
                auto& primitive = mesh.primitives[j];
                MaterialData data;
                if (primitive.material >= 0 && primitive.material < m_model.materials_count) {
                    const auto& material = m_model.materials[primitive.material];
                    int32_t basecoloridx = material.pbr_metallic_roughness.base_color_texture.index;
                    if (basecoloridx >= 0 && basecoloridx < static_cast<int32_t>(m_model.textures_count)) {
                        modeldata.submeshes[i].indexes.basecolor = m_model.textures[basecoloridx].source;
                    }
                    int32_t normalidx = material.normal_texture.index;
                    if (normalidx >= 0 && normalidx < static_cast<int32_t>(m_model.textures_count)) {
                        modeldata.submeshes[i].indexes.normal = m_model.textures[normalidx].source;
                    }
                    int32_t metrough = material.pbr_metallic_roughness.metallic_roughness_texture.index;
                    if (metrough >= 0 && metrough < static_cast<int32_t>(m_model.textures_count)) {
                        modeldata.submeshes[i].indexes.metrough = m_model.textures[metrough].source;
                    }
                }
            }
        }
    }

    //private functions
    tg3_model ModelImporter::parsemodel(const std::string& path)
    {
        tg3_parse_options opts;
        tg3_model model;

        tg3_parse_options_init(&opts);
        tg3_error_stack_init(&m_errors);

        tg3_error_code err;
        err = tg3_parse_file(&model, &m_errors, path.c_str(), path.length(), &opts);

        if (err != TG3_OK) {
            for (uint32_t i = 0; i < m_errors.count; i++) {
                std::println("{} {}", (int)m_errors.entries[i].severity,
                             m_errors.entries[i].message ? m_errors.entries[i].message : "null");
            }
            tg3_error_stack_free(&m_errors);
            return model;
        }

        return model;
    }

    ModelImportData ModelImporter::parseSubMeshes(const tg3_model& model)
    {
        ModelImportData data;
        for (size_t i = 0; i < model.meshes_count; i++) {
            const auto& mesh = model.meshes[i];
            for (size_t j = 0; j < mesh.primitives_count; j++) {
                const auto& primitive = mesh.primitives[j];
                ModelImportSubMeshData submeshdata;

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
                for (size_t a = 0; a < primitive.attributes_count; a++) {
                    const auto& attribute = primitive.attributes[a];
                    int32_t accessorIndex = attribute.value;
                    const auto& accessor = model.accessors[accessorIndex];
                    const auto& bufferview = model.buffer_views[accessor.buffer_view];
                    const auto& buffer = model.buffers[bufferview.buffer];
                    const unsigned char* dataPtr = buffer.data.data + bufferview.byte_offset + accessor.byte_offset;
                    size_t stride = bufferview.byte_stride;
                    if (stride == 0) {
                        stride = getComponentCount(accessor.type) * getComponentSizeInBytes(accessor.component_type);
                    }
                    std::string_view attributename(attribute.key.data, attribute.key.len);
                    switch(attributeType(attributename)) {
                        case VertexAttributeType::Position:
                            for (uint64_t k = 0; k < accessor.count; k++) {
                                const auto* offset = reinterpret_cast<const float*>(dataPtr + (k * stride));
                                submeshdata.vertices[k].position = readPositions(offset);
                            }
                            break;
                        case VertexAttributeType::Texcoord:
                            for (uint64_t k = 0; k < accessor.count; k++) {
                                const auto* offset = reinterpret_cast<const float*>(dataPtr + (k * stride));
                                submeshdata.vertices[k].texcoords = readTexcoords(offset);
                            }
                            break;
                        case VertexAttributeType::Normal:
                            for (uint64_t k = 0; k < accessor.count; k++) {
                                const auto* offset = reinterpret_cast<const float*>(dataPtr + (k * stride));
                                submeshdata.vertices[k].normal = readNormals(offset);
                            }
                            break;
                        case VertexAttributeType::Tangent:
                            for (uint64_t k = 0; k < accessor.count; k++) {
                                const auto* offset = reinterpret_cast<const float*>(dataPtr + (k * stride));
                                submeshdata.vertices[k].tangents = readTangents(offset);
                            }
                            break;
                        case VertexAttributeType::Unknown:
                            //TODO: handle unknown cases
                            break;
                        default:
                            //TODO: handle default case
                            break;
                    }
                }
                data.submeshes.push_back(std::move(submeshdata));
            }
        }
        return data;
    }

    void ModelImporter::parseIndices(ModelImportData& data, const tg3_model& model)
    {
        for (size_t i = 0; i < model.meshes_count; i++) {
            const auto& mesh = model.meshes[i];
            for (size_t j = 0; j < mesh.primitives_count; j++) {
                const auto& primitive = mesh.primitives[j];
                if (primitive.indices > -1) {
                    const auto& accessor = model.accessors[primitive.indices];
                    const auto& bufferview = model.buffer_views[accessor.buffer_view];
                    const auto& buffer = model.buffers[bufferview.buffer];
                    const unsigned char* dataPtr = buffer.data.data + bufferview.byte_offset + accessor.byte_offset;
                    size_t stride = bufferview.byte_stride;
                    if (stride == 0) {
                        stride = getComponentCount(accessor.type) * getComponentSizeInBytes(accessor.component_type);
                    }
                    for (size_t idx = 0; idx < accessor.count; ++idx) {
                        uint32_t index = 0;
                        if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_INT) {
                            index = *reinterpret_cast<const uint32_t*>(dataPtr + (idx * stride));
                        } else if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_SHORT) {
                            index = *reinterpret_cast<const uint16_t*>(dataPtr + (idx * stride));
                        } else if (accessor.component_type == TG3_COMPONENT_TYPE_UNSIGNED_BYTE) {
                            index = *reinterpret_cast<const uint8_t*>(dataPtr + (idx * stride));
                        }
                        data.submeshes[i].indices.push_back(index);
                    }
                }
            }
        }
    }

    ModelImporter::VertexAttributeType ModelImporter::attributeType(std::string_view name)
    {
        if (name == "POSITION") {
            return VertexAttributeType::Position;
        }
        if (name == "TEXCOORD_0") {
            return VertexAttributeType::Texcoord;
        }
        if (name == "NORMAL") {
            return VertexAttributeType::Normal;
        }
        if (name == "TANGENT") {
            return VertexAttributeType::Tangent;
        }
        return VertexAttributeType::Unknown;
    }

    glm::vec3 ModelImporter::readPositions(const float* offset)
    {
        return glm::vec3(offset[0], offset[1], offset[2]);
    }

    glm::vec2 ModelImporter::readTexcoords(const float* offset)
    {
        return glm::vec2(offset[0], offset[1]);
    }

    glm::vec3 ModelImporter::readNormals(const float* offset)
    {
        return glm::vec3(offset[0], offset[1], offset[2]);
    }

    glm::vec4 ModelImporter::readTangents(const float* offset)
    {
        return glm::vec4(offset[0], offset[1], offset[2], offset[3]);
    }

}//namespace Lumos
