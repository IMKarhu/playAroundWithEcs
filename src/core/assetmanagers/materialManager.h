#pragma once
#include "assetbase.hpp"
#include "textureManager.h"
#include <string_view>
#include <unordered_map>

#ifdef _WIN32
    #ifdef CORE_EXPORT
    #define CORE_API __declspec(dllexport)
    #else
    #define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API
#endif


namespace Lumos
{
    class CORE_API MaterialManager : public ISubAssetManager
    {
    public:
        MaterialManager(IGPUResourceFactory& resourcefactory);
        MaterialHandle create(std::string_view name, MaterialResource& resource, TextureManager& texturemanager);
        void createssbobuffer();
        MaterialResource* get(MaterialHandle handle);
        const MaterialResource* get(MaterialHandle handle) const;

        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, MaterialResource> m_materials;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        //test code for bindless stuff and ssbo
        std::vector<GpuMaterial> m_gpumaterials;
        uint32_t m_materialssbo = 0;
        IGPUResourceFactory& m_resourcefactory;
    };
}//namespace Lumos
