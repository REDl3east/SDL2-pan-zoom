#include "main.h"

#include <memory>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class PixelWorld {
 public:
  PixelWorld(std::shared_ptr<SDL_Renderer> renderer, int x, int y, int width, int height) {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    m_renderer = renderer;
    m_texture = SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    m_surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    m_pixels = (Uint32 *)m_surface->pixels;

    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        m_pixels[j * m_surface->w + i] = (0xFF << 24) | (0xFF << 16) | (0xFF << 8) | 0x00;
      }
    }
    SDL_UpdateTexture(m_texture, NULL, m_surface->pixels, m_surface->w * sizeof(Uint32));
  }
  ~PixelWorld() {
    SDL_FreeSurface(m_surface);
    SDL_DestroyTexture(m_texture);
  }

  void point(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    m_pixels[y * m_surface->w + x] = (r << 24) | (g << 16) | (b << 8) | 0x00;
  }
  void update_position(int xrel, int yrel) {
    m_x += xrel;
    m_y += yrel;
  }

  void zoom_in(int mousex, int mousey) {
    if (m_scale > 1000000) return;

    m_scale *= (1 + zoom_scale);
    m_x -= zoom_scale * (mousex - m_x);
    m_y -= zoom_scale * (mousey - m_y);
  }
  void zoom_out(int mousex, int mousey) {
    if (m_scale < 0.05) return;

    m_scale /= (1 + zoom_scale);
    m_x += (zoom_scale / (1 + zoom_scale)) * (mousex - m_x);
    m_y += (zoom_scale / (1 + zoom_scale)) * (mousey - m_y);
  }

  void render() {
    SDL_UpdateTexture(m_texture, NULL, m_surface->pixels, m_surface->w * sizeof(Uint32));
    SDL_Rect r = {m_x, m_y, (int)(m_width * m_scale), (int)(m_height * m_scale)};
    SDL_RenderCopy(m_renderer.get(), m_texture, NULL, &r);
  }

 private:
  int m_x, m_y, m_width, m_height;
  std::shared_ptr<SDL_Renderer> m_renderer;
  SDL_Texture *m_texture;
  SDL_Surface *m_surface;
  Uint32 *m_pixels;
  double m_scale = 1;
  static constexpr double zoom_scale = 1;
};

int main(int argc, char *argv[]) {
  SDL_Event event;
  SDL_Rect r;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  SDL_Window *window = SDL_CreateWindow("SDL Pan/Zoom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

  std::shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(window, -1, 0), [](SDL_Renderer *p) { SDL_DestroyRenderer(p); });

  PixelWorld world(renderer, 250, 100, 500, 1000);

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

      if (quit)
        break;

      SDL_RenderClear(renderer.get());

      world.point(250, 250, 0xff, 0x00, 0x00);
      world.render();

      SDL_RenderPresent(renderer.get());
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
