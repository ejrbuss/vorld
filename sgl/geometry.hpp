#pragma once

#include "./definitions.hpp"

namespace sgl {
    class mesh {

        private:
        std::vector<float> values;
        std::vector<float> uvs;

        public:

        mesh() {}
        auto add(std::vector<float> v) -> mesh & {
            values = v;
            return * this;
        }
        auto add_uvs(std::vector<float> v) -> mesh & {
            uvs = v;
            return * this;
        }
        auto add(float x, float y, float z) -> mesh & {
            values.push_back(x);
            values.push_back(y);
            values.push_back(z);
            return * this;
        }
        auto add_uvs(float x, float y) -> mesh & {
            uvs.push_back(x);
            uvs.push_back(y);
            return * this;
        }
        auto add(const glm::vec3 & v) -> mesh & {
            return add(v.x, v.y, v.z);
        }
        auto add_uvs(const glm::vec2 & v) -> mesh & {
            return add_uvs(v.x, v.y);
        }
        auto data() -> float * {
            return &values[0];
        }
        auto size() -> int {
            return sizeof(float) * values.size();
        }
        auto verticies() -> int {
            return values.size() / 3;
        }
        auto subdivide(int power = 1) -> mesh & {
            std::vector<float> new_values;
            std::vector<float> new_uvs;
            for(int i = 0; i * 9 + 8 < (int)(values.size()); i++) {
                int t = i * 9;

                // original triangle
                glm::vec3 v1(values[t + 0], values[t + 1], values[t + 2]);
                glm::vec3 v2(values[t + 3], values[t + 4], values[t + 5]);
                glm::vec3 v3(values[t + 6], values[t + 7], values[t + 8]);

                // lerped points
                glm::vec3 v1v2 = (v1 + v2) / 2.0f;
                glm::vec3 v2v3 = (v2 + v3) / 2.0f;
                glm::vec3 v3v1 = (v3 + v1) / 2.0f;

                // new triangles
                sgl::util::push_triangle(new_values, v1,   v1v2, v3v1);
                sgl::util::push_triangle(new_values, v1v2, v2v3, v3v1);
                sgl::util::push_triangle(new_values, v3v1, v2v3, v3);
                sgl::util::push_triangle(new_values, v1v2, v2,   v2v3);

                if(i * 6 + 5 < (int)(uvs.size())) {
                    int uvv = i * 6;

                    // original uv coordinates
                    glm::vec2 uv1(uvs[uvv + 0], uvs[uvv + 1]);
                    glm::vec2 uv2(uvs[uvv + 2], uvs[uvv + 3]);
                    glm::vec2 uv3(uvs[uvv + 4], uvs[uvv + 5]);

                    // lerped points
                    glm::vec2 uv1uv2 = (uv1 + uv2) / 2.0f;
                    glm::vec2 uv2uv3 = (uv2 + uv3) / 2.0f;
                    glm::vec2 uv3uv1 = (uv3 + uv1) / 2.0f;

                    // new uv coordinates
                    sgl::util::push_triangle(new_uvs, uv1,    uv1uv2, uv3uv1);
                    sgl::util::push_triangle(new_uvs, uv1uv2, uv2uv3, uv3uv1);
                    sgl::util::push_triangle(new_uvs, uv3uv1, uv2uv3, uv3);
                    sgl::util::push_triangle(new_uvs, uv1uv2, uv2,    uv2uv3);

                }
            }
            values      = new_values;
            uvs         = new_uvs;
            if(power > 1) {
                return subdivide(power - 1);
            }
            return * this;
        }
        auto uvs_mesh() -> mesh {
            sgl::mesh mesh;
            mesh.add(uvs);
            return mesh;
        }
    };
    namespace geo {
        auto triangle() -> mesh {
            sgl::mesh triangle;
            triangle.add(std::vector<float> {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f,  1.0f, 0.0f
            });
            return triangle;
        }
        auto cube() -> mesh {
            sgl::mesh cube;
            cube.add(std::vector<float> {
                // triangle 1
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                // triangle 2
                1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                // triangle 3
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                // triangle 4
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                // triangle 5
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,
                // triangle 6
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                // triangle 7
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                // triangle 8
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                // triangle 9
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                // triangle 10
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                // triangle 11
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                // triangle 12
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            })
            .add_uvs(std::vector<float> {
                /* 1 sided texture */

                // xyz that is the same for the triangle is ignored
                // -1 maps to 0, 1 maps to 1
                // left to right

                // triangle 1
                0, 0,
                0, 1,
                1, 1,
                // triangle 2
                1, 1,
                0, 0,
                0, 1,
                // triangle 3
                1, 1,
                0, 0,
                1, 0,
                // triangle 4
                1, 1,
                1, 0,
                0, 0,
                // triangle 5
                0, 0,
                1, 1,
                1, 0,
                // triangle 6
                1, 1,
                0, 1,
                0, 0,
                // triangle 7
                0, 1,
                0, 0,
                1, 0,
                // triangle 8
                1, 1,
                0, 0,
                1, 0,
                // triangle 9
                0, 0,
                1, 1,
                0, 1,
                // triangle 10
                1, 1,
                1, 0,
                0, 0,
                // triangle 11
                1, 1,
                0, 0,
                0, 1,
                // triangle 12
                1, 1,
                0, 1,
                1, 0
            });
            return cube;
        }
        auto skybox() -> mesh {
            sgl::mesh skybox;
            skybox.add(std::vector<float> {
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            });
            return skybox;
        }
    }
}