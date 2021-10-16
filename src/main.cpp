#include "main.h"

static constexpr int INITIAL_WINDOW_WIDTH = 1280;
static constexpr int INITIAL_WINDOW_HEIGHT = 720;
static constexpr int WORLD_WIDTH = 255;
static constexpr int WORLD_HEIGHT = 255;
static constexpr const char *APP_NAME = "SDL Pan/Zoom";

int main(int argc, char *argv[]) {
  SDL_Event event;
  SDL_Rect r;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

  std::shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(window, -1, 0), [](auto *p) { SDL_DestroyRenderer(p); });

  PixelWorld world(renderer, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, WORLD_WIDTH, WORLD_HEIGHT);

  for (int i = 0; i < world.width(); i++) {
    for (int j = 0; j < world.height(); j++) {
      world.point(i, j, i, j, j);
    }
  }

  bool quit = false;
  bool mouse_down = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_LEFT) {
          } else if (event.key.keysym.sym == SDLK_RIGHT) {
          }
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouse_down = true;
          }
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouse_down = false;
          }
          break;
        }
        case SDL_MOUSEMOTION: {
          if (mouse_down) {
            world.update_position(event.motion.xrel, event.motion.yrel);
          }
          break;
        }
        case SDL_MOUSEWHEEL: {
          int mousex, mousey;
          SDL_GetMouseState(&mousex, &mousey);
          if (event.wheel.y > 0) {  //up
            world.zoom_in(mousex, mousey);
          } else if (event.wheel.y < 0) {  //down
            world.zoom_out(mousex, mousey);
          }
          break;
        }
      }

      if (quit) break;

      SDL_RenderClear(renderer.get());

      world.render();

      SDL_RenderPresent(renderer.get());
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
