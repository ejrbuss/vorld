// Eric Buss V00818492

#include <sgl/sgl>

auto main() -> int {

    try {

        namespace hint = sgl::glfw::hint;

        const int width    = 2500;
        const int height   = 1000;

        sgl::glfw::window window;
        window
            .hint(hint::samples, 4)
            .hint(hint::version::major, 3)
            .hint(hint::version::minor, 3)
            .hint(hint::forward_compat, true)
            .hint(hint::profile, hint::core::profile)
            .clear(0.0f, 0.0f, 0.0f, 0.0f)
            .create(width, height, "Vorld", nullptr, nullptr)
            //.create(width, height, "Vorld", glfwGetPrimaryMonitor(), nullptr)
            .glew()
            .inputMode(sgl::glfw::sticky_keys, true);

        std::cout << "DEBUG: window constructed" << std::endl;

        sgl::enable(sgl::cull_face);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        sgl::vertex_array vao;
        vao.bind();

        // Load geometry
        std::cout << "DEBUG: Loading geometry...";

        sgl::mesh skybox_cube = sgl::geo::skybox();

        sgl::buffer_array sb_buffer;
        sb_buffer
            .bind(sgl::array_buffer)
            .buffer(skybox_cube, sgl::static_draw);

        sgl::mesh sun_cube = sgl::geo::cube();
        sun_cube.subdivide(5);

        sgl::buffer_array sn_buffer;
        sn_buffer
            .bind(sgl::array_buffer)
            .buffer(sun_cube, sgl::static_draw);

        sgl::mesh fluid_cube = sgl::geo::cube();
        fluid_cube.subdivide(6);

        sgl::buffer_array fl_buffer;
        fl_buffer
            .bind(sgl::array_buffer)
            .buffer(fluid_cube, sgl::static_draw);

        sgl::mesh planet_cube = sgl::geo::cube();
        planet_cube.subdivide(7);

        sgl::buffer_array fl_uv_buffer;
        fl_uv_buffer
            .bind(sgl::array_buffer)
            .buffer(fluid_cube.uvs_mesh(), sgl::static_draw);

        sgl::buffer_array vt_buffer;
        vt_buffer
            .bind(sgl::array_buffer)
            .buffer(planet_cube, sgl::static_draw);

        sgl::buffer_array uv_buffer;
        uv_buffer
            .bind(sgl::array_buffer)
            .buffer(planet_cube.uvs_mesh(), sgl::static_draw);

        std::cout << "Finished." << std::endl << "DEBUG: compiling shaders...";

        // Load shaders
        sgl::shader shader;
        shader
            .vertex(sgl::util::file_as_string("shaders/vertex.glsl"))
            .fragment(sgl::util::file_as_string("shaders/fragment.glsl"))
            .load();

        std::cout << "Finished." << std::endl << "DEBUG: Loading textures...";

        // Load textures
        sgl::texture skybox;
        skybox
            .load_cubemap(std::vector<std::string> {
                "skybox/6.jpg",
                "skybox/2-2.jpg",
                "skybox/1.jpg",
                "skybox/3.jpg",
                "skybox/5.jpg",
                "skybox/4.jpg"
            })
            .int_parameter(sgl::option::min_filter, sgl::option::linear, sgl::texture_cube_map)
            .int_parameter(sgl::option::mag_filter, sgl::option::linear, sgl::texture_cube_map)
            .int_parameter(sgl::option::texture_wrap_S, sgl::option::clamp_to_edge, sgl::texture_cube_map)
            .int_parameter(sgl::option::texture_wrap_T, sgl::option::clamp_to_edge, sgl::texture_cube_map)
            .int_parameter(sgl::option::texture_wrap_R, sgl::option::clamp_to_edge, sgl::texture_cube_map);

        sgl::texture texture;
        texture
            //.load("textures/grass.tga", sgl::invert_y) // Mosaic weirdness
            .load("textures/planet.tga", sgl::invert_y)
            .int_parameter(sgl::option::mag_filter, sgl::option::linear)
            .int_parameter(sgl::option::min_filter, sgl::option::linear_mipmap);

        sgl::texture sun_texture;
        sun_texture
            .load("textures/sun.tga", sgl::invert_y)
            .int_parameter(sgl::option::mag_filter, sgl::option::linear)
            .int_parameter(sgl::option::min_filter, sgl::option::linear_mipmap);

        sgl::texture water_texture;
        water_texture
            .load("textures/water.jpg")
            .int_parameter(sgl::option::mag_filter, sgl::option::linear)
            .int_parameter(sgl::option::min_filter, sgl::option::linear_mipmap);

        std::cout << "Finished." << std::endl << "DEBUG: Starting main loop...";

        glm::vec3 sun_position = glm::vec3(20, -5, 0);
        glm::vec3 sun_color    = glm::vec3(0.35f, 0.85f, 0.9f);
        float sun_power        = 0.7f;

        float day_time_factor      = 1.0f;
        float texture_displacement = 0.0f;

        // Configure Imgui
        ImGui::GetIO().FontGlobalScale = 1.75f;


        // Toggles
        bool toggle_wireframes = false;
        bool toggle_normals    = false;
        bool toggle_skybox     = true;
        bool toggle_clouds     = true;
        bool toggle_water      = true;
        bool toggle_lighting   = true;

        // Planet
        glm::vec2 planet_octave_1 = glm::vec2(1.0f,  2.0f);
        glm::vec2 planet_octave_2 = glm::vec2(0.5f,  8.0f);
        glm::vec2 planet_octave_3 = glm::vec2(0.25f, 32.0f);
        float planet_magnitude = 0.2f;

        glm::vec4 planet_material = glm::vec4(0.1f, 0.4f, 0.1f, 8.0f);

        // Water
        glm::vec2 water_octave_1 = glm::vec2(0.25f, 2.0f);
        glm::vec2 water_octave_2 = glm::vec2(1.0f,  9.0f);
        glm::vec2 water_octave_3 = glm::vec2(0.5f,  21.0f);
        float water_magnitude = planet_magnitude / 4.0f;

        glm::vec4 water_material = glm::vec4(0.1f, 0.5f, 2.0f, 32.0f);

        // Cloud
        glm::vec2 cloud_octave_1 = glm::vec2(0.8f, 3.0f);
        glm::vec2 cloud_octave_2 = glm::vec2(0.6f, 10.0f);
        glm::vec2 cloud_octave_3 = glm::vec2(0.3f, 24.0f);
        float cloud_magnitude = planet_magnitude / 4.0f;

        glm::vec4 cloud_material = glm::vec4(0.5f, 1.0f, 0.3f, 4.0f);

        window.loop([&]() {

//--- Imgui ------------------------------------------------------------------//

            if(window.imgui_on()) {

                // Setup

                ImGui::SetWindowPos(ImVec2(5.0f, 5.0f));
                ImGui::SetWindowSize(ImVec2(450.0f, 990.0f));

                // Stats

                auto const framerate = ImGui::GetIO().Framerate;
                auto const position  = window.view_position();
                auto const speed     = window.current_speed();

                ImGui::Text("FPS: %.1f (%.3f ms/frame)", framerate, 100.0f / framerate);
                ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
                ImGui::Text("Speed: %.2f", speed);

                ImGui::SliderFloat("Time", &day_time_factor, 1.0f, 200.0f);
                ImGui::SliderFloat("Sun Distance", &sun_position.x, 0.0f, 50.0f);

                // Toggles

                if (ImGui::CollapsingHeader("Toggles")) {
                    ImGui::Checkbox("Wireframes", &toggle_wireframes);
                    ImGui::Checkbox("Normals",    &toggle_normals);
                    ImGui::Checkbox("Skybox",     &toggle_skybox);
                    ImGui::Checkbox("Clouds",     &toggle_clouds);
                    ImGui::Checkbox("Water",      &toggle_water);
                    ImGui::Checkbox("Lighting",   &toggle_lighting);
                }

                // Terrain Noise

                if (ImGui::CollapsingHeader("Terrain Noise")) {
                    ImGui::SliderFloat("Magnitude p", &planet_magnitude, 0.01f, 2.0f);
                    ImGui::Text("Octave 1");
                    ImGui::SliderFloat("Magnitude p1", &planet_octave_1.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency p1", &planet_octave_1.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 2");
                    ImGui::SliderFloat("Magnitude p2", &planet_octave_2.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency p2", &planet_octave_2.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 3");
                    ImGui::SliderFloat("Magnitude p3", &planet_octave_3.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency p3", &planet_octave_3.y, 1.0f, 32.0f);
                }

                // Terrain Material

                if (ImGui::CollapsingHeader("Terrain Material")) {
                    ImGui::SliderFloat("Ambient p",  &planet_material.x, 0.0f, 2.0f);
                    ImGui::SliderFloat("Diffuse p",  &planet_material.y, 0.0f, 2.0f);
                    ImGui::SliderFloat("Specular p", &planet_material.z, 0.0f, 2.0f);
                    ImGui::SliderFloat("Power p",    &planet_material.w, 0.0f, 64.0f);
                }

                // Water Noise

                if (ImGui::CollapsingHeader("Water Noise")) {
                    ImGui::SliderFloat("Magnitude w", &water_magnitude, 0.01f, 2.0f);
                    ImGui::Text("Octave 1");
                    ImGui::SliderFloat("Magnitude w1", &water_octave_1.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency w1", &water_octave_1.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 2");
                    ImGui::SliderFloat("Magnitude w2", &water_octave_2.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency w2", &water_octave_2.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 3");
                    ImGui::SliderFloat("Magnitude w3", &water_octave_3.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency w3", &water_octave_3.y, 1.0f, 32.0f);
                }

                // Water Material

                if (ImGui::CollapsingHeader("Water Material")) {
                    ImGui::SliderFloat("Ambient w",  &water_material.x, 0.0f, 2.0f);
                    ImGui::SliderFloat("Diffuse w",  &water_material.y, 0.0f, 2.0f);
                    ImGui::SliderFloat("Specular w", &water_material.z, 0.0f, 2.0f);
                    ImGui::SliderFloat("Power w",    &water_material.w, 0.0f, 64.0f);
                }

                // Cloud noise

                if (ImGui::CollapsingHeader("Cloud Noise")) {
                    ImGui::SliderFloat("Magnitude c", &cloud_magnitude, 0.01f, 2.0f);
                    ImGui::Text("Octave 1");
                    ImGui::SliderFloat("Magnitude c1", &cloud_octave_1.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency c1", &cloud_octave_1.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 2");
                    ImGui::SliderFloat("Magnitude c2", &cloud_octave_2.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency c2", &cloud_octave_2.y, 1.0f, 32.0f);
                    ImGui::Text("Octave 3");
                    ImGui::SliderFloat("Magnitude c3", &cloud_octave_3.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Frequency c3", &cloud_octave_3.y, 1.0f, 32.0f);
                }

                // Cloud Material

                if (ImGui::CollapsingHeader("Cloud Material")) {
                    ImGui::SliderFloat("Ambient c",  &cloud_material.x, 0.0f, 2.0f);
                    ImGui::SliderFloat("Diffuse c",  &cloud_material.y, 0.0f, 2.0f);
                    ImGui::SliderFloat("Specular c", &cloud_material.z, 0.0f, 2.0f);
                    ImGui::SliderFloat("Power c",    &cloud_material.w, 0.0f, 64.0f);
                }

                // Lighting

                if (ImGui::CollapsingHeader("Lighting")) {
                    ImGui::SliderFloat("Light Red",   &sun_color.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Light Green", &sun_color.y, 0.0f, 1.0f);
                    ImGui::SliderFloat("Light Blue",  &sun_color.z, 0.0f, 1.0f);
                    ImGui::SliderFloat("Power",       &sun_power, 0.0f, 1.0f);

                }

            }

//--- IO Handling ------------------------------------------------------------//

            sgl::wireframes(toggle_wireframes);

            shader.uniform(toggle_normals,   "debug_normal");
            shader.uniform(!toggle_lighting, "debug_no_light");

//--- Skybox Shader ----------------------------------------------------------//

            // Default Matrix adjustments
            // Rotate the sun!
            sun_position         = glm::vec3(glm::rotate(window.delta() * day_time_factor / 100.0f, glm::vec3(0.2, 0.9, 0.1)) * glm::vec4(sun_position, 1));
            // Get the current view from the window
            glm::mat4 proj_view  = window.matrix(false);
            // Default model to identity
            glm::mat4 model      = glm::mat4(1.0);
            // Used to modulate water and clouds
            texture_displacement = texture_displacement + day_time_factor / 200.0f;

            if(toggle_skybox) {

                sgl::depth_mask(false); // Because we are drawing the skybox, depth
                                        // must be disabled
                // Shader Configuration
                shader
                    // Vertex
                    .uniform(proj_view,                         "proj_view")
                    .uniform(model,                             "model")
                    // Fragment
                    .uniform(true,                              "skybox")
                    .uniform(window.view_position(),            "view_position")
                    .uniform(window.view_direction(),           "view_direction")
                    .uniform(sun_color,                         "light_color")
                    .uniform(sun_power,                         "light_power")
                    .texture(skybox, "skybox_texture", 0, sgl::texture_cube_map);

                // Rendering
                sgl::vertex_attribute(0, [&]() {
                    sb_buffer.attributize(0, 3);
                    sgl::draw_arrays(sgl::triangles, 0, skybox_cube.verticies());
                });

                // Cleanup
                shader
                    .uniform(false, "skybox");
                sgl::depth_mask(true);
            }

//--- Sun Shader -------------------------------------------------------------//

            // Matrix adjustments
            proj_view = window.matrix();
            model     =
                glm::translate(sun_position) *
                glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

            // Shader configuration
            shader
                // Disable skybox and enable sun
                .uniform(true,                              "sun")
                // Vertex shader
                .uniform(glm::vec2( 1.0f,  2.0f ),          "octave_1")
                .uniform(glm::vec2( 0.5f,  8.0f ),          "octave_2")
                .uniform(glm::vec2( 0.25f, 32.0f),          "octave_3")
                .uniform(0.2f,                              "magnitude")
                .uniform(proj_view,                         "proj_view")
                .uniform(model,                             "model")
                // Fragment shader
                .uniform(4.0f,                              "tile_repeat")
                .uniform(glm::vec4(4.0f, 2.0f, 4.0f, 1.0f), "material")
                .uniform(sun_color,                         "light_color")
                .uniform(sun_power,                         "light_power")
                .uniform(window.view_position(),            "view_position")
                .uniform(true,                              "sample_1d")
                .texture(sun_texture,                       "texture");

            // Rendering
            sgl::vertex_attribute(0, [&]() {
                sn_buffer.attributize(0, 3);
                sgl::draw_arrays(sgl::triangles, 0, sun_cube.verticies());
            });

            // Cleanup
            shader
                .uniform(false, "sun")
                .uniform(false, "sample_1d");

//--- Planet Shader ----------------------------------------------------------//

            // Matrix adjustments
            model = glm::mat4(1.0);

            // Shader configuration
            shader
                // Vertex shader
                .uniform(planet_octave_1,                   "octave_1")
                .uniform(planet_octave_2,                   "octave_2")
                .uniform(planet_octave_3,                   "octave_3")
                .uniform(planet_magnitude,                  "magnitude")
                .uniform(proj_view,                         "proj_view")
                .uniform(model,                             "model")
                // Fragment shader
                .uniform(4.0f,                              "tile_repeat")
                .uniform(planet_material,                   "material")
                .uniform(sun_position,                      "light_position")
                .uniform(sun_color,                         "light_color")
                .uniform(sun_power,                         "light_power")
                .uniform(window.view_position(),            "view_position")
                .uniform(true,                              "sample_1d")
                .texture(texture,                           "texture");

            // Rendering
            sgl::vertex_attribute(0, [&]() {
            sgl::vertex_attribute(1, [&]() {
                vt_buffer.attributize(0, 3);
                uv_buffer.attributize(1, 2);
                sgl::draw_arrays(sgl::triangles, 0, planet_cube.verticies());
            });
            });

            // Cleanup
            shader
                .uniform(false, "sample_1d");

//--- Water Shader -----------------------------------------------------------//

        if(toggle_water) {

            // Matrix adjustments
            model =
                glm::scale(glm::vec3(1 + planet_magnitude * 0.3));

            // Shader configuration
            shader
                .uniform(true,                               "water")
                // Vertex shader
                .uniform(water_octave_1,                     "octave_1")
                .uniform(water_octave_2,                     "octave_2")
                .uniform(water_octave_3,                     "octave_3")
                .uniform(water_magnitude,                    "magnitude")
                .uniform(proj_view,                          "proj_view")
                .uniform(model,                              "model")
                // Fragment shader
                .uniform(32.0f,                              "tile_repeat")
                .uniform(water_material,                     "material")
                .uniform(sun_position,                       "light_position")
                .uniform(sun_color,                          "light_color")
                .uniform(sun_power,                          "light_power")
                .uniform(window.view_position(),             "view_position")
                .uniform(true,                               "sample_2d")
                .uniform(texture_displacement,               "texture_displacement")
                .texture(water_texture,                      "texture");

            // Rendering
            sgl::vertex_attribute(0, [&]() {
            sgl::vertex_attribute(1, [&]() {
                fl_buffer.attributize(0, 3);
                fl_uv_buffer.attributize(1, 2);
                sgl::draw_arrays(sgl::triangles, 0, fluid_cube.verticies());
            });
            });

            shader
                .uniform(false, "water")
                .uniform(false, "sample_2d")
                .uniform(0.0f,  "texture_displacement");

        }

//--- Cloud Shader -----------------------------------------------------------//

        if(toggle_clouds) {

                        // Matrix Adjustments
            model =
                glm::scale(glm::vec3(1.15f));

            // Shader configuration
            shader
                .uniform(true,                               "cloud")
                // Vertex shader
                .uniform(cloud_octave_1,                     "octave_1")
                .uniform(cloud_octave_2,                     "octave_2")
                .uniform(cloud_octave_3,                     "octave_3")
                .uniform(cloud_magnitude,                    "magnitude")
                .uniform(proj_view,                          "proj_view")
                .uniform(model,                              "model")
                // Fragment shader
                .uniform(32.0f,                              "tile_repeat")
                .uniform(cloud_material,                     "material")
                .uniform(sun_position,                       "light_position")
                .uniform(sun_color,                          "light_color")
                .uniform(sun_power,                          "light_power")
                .uniform(window.view_position(),             "view_position")
                .uniform(texture_displacement,               "texture_displacement");

            // Rendering
            sgl::vertex_attribute(0, [&]() {
                fl_buffer.attributize(0, 3);
                sgl::draw_arrays(sgl::triangles, 0, fluid_cube.verticies());
            });

            // Cleanup
            shader
                .uniform(false, "cloud")
                .uniform(0.0f,  "texture_displacement");

        }

//--- End of Shaders ---------------------------------------------------------//

        });
        std::cout << "Finished." << std::endl;
    } catch(std::exception & ex) {
        std::cout << std::endl << ex.what() << std::endl;
    }
    return sgl::exit::success;
}