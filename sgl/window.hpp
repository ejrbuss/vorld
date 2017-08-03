#pragma once

#include "./definitions.hpp"

namespace sgl {
    namespace glfw {
        namespace hint {
            namespace version {
                auto const major = GLFW_CONTEXT_VERSION_MAJOR;
                auto const minor = GLFW_CONTEXT_VERSION_MINOR;
            }
            namespace core {
                auto const profile = GLFW_OPENGL_CORE_PROFILE;
            }
            auto const samples        = GLFW_SAMPLES;
            auto const forward_compat = GLFW_OPENGL_FORWARD_COMPAT;
            auto const profile        = GLFW_OPENGL_PROFILE;

        }
        namespace key {
            auto const unknown      = GLFW_KEY_UNKNOWN;
            auto const space        = GLFW_KEY_SPACE;
            auto const apostrophe   = GLFW_KEY_APOSTROPHE;
            auto const comma        = GLFW_KEY_COMMA;
            auto const minus        = GLFW_KEY_MINUS;
            auto const period       = GLFW_KEY_PERIOD;
            auto const slash        = GLFW_KEY_SLASH;
            auto const _0           = GLFW_KEY_0;
            auto const _1           = GLFW_KEY_1;
            auto const _2           = GLFW_KEY_2;
            auto const _3           = GLFW_KEY_3;
            auto const _4           = GLFW_KEY_4;
            auto const _5           = GLFW_KEY_5;
            auto const _6           = GLFW_KEY_6;
            auto const _7           = GLFW_KEY_7;
            auto const _8           = GLFW_KEY_8;
            auto const _9           = GLFW_KEY_9;
            auto const semicolon    = GLFW_KEY_SEMICOLON;
            auto const equal        = GLFW_KEY_EQUAL;
            auto const a            = GLFW_KEY_A;
            auto const b            = GLFW_KEY_B;
            auto const c            = GLFW_KEY_C;
            auto const d            = GLFW_KEY_D;
            auto const e            = GLFW_KEY_E;
            auto const f            = GLFW_KEY_F;
            auto const g            = GLFW_KEY_G;
            auto const h            = GLFW_KEY_H;
            auto const i            = GLFW_KEY_I;
            auto const j            = GLFW_KEY_J;
            auto const k            = GLFW_KEY_K;
            auto const l            = GLFW_KEY_L;
            auto const m            = GLFW_KEY_M;
            auto const n            = GLFW_KEY_N;
            auto const o            = GLFW_KEY_O;
            auto const p            = GLFW_KEY_P;
            auto const q            = GLFW_KEY_Q;
            auto const r            = GLFW_KEY_R;
            auto const s            = GLFW_KEY_S;
            auto const t            = GLFW_KEY_T;
            auto const u            = GLFW_KEY_U;
            auto const v            = GLFW_KEY_V;
            auto const w            = GLFW_KEY_W;
            auto const x            = GLFW_KEY_X;
            auto const y            = GLFW_KEY_Y;
            auto const z            = GLFW_KEY_Z;
            auto const left_bracket = GLFW_KEY_LEFT_BRACKET;
            auto const backslash    = GLFW_KEY_BACKSLASH;
            auto const right_bracket = GLFW_KEY_RIGHT_BRACKET;
            auto const grave_accent = GLFW_KEY_GRAVE_ACCENT;
            auto const world_1      = GLFW_KEY_WORLD_1;
            auto const world_2      = GLFW_KEY_WORLD_2;
            auto const escape       = GLFW_KEY_ESCAPE;
            auto const enter        = GLFW_KEY_ENTER;
            auto const tab          = GLFW_KEY_TAB;
            auto const backspace    = GLFW_KEY_BACKSPACE;
            auto const insert       = GLFW_KEY_INSERT;
            auto const del          = GLFW_KEY_DELETE;
            auto const right        = GLFW_KEY_RIGHT;
            auto const left         = GLFW_KEY_LEFT;
            auto const down         = GLFW_KEY_DOWN;
            auto const up           = GLFW_KEY_UP;
            auto const page_up      = GLFW_KEY_PAGE_UP;
            auto const page_down    = GLFW_KEY_PAGE_DOWN;
            auto const home         = GLFW_KEY_HOME;
            auto const end          = GLFW_KEY_END;
            auto const caps_lock    = GLFW_KEY_CAPS_LOCK;
            auto const scroll_lock  = GLFW_KEY_SCROLL_LOCK;
            auto const num_lock     = GLFW_KEY_NUM_LOCK;
            auto const print_screen = GLFW_KEY_PRINT_SCREEN;
            auto const pause        = GLFW_KEY_PAUSE;
            auto const f1           = GLFW_KEY_F1;
            auto const f2           = GLFW_KEY_F2;
            auto const f3           = GLFW_KEY_F3;
            auto const f4           = GLFW_KEY_F4;
            auto const f5           = GLFW_KEY_F5;
            auto const f6           = GLFW_KEY_F6;
            auto const f7           = GLFW_KEY_F7;
            auto const f8           = GLFW_KEY_F8;
            auto const f9           = GLFW_KEY_F9;
            auto const f10          = GLFW_KEY_F10;
            auto const f11          = GLFW_KEY_F11;
            auto const f12          = GLFW_KEY_F12;
            auto const f13          = GLFW_KEY_F13;
            auto const f14          = GLFW_KEY_F14;
            auto const f15          = GLFW_KEY_F15;
            auto const f16          = GLFW_KEY_F16;
            auto const f17          = GLFW_KEY_F17;
            auto const f18          = GLFW_KEY_F18;
            auto const f19          = GLFW_KEY_F19;
            auto const f20          = GLFW_KEY_F20;
            auto const f21          = GLFW_KEY_F21;
            auto const f22          = GLFW_KEY_F22;
            auto const f23          = GLFW_KEY_F23;
            auto const f24          = GLFW_KEY_F24;
            auto const f25          = GLFW_KEY_F25;
            auto const kp_0         = GLFW_KEY_KP_0;
            auto const kp_1         = GLFW_KEY_KP_1;
            auto const kp_2         = GLFW_KEY_KP_2;
            auto const kp_3         = GLFW_KEY_KP_3;
            auto const kp_4         = GLFW_KEY_KP_4;
            auto const kp_5         = GLFW_KEY_KP_5;
            auto const kp_6         = GLFW_KEY_KP_6;
            auto const kp_7         = GLFW_KEY_KP_7;
            auto const kp_8         = GLFW_KEY_KP_8;
            auto const kp_9         = GLFW_KEY_KP_9;
            auto const kp_decimal   = GLFW_KEY_KP_DECIMAL;
            auto const kp_divide    = GLFW_KEY_KP_DIVIDE;
            auto const kp_multiply  = GLFW_KEY_KP_MULTIPLY;
            auto const kp_subtract  = GLFW_KEY_KP_SUBTRACT;
            auto const kp_add       = GLFW_KEY_KP_ADD;
            auto const kp_enter     = GLFW_KEY_KP_ENTER;
            auto const kp_equal     = GLFW_KEY_KP_EQUAL;
            auto const left_shift   = GLFW_KEY_LEFT_SHIFT;
            auto const left_control = GLFW_KEY_LEFT_CONTROL;
            auto const left_alt     = GLFW_KEY_LEFT_ALT;
            auto const left_super   = GLFW_KEY_LEFT_SUPER;
            auto const right_shift  = GLFW_KEY_RIGHT_SHIFT;
            auto const right_control = GLFW_KEY_RIGHT_CONTROL;
            auto const right_alt    = GLFW_KEY_RIGHT_ALT;
            auto const right_super  = GLFW_KEY_RIGHT_SUPER;
            auto const menu         = GLFW_KEY_MENU;
            auto const last         = GLFW_KEY_LAST;
        }
        auto sticky_keys = GLFW_STICKY_KEYS;
        auto press       = GLFW_PRESS;

        class window {

            private:
            GLFWwindow * glfw_window = nullptr;
            glm::vec4 clear_color;

            glm::vec3 position = glm::vec3( 0, 0, 5 );
            glm::vec3 direction;
            glm::mat4 proj_view_with_position;
            glm::mat4 proj_view;

            const float init_horiz_angle = 3.14;
            float horiz_angle            = init_horiz_angle; // horizontal angle : toward -Z
            const float init_vert_angle  = 0.0f;
            float vert_angle             = init_vert_angle;  // vertical angle : 0, look at the horizon
            float fov                    = 45.0f;            // Field of View

            double last_time;
            float delta_time;
            float init_speed    = 3.0f;
            float speed         = init_speed; // 3 units / second
            float mouse_speed   = 0.1f;
            bool lock_cursor    = true;
            bool lock_cursor_up = true;

            public:
            // key bindings
            int forward       = sgl::glfw::key::w;
            int back          = sgl::glfw::key::s;
            int right         = sgl::glfw::key::d;
            int left          = sgl::glfw::key::a;
            int up            = sgl::glfw::key::q;
            int down          = sgl::glfw::key::e;
            int speed_up      = sgl::glfw::key::equal;
            int slow_down     = sgl::glfw::key::minus;
            int fov_up        = sgl::glfw::key::left_bracket;
            int fov_down      = sgl::glfw::key::right_bracket;
            int toggle_cursor = sgl::glfw::key::left_control;

            double width;
            double hwidth;
            double height;
            double hheight;

            window() {
                if(!glfwInit()) { sgl::raise("Failed to initialize GLFW"); }
            }
            auto hint(int hint, int value) -> window & {
                glfwWindowHint(hint, value);
                return * this;
            }
            auto create(int w, int h, const char * title, GLFWmonitor * monitor, GLFWwindow * share) -> window & {
                width   = w;
                height  = h;
                hwidth  = width / 2;
                hheight = height / 2;
                glfw_window = glfwCreateWindow(width, height, title, monitor, share);
                if(glfw_window == NULL) {
                    glfwTerminate();
                    sgl::raise("Failed to open GLFW window");
                }
                // Setup ImGui binding
                ImGui_ImplGlfwGL3_Init(glfw_window, true);
                return * this;
            }
            auto glew() -> window & {
                glfwMakeContextCurrent(glfw_window);
                glewExperimental = true;
                if(glewInit() != GLEW_OK) { sgl::raise("Failed to initialize GLEW"); }
                sgl::clear_errors();
                return * this;
            }
            auto inputMode(int mode, int value) -> window & {
                glfwSetInputMode(glfw_window, mode, value);
                return * this;
            }
            auto swap() -> window & {
                glfwSwapBuffers(glfw_window);
                return * this;
            }
            auto delta() -> float {
                return delta_time;
            }
            auto poll(bool mouse = true) -> window & {

                double mouse_x = 0;
                double mouse_y = 0;

                // polling events
                glfwPollEvents();
                if(mouse) {
                    glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
                }

                double current_time = glfwGetTime();
                delta_time          = current_time - last_time;
                last_time           = current_time;

                if(lock_cursor) {
                    horiz_angle += mouse_speed * delta_time * float(hwidth  - mouse_x);
                    vert_angle  += mouse_speed * delta_time * float(hheight - mouse_y);
                }
                if(key(toggle_cursor) == sgl::glfw::press) {
                    if(lock_cursor_up) {
                        lock_cursor    = !lock_cursor;
                        lock_cursor_up = false;
                        if(lock_cursor) {
                            glfwSetCursorPos(glfw_window, hwidth, hheight);
                        }
                    }
                } else {
                    lock_cursor_up = true;
                }

                direction = glm::vec3(
                    cos(vert_angle) * sin(horiz_angle),
                    sin(vert_angle),
                    cos(vert_angle) * cos(horiz_angle)
                );

                glm::vec3 horiz(
                    sin(horiz_angle - init_horiz_angle / 2.0f),
                    0,
                    cos(horiz_angle - init_horiz_angle / 2.0f)
                );

                // Handle input
                {
                    // Normal Movement
                    if(key(forward) == sgl::glfw::press) { position += direction * delta_time * speed; }
                    if(key(back)    == sgl::glfw::press) { position -= direction * delta_time * speed; }
                    if(key(right)   == sgl::glfw::press) { position += horiz     * delta_time * speed; }
                    if(key(left)    == sgl::glfw::press) { position -= horiz     * delta_time * speed; }
                    // Vertical Movement
                    if(key(up)      == sgl::glfw::press) { position += glm::vec3(0, delta_time * speed, 0); }
                    if(key(down)    == sgl::glfw::press) { position -= glm::vec3(0, delta_time * speed, 0); }
                    // Speed
                    if(key(speed_up)  == sgl::glfw::press) { speed += delta_time * init_speed; }
                    if(key(slow_down) == sgl::glfw::press) { speed -= delta_time * init_speed; }
                }

                glm::vec3 up = glm::cross(horiz, direction);
                //glm::vec3 up = glm::normalize(position);
                // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
                glm::mat4 proj = glm::perspective(fov, float(width) / float(height), 0.1f, 100.0f);
                // Camera matrix
                glm::mat4 view = glm::lookAt(
                    position,             // Camera is here
                    position + direction, // and looks here : at the same position, plus "direction"
                    up                    // Head is up (set to 0,-1,0 to look upside-down)
                );
                proj_view_with_position =  proj * view;
                view = glm::lookAt(
                    glm::vec3(0, 0, 0),
                    direction,
                    up
                );
                proj_view = proj * view;
                return * this;
            }
            auto matrix(bool with_position = true) -> glm::mat4 {
                if(with_position) {
                    return proj_view_with_position;
                }
                return proj_view;
            }
            auto view_position() -> glm::vec3 {
                return position;
            }
            auto view_direction() -> glm::vec3 {
                return direction;
            }
            auto current_speed() -> float {
                return speed;
            }
            auto imgui_on() -> bool {
                return !lock_cursor;
            }
            auto clear(float r, float g, float b, float a) -> window & {
                clear_color.r = r;
                clear_color.g = g;
                clear_color.b = b;
                clear_color.a = a;
                return * this;
            }
            auto clear(glm::vec4 color) -> window & {
                return clear(color.r, color.g, color.b, color.a);
            }
            auto shouldClose() -> int {
                return glfwWindowShouldClose(glfw_window);
            }
            auto key(int key) -> bool {
                return glfwGetKey(glfw_window, key);
            }
            auto loop(const std::function<void()> & fn) -> window & {
                last_time = glfwGetTime();
                glfwSetCursorPos(glfw_window, hwidth, hheight);
                do {

                    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
                    glClear(sgl::color_buffer_bit | sgl::depth_buffer_bit);
                    glEnable(sgl::depth_test);
                    glDepthFunc(sgl::less);
                    fn();
                    if(!lock_cursor) {
                        ImGui::Render();
                    }
                    poll();
                    if(lock_cursor) {
                        glfwSetCursorPos(glfw_window, hwidth, hheight);
                        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    } else {
                        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        ImGui_ImplGlfwGL3_NewFrame();
                    }
                    swap();
                } while(
                    key(sgl::glfw::key::escape) != sgl::glfw::press &&
                    !shouldClose()
                );
                return * this;
            }
        };

    }
}