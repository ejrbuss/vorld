#pragma once

#include "./definitions.hpp"
#include "./gl.hpp"

namespace sgl {
    class shader {

        private:
        sgl::uint id;
        std::string vertex_source;
        std::string fragment_source;

        public:
        shader() {}
        auto vertex(std::string source) -> shader & {
            vertex_source = source;
            return * this;
        }
        auto fragment(std::string source) -> shader & {
            fragment_source = source;
            return * this;
        }
        auto load() -> shader & {
            sgl::uint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
            sgl::uint vertex_id   = glCreateShader(GL_VERTEX_SHADER);

            const char * vertex_source_cstr   = vertex_source.c_str();
            const char * fragment_source_cstr = fragment_source.c_str();
            int result                        = false;
            int info_length;

            glShaderSource(vertex_id, 1, &vertex_source_cstr, NULL);
            glCompileShader(vertex_id);

            glGetShaderiv(vertex_id, GL_COMPILE_STATUS,  &result);
            glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &info_length);
            if (!result){
                char vertex_error_message[info_length + 1];
                glGetShaderInfoLog(vertex_id, info_length, NULL, vertex_error_message);
                sgl::raise(vertex_error_message);
            }

            glShaderSource(fragment_id, 1, &fragment_source_cstr, NULL);
            glCompileShader(fragment_id);

            glGetShaderiv(fragment_id, GL_COMPILE_STATUS,  &result);
            glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &info_length);
            if(!result){
                char fragment_error_message[info_length + 1];
                glGetShaderInfoLog(fragment_id, info_length, NULL, fragment_error_message);
                sgl::raise(fragment_error_message);
            }

            sgl::uint program_id = glCreateProgram();
            glAttachShader(program_id, vertex_id);
            glAttachShader(program_id, fragment_id);
            glLinkProgram(program_id);

            glGetProgramiv(program_id, GL_LINK_STATUS, &result);
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_length);
            if(!result){
                char program_error_message[info_length + 1];
                glGetShaderInfoLog(program_id, info_length, NULL, program_error_message);
                sgl::raise(program_error_message);
            }

            glDetachShader(program_id, vertex_id);
            glDetachShader(program_id, fragment_id);

            glDeleteShader(vertex_id);
            glDeleteShader(fragment_id);

            id = program_id;
            sgl::raise_errors("shaders.hpp @ sgl::shader.load");
            return * this;
        }
        auto use() -> shader & {
            glUseProgram(id);
            sgl::raise_errors("shaders.hpp @ sgl::shader.use");
            return * this;
        }
        auto uniform(glm::vec4 v, const char * name) -> shader & {
            use();
            sgl::uint vec_id = glGetUniformLocation(id, name);
            glUniform4f(vec_id, v.x, v.y, v.z, v.w);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(vec4)");
            return * this;
        }
        auto uniform(glm::vec3 v, const char * name) -> shader & {
            use();
            sgl::uint vec_id = glGetUniformLocation(id, name);
            glUniform3f(vec_id, v.x, v.y, v.z);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(vec3)");
            return * this;
        }
        auto uniform(glm::vec2 v, const char * name) -> shader & {
            use();
            sgl::uint vec_id = glGetUniformLocation(id, name);
            glUniform2f(vec_id, v.x, v.y);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(vec2)");
            return * this;
        }
        auto uniform(bool b, const char * name) -> shader & {
            use();
            sgl::uint int_id = glGetUniformLocation(id, name);
            glUniform1i(int_id, b);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(bool)");
            return * this;
        }
        auto uniform(float f, const char * name) -> shader & {
            use();
            sgl::uint float_id = glGetUniformLocation(id, name);
            glUniform1f(float_id, f);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(float)");
            return * this;
        }
        auto uniform(glm::mat4 matrix, const char * name, bool transpose = false) -> shader & {
            use();
            sgl::uint matrix_id = glGetUniformLocation(id, name);
            glUniformMatrix4fv(matrix_id, 1, transpose, &matrix[0][0]);
            sgl::raise_errors("shaders.hpp @ sgl::shader.uniform(mat4)");
            return * this;
        }
        auto texture(sgl::texture texture, const char * name, int offset = 0, GLenum type = sgl::texture_2d) -> shader & {
            use();
            sgl::uint texture_id = glGetUniformLocation(id, name);
            glActiveTexture(GL_TEXTURE0);
            texture.bind(type);
            glUniform1i(texture_id, offset);
            sgl::raise_errors("shaders.hpp @ sgl::shader.texture");
            return * this;
        }
    };
}