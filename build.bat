@echo off
for /f "tokens=1,* delims= " %%a in ("%*") do set ALL_BUT_FIRST=%%b
@echo on
g++ imgui/imgui_draw.cpp imgui/imgui.cpp imgui/imgui_impl_glfw_gl3.cpp %1 -Wall -g -std=gnu++14 -I . -L ./lib -lmingw32 -lm -lglu32 -lfreeglut -lopengl32 -lglfw3 -lglew32 %ALL_BUT_FIRST%