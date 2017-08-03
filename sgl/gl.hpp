#pragma once

#include "./definitions.hpp"

namespace sgl {

    typedef GLuint uint;

    auto const gl_float         = GL_FLOAT;
    auto const gl_unsigned_byte = GL_UNSIGNED_BYTE;
    auto const array_buffer     = GL_ARRAY_BUFFER;
    auto const static_draw      = GL_STATIC_DRAW;
    auto const triangles        = GL_TRIANGLES;

    auto const cull_face        = GL_CULL_FACE;
    auto const color_buffer_bit = GL_COLOR_BUFFER_BIT;
    auto const depth_buffer_bit = GL_DEPTH_BUFFER_BIT;
    auto const depth_test       = GL_DEPTH_TEST;
    auto const less             = GL_LESS;

    auto wireframes(bool b) -> void {
        if(b) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    auto enable(GLenum option) -> void {
        glEnable(option);
    }

    auto depth_mask(bool enable) -> void {
        glDepthMask(enable);
    }

    auto raise_errors(const char * message = nullptr) -> void {
        GLenum gl_error = glGetError();
        if(gl_error != GL_NO_ERROR) {
            if(message != nullptr) { std::cerr << message << std::endl; }
            sgl::raise((const char *)(gluErrorString(gl_error)));
        }
    }

    auto get_errors() -> std::string {
        GLenum gl_error = glGetError();
        const char * error = (const char *)(gluErrorString(gl_error));
        return std::string(error);
    }

    auto clear_errors() -> void {
        GLenum gl_error = glGetError();
        while(gl_error != GL_NO_ERROR) {
            gl_error = glGetError();
        }
    }

    auto buffer(GLenum target, size_t size, const void * data, GLenum usage) -> void {
        glBufferData(target, size, data, usage);
        sgl::raise_errors("gl.hpp @ sgl::buffer");
    }

    auto vertex_attribute(sgl::uint index, std::function<void()> fn) -> void {
        glEnableVertexAttribArray(index);
        fn();
        glDisableVertexAttribArray(index);
        sgl::raise_errors("gl.hpp @ sgl::vertex_attribute");
    }

    auto draw_arrays(GLenum mode, int first, size_t count) -> void {
        glDrawArrays(mode, first, count);
        sgl::raise_errors("gl.hpp @ sgl::draw_arrays");
    }

    class vertex_array {

        private:
        sgl::uint id;

        public:
        vertex_array() {
            glGenVertexArrays(1, &id);
            sgl::raise_errors("gl.hpp @ sgl::vertex_array");
        }
        auto bind() -> vertex_array & {
            glBindVertexArray(id);
            sgl::raise_errors("gl.hpp @ sgl::vertex_array.bind");
            return * this;
        }
    };

    class buffer_array {

        private:
        sgl::uint id;
        GLenum target;

        public:
        buffer_array() {
            glGenBuffers(1, &id);
            sgl::raise_errors("gl.hpp @ sgl::buffer_array");
        }
        auto bind(GLenum t) -> buffer_array & {
            target = t;
            glBindBuffer(target, id);
            sgl::raise_errors("gl.hpp @ sgl::buffer_array.bind");
            return * this;
        }
        auto buffer(size_t size, const void * data, GLenum usage) -> buffer_array & {
            sgl::buffer(target, size, data, usage);
            sgl::raise_errors("gl.hpp @ sgl::buffer_array.buffer");
            return * this;
        }
        auto buffer(sgl::mesh geo, GLenum usage) -> buffer_array & {
            return buffer(geo.size(), geo.data(), usage);
        }
        auto attributize(
            int    index,
            int    size,
            GLenum type       = sgl::gl_float,
            bool   normalized = false,
            int    stride     = 0,
            void * offset     = (void *)(0)
        ) -> buffer_array & {
            bind(target);
            glVertexAttribPointer(index, size, type, normalized, stride, offset);
            sgl::raise_errors("gl.hpp @ sgl::buffer_array.attributize");
            return * this;
        }
    };
}