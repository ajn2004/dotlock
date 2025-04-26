#include "../include/gui.hpp"
#include "../include/simulation.hpp"
#include "../include/feedback.hpp"
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;

MicroscopeSim sim;
PID pid_feedback;
Vec2 action = {0.0, 0.0};
Vec2 target = {0.0, 0.0};
bool use_feedback = false;
bool use_pid = true;

void paritcle_visualize(){
  // Draw visualization background
  ImGui::Begin("Simulation Visualization");
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImVec2 center = ImVec2(origin.x + 200, origin.y + 200);
  
  float scale = 100.0f;
  ImVec2 dot(center.x + sim.dot_pos[0]*scale, center.y - sim.dot_pos[1]*scale);

  draw_list->AddCircle(dot, 5.0f, IM_COL32(255, 255, 0, 255));
  ImGui::Dummy(ImVec2(400, 400));
  ImGui::End();
}

void main_side_panel(){
  // Side panel
  ImGui::Begin("Readout");
  ImGui::Text("Particle Location\n x: %.3f\n y: %.3f", sim.dot_pos[0], sim.dot_pos[1]);
  ImGui::Separator();
  ImGui::Text("Reward\n current: %.4f", sim.reward());
  ImGui::Separator();
  ImGui::Text("Desired Location");
  ImGui::InputDouble("x", &target[0], 2, 20, "%.3f");
  ImGui::InputDouble("y", &target[1], 2, 20, "%.3f");
  ImGui::Separator();
  if (ImGui::Button(use_pid ? "PID (on)" : "PPO (off)", ImVec2(200, 40))) {
    use_pid = !use_pid;
  }
  if (ImGui::Button(use_feedback ? "Disable Feedback" : "Enable Feedback", ImVec2(200, 40))) {
    use_feedback = !use_feedback;
  }
  ImGui::End();
}

void pid_control_panel(){
  ImGui::Begin("PID-Panel");
  ImGui::Text("Weight variables: ");
  ImGui::InputDouble("Kp", &pid_feedback.kp, 1, 10, "%.3f");
  ImGui::InputDouble("Ki", &pid_feedback.ki, 0.01f, 0.1f, "%.3f");
  ImGui::InputDouble("Kd", &pid_feedback.kd, 0.1f, 0.1f, "%.3f");
  ImGui::End();
}
void render_simulation() {
    // Simulate step
    if (use_feedback) {
        // Placeholder PID: Move toward dot
      if(use_pid){
	action = pid_feedback.compute(target, sim.dot_pos);
      }
      else{
	   action = basic_feedback(target, sim.dot_pos);
      }
    }
    sim.step(action);
    action = {0.0, 0.0};
    // Draw visualization background
    paritcle_visualize();

    // Side panel
    main_side_panel();

    // PID control
    if(use_pid){
      
    }
}

void run_gui() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("DotLock GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render_simulation();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
