#pragma once
#include <cstdint>
#include <string_view>


namespace Lumos
{
    class StringHash
    {
    public:
        static constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
        static constexpr uint64_t FNV_PRIME = 1099511628211ULL;

        // FNV-1a 64-bit hash
        static constexpr uint64_t hash(std::string_view str) {
            uint64_t h = FNV_OFFSET_BASIS;
            for (char c : str) {
                h ^= static_cast<uint64_t>(c);
                h *= FNV_PRIME;
            }
            return h;
        }
    };

    constexpr uint64_t operator""_id(const char* str, size_t size) {
        return StringHash::hash(std::string_view(str, size));
    }
} //namespace Lumos

