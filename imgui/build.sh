#!/bin/sh
g++ -g -Wall imgui.cpp ../../imgui/backends/imgui_impl_sdl2.cpp ../../imgui/backends/imgui_impl_sdlrenderer2.cpp ../../imgui/imgui*.cpp `pkg-config --cflags --libs sdl2` -o imgui_cpp -I../../imgui -I../../imgui/backends && ./imgui_cpp
