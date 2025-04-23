#include "../include/simulation.hpp"
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;

MicroscopeSim sim;
Vec2 action = {0.0, 0.0};
bool use_feedback = false;
bool use_pid = true;

void render_simulation() {
    // Simulate step
    if (use_feedback) {
        // Placeholder PID: Move toward dot
        Vec2 error = subtract(sim.dot_pos, sim.stage_pos);
        action = scale(error, 0.1);  // crude proportional term
    }
    sim.step(action);

    // Draw visualization background
    ImGui::Begin("Simulation Visualization");
    ImDrawList* draw_list = ImGui::GetWinowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(origin.x + 200, origin.y + 200);

    float scale = 100.0f;
    ImVec2 dot(center.x + sim.dot_pos[0]*scale, center.y - sim.dot_pos[1]*scale);
    ImVec2 stage(center.x + sim.stage_pos[0]*scale, center.y - sim.stage_pos[1]*scale);

    draw_list->AddCircle(dot, 5.0f, IM_COL32(255, 255, 0, 255));
    draw_list->AddCircle(stage, 7.0f, IM_COL32(0, 255, 0, 255), 0, 2.0f);
    ImGui::Dummy(ImVec2(400, 400));
    ImGui::End();

    // Side panel
    ImGui::Begin("Readout");
    ImGui::Text("Particle Location\n x: %.3f\n y: %.3f", sim.dot_pos[0], sim.dot_pos[1]);
    ImGui::Separator();
    ImGui::Text("Reward\n current: %.4f", sim.reward());
    ImGui::Separator();
    ImGui::Text("Desired Location\n x: 0\n y: 0");
    ImGui::Separator();
    if (ImGui::Button(use_pid ? "PID (on)" : "PPO (off)", ImVec2(200, 40))) {
        use_pid = !use_pid;
    }
    if (ImGui::Button(use_feedback ? "Disable Feedback" : "Enable Feedback", ImVec2(200, 40))) {
        use_feedback = !use_feedback;
    }
    ImGui::End();
}

void run_gui() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("DotLock GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render_simulation();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
