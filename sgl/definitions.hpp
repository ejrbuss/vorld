#pragma once

// IO
#include <GL/glew.h>
#include <GL/glfw3.h>
// Math
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
// C++
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <streambuf>
#include <functional>
#include <stdexcept>
// imgui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

namespace sgl {

    namespace exit {
        auto const success = EXIT_SUCCESS;
        auto const failure = EXIT_FAILURE;
    }

    auto raise(const char * message) -> void {
        throw std::runtime_error(message);
    }

}