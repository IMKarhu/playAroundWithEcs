#include "modelImporter.h"
#include "tiny_gltf_v3.h"
#define TINYGLTF3_IMPLEMENTATION
#include <print>

Mesh ModelImporter::importDataFromFIle(std::string file)
{
    Mesh mesh;
    tg3_parse_options opts;
    tg3_error_stack errors;
    tg3_model model;

    tg3_parse_options_init(&opts);
    tg3_error_stack_init(&errors);

    tg3_error_code err = tg3_parse_file(&model, &errors, file.c_str(), 110, &opts);
    if (err != TG3_OK) {
        for (uint32_t i = 0; i < errors.count; i++) {
            std::println("{} {}", (int)errors.entries[i].severity, errors.entries[i].message ? errors.entries[i].message : "null");
        }
    }


    tg3_model_free(&model);
    tg3_error_stack_free(&errors);
    return mesh;
}

