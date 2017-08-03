#pragma once

#include "./definitions.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "./stb_image.h"

namespace sgl {
    namespace format {
        auto const red      = GL_RED;
        auto const rg       = GL_RG;
        auto const rgb      = GL_RGB;
        auto const bgr      = GL_BGR;
        auto const rgba     = GL_RGBA;
        auto const argb     = GL_BGRA;
        auto const int_red  = GL_RED_INTEGER;
        auto const int_rg   = GL_RG_INTEGER;
        auto const int_rgb  = GL_RGB_INTEGER;
        auto const int_bgr  = GL_BGR_INTEGER;
        auto const int_rgba = GL_RGBA_INTEGER;
        auto const int_bgra = GL_BGRA_INTEGER;
    }
    namespace option {
        auto const texture_wrap_S = GL_TEXTURE_WRAP_S;
        auto const texture_wrap_T = GL_TEXTURE_WRAP_T;
        auto const texture_wrap_R = GL_TEXTURE_WRAP_R;
        auto const mag_filter     = GL_TEXTURE_MAG_FILTER;
        auto const min_filter     = GL_TEXTURE_MIN_FILTER;
        auto const linear_mipmap  = GL_LINEAR_MIPMAP_LINEAR;
        auto const linear         = GL_LINEAR;
        auto const nearest        = GL_NEAREST;
        auto const less           = GL_LESS;
        auto const clamp_to_edge  = GL_CLAMP_TO_EDGE;
    }
    auto const texture_cube_map_positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    auto const texture_cube_map            = GL_TEXTURE_CUBE_MAP;
    auto const texture_2d                  = GL_TEXTURE_2D;
    auto const invert_y                    = 2;

    class texture {

        private:
        unsigned char * data = nullptr;
        sgl::uint id;
        int width;
        int height;
        int bpp;

        public:
        texture() {
            glGenTextures(1, &id);
            sgl::raise_errors("texture.hpp @ sgl::texture");
        }
        auto bind(GLenum type = sgl::texture_2d) -> texture & {
            glBindTexture(type, id);
            sgl::raise_errors("texture.hpp @ sgl::texture.bind");
            return * this;
        }
        auto load(
            const char * path,
            const int options  = 0,
            const int internal = sgl::format::rgb,
            const int external = sgl::format::rgb
        ) -> texture & {
            if(options & sgl::invert_y) {
                stbi_set_flip_vertically_on_load(true);
            }
            data = stbi_load(path, &width, &height, &bpp, 3);
            if(!data) {
                sgl::raise("texture failed to load");
            }
            stbi_set_flip_vertically_on_load(false);
            bind();
            glTexImage2D(sgl::texture_2d, 0, internal, width, height, 0, external, sgl::gl_unsigned_byte, data);
            stbi_image_free(data);
            sgl::raise_errors("texture.hpp @ sgl::texture.load");
            return * this;

        }
        auto load_cubemap(
            std::vector<std::string> faces,
            const int options  = 0,
            const int internal = sgl::format::rgb,
            const int external = sgl::format::rgb
        ) -> texture & {
            bind(sgl::texture_cube_map);
            if(options & sgl::invert_y) {
                stbi_set_flip_vertically_on_load(true);
            }
            for (unsigned int i = 0; i < 6; i++) {
                data = stbi_load(faces[i % faces.size()].c_str(), &width, &height, &bpp, 0);
                if(!data) {
                    sgl::raise("texture failed to load");
                }
                stbi_set_flip_vertically_on_load(false);

                glTexImage2D(sgl::texture_cube_map_positive_x + i, 0, internal, width, height, 0, external, sgl::gl_unsigned_byte, data);
                stbi_image_free(data);
            }
            return * this;
        }
        auto int_parameter(GLenum option, int value, GLenum type = sgl::texture_2d) -> texture & {
            bind(type);
            glTexParameteri(type, option, value);
            glGenerateMipmap(type);
            sgl::raise_errors("texture.hpp @ sgl::texture.parameter");
            return * this;
        }

    };
}