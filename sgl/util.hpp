#pragma once

#include "./definitions.hpp"

namespace sgl {
    namespace util {
        auto file_as_string(const char * path) -> std::string {
            std::ifstream stream(path);
            if (!stream) {
                sgl::raise(("The file " + std::string(path) + " does not exist").c_str());
            }
            return std::string(
                std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>());
            };
        auto file_as_bytes(const char * path) -> std::vector<unsigned char> {
            std::vector<unsigned char> bytes;
            std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
            if (!stream) {
                sgl::raise(("The file " + std::string(path) + " does not exist").c_str());
            }
            unsigned char c = stream.get();
            while(stream.good()) { bytes.push_back(c); c = stream.get(); }
            return bytes;
        }
        auto push_vec(std::vector<float> & c, const glm::vec3 v) -> void {
            c.push_back(v.x);
            c.push_back(v.y);
            c.push_back(v.z);
        }
        auto push_vec(std::vector<float> & c, const glm::vec2 v) -> void {
            c.push_back(v.x);
            c.push_back(v.y);
        }
        auto push_triangle(std::vector<float> & c, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3) -> void {
            push_vec(c, v1);
            push_vec(c, v2);
            push_vec(c, v3);
        }
        auto push_triangle(std::vector<float> & c, const glm::vec2 & v1, const glm::vec2 & v2, const glm::vec2 & v3) -> void {
            push_vec(c, v1);
            push_vec(c, v2);
            push_vec(c, v3);
        }
        auto normal(const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 v3) -> glm::vec3 {
            const glm::vec3 u = v1 - v3;
            const glm::vec3 v = v1 - v2;
            const glm::vec3 n = glm::normalize(glm::cross(u, v));
            return n;
        }
        auto identity(glm::vec3 v) -> glm::vec3 {
            return v;
        }
        template<typename T>
        auto clamp(const T value, const T low, const T high) -> T {
            if(value < low)  { return low; }
            if(value > high) { return high; }
            return value;
        }
        auto fract(float f) -> float {
            return f - floor(f);
        }
    }
}