#include "modelImporter.h"
// #include "tiny_gltf_v3.h"
// #define TINYGLTF3_IMPLEMENTATION
#include <print>
#include <iostream>

/*
 * Partially implemented by Gemini with heavy touch on my part
 * This needs to be reimplemented.. I am not happy with this
*/

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

ModelImporter::ModelImportData ModelImporter::importDataFromFile(std::string file)
{
    ModelImportData data;
    tg3_parse_options opts;
    tg3_error_stack errors;
    tg3_model model;

    tg3_parse_options_init(&opts);
    tg3_error_stack_init(&errors);

    tg3_error_code err;
    err = tg3_parse_file(&model, &errors, file.c_str(), file.length(), &opts);

    if (err != TG3_OK) {
        for (uint32_t i = 0; i < errors.count; i++) {
            std::println("{} {}", (int)errors.entries[i].severity, errors.entries[i].message ? errors.entries[i].message : "null");
        }
    }


    for (size_t i = 0; i < model.nodes_count; i++) {
        const auto& node = model.nodes[i];
        data.transform.scale = glm::vec3(node.scale[0], node.scale[2], node.scale[2]);
    }
    parseModel(model, data);

    tg3_model_free(&model);
    tg3_error_stack_free(&errors);

    return data;
}

ScreenQuad ModelImporter::screenQuad()
{
    ScreenQuad outMesh;
    outMesh.name = "screen";
    outMesh.vertices.resize(6);
    outMesh.vertices[0].position = { -1.0f, 1.0f, 1.0f };
    outMesh.vertices[1].position = {  1.0f, 1.0f, 1.0f };
    outMesh.vertices[2].position = {  1.0f, -1.0f, 1.0f };
    outMesh.vertices[3].position = { -1.0f, -1.0f, 1.0f };
    outMesh.indices = { 0, 1, 2, 0, 2, 3 };
    outMesh.vertices[0].texcoord = {  0.0f, 1.0f };
    outMesh.vertices[1].texcoord = {  1.0f, 1.0f };
    outMesh.vertices[2].texcoord = {  1.0f, 0.0f };
    outMesh.vertices[3].texcoord = {  0.0f, 0.0f };
    return outMesh;
}

void ModelImporter::parseModel(const tg3_model& model, ModelImportData& data)
{
    std::println("model mesh count: {}", model.meshes_count);
    for (size_t i = 0; i < model.meshes_count; i++) {
        parseMeshe(model, model.meshes[i], data);
    }

    for (size_t i = 0; i < model.textures_count; i++) {
        parseTextures(model.images[i], data);
    }

    data.materialhandles.resize(model.materials_count);
    for (size_t i = 0; i < model.materials_count; i++) {
        parseMaterials(model, model.materials[i], data.materialhandles[i]);
    }

}

void ModelImporter::parseMeshe(const tg3_model& model, const tg3_mesh& mesh, ModelImportData& data)
{
    uint32_t submeshnum = 0;
    std::println("mesh name: {}, and primitive count: {}", std::string(mesh.name.data, mesh.name.len),
                                                          mesh.primitives_count);
    for (size_t i = 0; i < mesh.primitives_count; i++) {
        parsePrimitive(model, mesh.primitives[i], data, submeshnum);
        submeshnum++;
    }
}

void ModelImporter::parseTextures(const tg3_image& image, ModelImportData& data)
{
    Texture tex;
    if (image.uri.data && image.uri.len > 0) {
        tex.filename = std::string(image.uri.data, image.uri.len);
    }
    data.textures.push_back(tex);
}

void ModelImporter::parseMaterials(const tg3_model& model, const tg3_material& material, MaterialHandles& materialhandle)
{
        auto& outmat = materialhandle;
        int32_t basecoloridx = material.pbr_metallic_roughness.base_color_texture.index;
        if (basecoloridx >= 0 && basecoloridx < static_cast<int32_t>(model.textures_count)) {
            outmat.basecolorhandle = model.textures[basecoloridx].source;
        }

        int32_t metallicroughness = material.pbr_metallic_roughness.metallic_roughness_texture.index;
        if (metallicroughness >= 0 && metallicroughness < static_cast<int32_t>(model.textures_count)) {
            outmat.metallicroughnesshandle = model.textures[metallicroughness].source;
        }

        int32_t normalidx = material.normal_texture.index;
        if (normalidx >= 0 && normalidx < static_cast<int32_t>(model.textures_count)) {
            outmat.normalhandle = model.textures[normalidx].source;
        }
}

void ModelImporter::parsePrimitive(const tg3_model& model, const tg3_primitive& primitive, ModelImportData& data, uint32_t submeshnum)
{
        SubMesh submesh;
        submesh.name = "test" + std::to_string(submeshnum);
        uint64_t totalvertcount = 0;
        for (size_t v = 0; v < primitive.attributes_count; v++) {
            std::string_view view(primitive.attributes[v].key.data, primitive.attributes[v].key.len);
            if (view == "POSITION") {
                totalvertcount = model.accessors[primitive.attributes[v].value].count;
                // std::println("totalvertcount: {}", totalvertcount);
                break;
            }
        }
        if (totalvertcount == 0) {
            std::println("totalvertcount was 0");
            return;
        }
        submesh.vertices.resize(totalvertcount);
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
                    submesh.vertices[v].position[0] = vertex[0];
                    submesh.vertices[v].position[1] = vertex[1];
                    submesh.vertices[v].position[2] = vertex[2];
                }
    
            }
            if (view == "TEXCOORD_0") {
                for (uint64_t v = 0; v < accessor.count; v++) {
                    const auto* uv = reinterpret_cast<const float*>(dataPtr + (v * stride));
                    submesh.vertices[v].texcoord[0] = uv[0];
                    submesh.vertices[v].texcoord[1] = uv[1];
                }
    
            }
            if (view == "NORMAL") {
                for (uint64_t v = 0; v < accessor.count; v++) {
                    const auto* normals = reinterpret_cast<const float*>(dataPtr + (v * stride));
                    submesh.vertices[v].normal[0] = normals[0];
                    submesh.vertices[v].normal[1] = normals[1];
                    submesh.vertices[v].normal[2] = normals[2];
                }
            }
    
        }
        if (primitive.material >= 0 && primitive.material < model.materials_count) {
            const auto& material = model.materials[primitive.material];
            const auto& pbr = material.pbr_metallic_roughness;
            for (uint64_t v = 0; v < totalvertcount; v++) {
                submesh.vertices[v].color[0] = pbr.base_color_factor[0];
                submesh.vertices[v].color[1] = pbr.base_color_factor[1];
                submesh.vertices[v].color[2] = pbr.base_color_factor[2];
                submesh.vertices[v].color[3] = pbr.base_color_factor[3];
            }
        }
        // if indices exist
        readIndices(model, primitive, submesh);
        data.submeshmaterialindex.push_back(primitive.material);
        data.outMesh.submeshes.push_back(submesh);
}

void ModelImporter::readIndices(const tg3_model& model, const tg3_primitive& primitive, SubMesh& submesh)
{
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
            submesh.indices.push_back(index);
        }
    }
}

void ModelImporter::readAttributes(std::string_view view, const tg3_accessor& accessor, SubMesh& submesh)
{
    std::println("TODO: implement this function at some point so we can replace all the if statement view lookups in parsePrimitives");
}

