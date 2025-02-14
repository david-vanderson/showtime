#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>

char name[50] = "Alice";
bool editing = false;

void myFrame() {
    // start floating window
    ImGui::Begin("Dear ImGui C++");

    // spacer
    ImGui::Dummy(ImVec2(0, 10));

    if (!editing) {
        ImGui::Text("Hello %s", name);
    } else {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Name?");
        ImGui::SameLine();
        ImGui::PushItemWidth(180);
        if (ImGui::InputText("label##extra", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue)) {
            editing = false;
        }

        ImGui::PopItemWidth();
    }

    ImGui::Dummy(ImVec2(0, 10));

    // ID is hash of label
    if (ImGui::Button(editing ? "Done" : "Edit")) {
        editing = !editing;
    }

    ImGui::Dummy(ImVec2(0, 10));

    ImGui::Text("%.0f FPS", ImGui::GetIO().Framerate);

    // end floating window
    ImGui::End();
}

int main(int, char**) {
    // init sdl and OS window
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui C++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    ImFontConfig font_cfg = ImFontConfig();
    font_cfg.SizePixels = 28; // make demo big
    io.Fonts->AddFontDefault(&font_cfg);


    bool done = false;
    while (!done) {
        // pass events to imgui
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        // don't do anything if minimized
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        // frame setup
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // just a normal function call
        myFrame();

        // frame end
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        // waits for vsync
        SDL_RenderPresent(renderer);

        // no wait for events
    }

    // app cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
