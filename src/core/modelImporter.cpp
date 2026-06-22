#include "modelImporter.h"
#include "tiny_gltf_v3.h"
#define TINYGLTF3_IMPLEMENTATION
#include <print>
#include <iostream>

// Helper to deduce extensions
bool hasExtension(const std::string& filepath, const std::string& ext) {
    if (filepath.size() >= ext.size()) {
        return (filepath.compare(filepath.size() - ext.size(), ext.size(), ext) == 0);
    }
    return false;
}

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

Mesh ModelImporter::importDataFromFIle(std::string file)
{
    Mesh outMesh;
    tg3_parse_options opts;
    tg3_error_stack errors;
    tg3_model model;

    tg3_parse_options_init(&opts);
    tg3_error_stack_init(&errors);

    bool success = false;
    tg3_error_code err;

    if (hasExtension(file, ".glb")) {
        // tg3_error_code err = tg3_parse_glb(&model, &errors, file.data(), 0, 0, 0, &opts);
    } else {
        err = tg3_parse_file(&model, &errors, file.c_str(), 110, &opts);
    }

    if (err != TG3_OK) {
        for (uint32_t i = 0; i < errors.count; i++) {
            std::println("{} {}", (int)errors.entries[i].severity, errors.entries[i].message ? errors.entries[i].message : "null");
            success = false;
        }
    }

    std::println("mesh count: {}", model.meshes_count);

    // iterate over all the meshes
    outMesh.submeshes.clear();
    for (size_t i = 0; i < model.meshes_count; i++) {
        const auto& mesh = model.meshes[i];
        std::println("primitive count: {}", mesh.primitives_count);
        for (size_t j = 0; j < mesh.primitives_count; j++) {
            const auto& primitive = mesh.primitives[j];
            std::println("primitive material: {}", primitive.material);
            SubMesh submesh;
            submesh.name = std::string(mesh.name.data, mesh.name.len);
            uint64_t totalvertcount = 0;
            for (size_t v = 0; primitive.attributes_count; v++) {
                std::string_view view(primitive.attributes[v].key.data, primitive.attributes[v].key.len);
                if (view == "POSITION") {
                    totalvertcount = model.accessors[primitive.attributes[v].value].count;
                    break;
                }
            }
            if (totalvertcount == 0) {
                continue;
            }
            submesh.vertices.resize(totalvertcount);
            for (size_t k = 0; k < primitive.attributes_count; k++) {
                const auto& attribute = primitive.attributes[k];
                std::println("attribute value: {}",attribute.value);
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
                    std::println("I am position");
                    for (uint64_t v = 0; v < accessor.count; v++) {
                        const auto* vertex = reinterpret_cast<const float*>(dataPtr + (v * stride));
                        submesh.vertices[v].position[0] = vertex[0];
                        submesh.vertices[v].position[1] = vertex[1];
                        submesh.vertices[v].position[2] = vertex[2];
                    }

                }
                if (view == "TEXCOORD_0") {
                    std::println("I am texCoords");
                    for (uint64_t v = 0; v < accessor.count; v++) {
                        const auto* uv = reinterpret_cast<const float*>(dataPtr + (v * stride));
                        submesh.vertices[v].texcoord[0] = uv[0];
                        submesh.vertices[v].texcoord[1] = uv[1];
                    }

                }
                if (view == "NORMAL") {
                    std::println("I am normals");
                    for (uint64_t v = 0; v < accessor.count; v++) {
                        const auto* normals = reinterpret_cast<const float*>(dataPtr + (v * stride));
                        submesh.vertices[v].normal[0] = normals[0];
                        submesh.vertices[v].normal[1] = normals[1];
                        submesh.vertices[v].normal[2] = normals[2];
                    }
                }
                if (primitive.material >= 0 && primitive.material < model.materials_count) {
                    const auto& material = model.materials[primitive.material];
                    const auto& pbr = material.pbr_metallic_roughness;
                    for (uint64_t v = 0; v < accessor.count; v++) {
                        submesh.vertices[v].color[0] = pbr.base_color_factor[0];
                        submesh.vertices[v].color[1] = pbr.base_color_factor[1];
                        submesh.vertices[v].color[2] = pbr.base_color_factor[2];
                        submesh.vertices[v].color[3] = pbr.base_color_factor[3];
                    }
                }
            }
            // if indices exist
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
            outMesh.submeshes.push_back(submesh);
        }
    }

    // --- STEP 2: EXTRACT IMAGES / TEXTURES ---
    // TinyGLTF automatically loads image binary data via embedded stb_image macros!
    // for (const auto& gltfImage : model.images) {
    //     ImportedTextureData texData;
    //     texData.width = gltfImage.width;
    //     texData.height = gltfImage.height;
    //     texData.componentCount = gltfImage.component;
    //
    //     // Copy the raw pixel bytes safely into our out-struct
    //     texData.pixels = gltfImage.image; 
    //
    //     outData.textures.push_back(texData);
    // }

    tg3_model_free(&model);
    tg3_error_stack_free(&errors);

    return outMesh;
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
