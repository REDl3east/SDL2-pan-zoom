#include "main.h"

/* Moving Rectangle */
int main(int argc, char *argv[]) {
  SDL_Event event;
  SDL_Rect r;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  SDL_Window *window = SDL_CreateWindow("SDL Pan/Zoom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 100, 100);
  SDL_Surface *surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
  auto *pixels = (Uint32 *)surface->pixels;

  int index = 0;
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (index % 2 == 0) {
        pixels[j * surface->w + i] = (0x00 << 24) | (0xFF << 16) | (0x00 << 8) | 0x00;
      } else {
        pixels[j * surface->w + i] = (0xdd << 24) | (0xFF << 16) | (0x11 << 8) | 0x00;
      }
      index++;
    }
  }
  SDL_UpdateTexture(texture, NULL, surface->pixels, surface->w * sizeof(Uint32));

  while (1) {

    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    SDL_Rect r = {50, 50, 500, 500};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
