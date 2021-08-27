#include "main.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class PixelWorld {
 public:
  PixelWorld(SDL_Renderer *renderer, int x, int y, int width, int height) {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    pixels = (Uint32 *)surface->pixels;

    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        pixels[j * surface->w + i] = (0xFF << 24) | (0xFF << 16) | (0xFF << 8) | 0x00;
      }
    }
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->w * sizeof(Uint32));
  }
  ~PixelWorld() {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
  }

  void point(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    pixels[y * surface->w + x] = (r << 24) | (g << 16) | (b << 8) | 0x00;
  }
  void update_position(int xrel, int yrel) {
    m_x += xrel;
    m_y += yrel;
  }

  void zoom_in(int mousex, int mousey) {
    if (scale > 1000000) return;

    int world_orginx = mousex - m_x;
    int world_orginy = mousey - m_y;

    // if (world_orginx < 0) return;
    // if (world_orginy < 0) return;
    // if (world_orginx >= m_width * scale) return;
    // if (world_orginy >= m_height * scale) return;

    int old_pointx = (float)world_orginx / scale;
    int old_pointy = (float)world_orginy / scale;

    scale *= 1.5;

    int new_pointx = (float)world_orginx / scale;
    int new_pointy = (float)world_orginy / scale;

    m_x -= (float)((float)(old_pointx - new_pointx) * scale);
    m_y -= (float)((float)(old_pointy - new_pointy) * scale);
  }
  void zoom_out(int mousex, int mousey) {
    if (scale < 0.05) return;
    
    int world_orginx = mousex - m_x;
    int world_orginy = mousey - m_y;

    // if (world_orginx < 0) return;
    // if (world_orginy < 0) return;
    // if (world_orginx >= m_width * scale) return;
    // if (world_orginy >= m_height * scale) return;

    int old_pointx = (float)world_orginx / scale;
    int old_pointy = (float)world_orginy / scale;

    scale /= 1.5;

    int new_pointx = (float)world_orginx / scale;
    int new_pointy = (float)world_orginy / scale;

    m_x -= (float)((float)(old_pointx - new_pointx) * scale);
    m_y -= (float)((float)(old_pointy - new_pointy) * scale);
  }

  void update_texture() {
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->w * sizeof(Uint32));
  }

  void render(SDL_Renderer *renderer) {
    SDL_Rect r = {m_x, m_y, (int)(m_width * scale), (int)(m_height * scale)};
    SDL_RenderCopy(renderer, texture, NULL, &r);
  }

 private:
  int m_x, m_y, m_width, m_height;
  SDL_Texture *texture;
  SDL_Surface *surface;
  Uint32 *pixels;
  double scale = 1;
};

int main(int argc, char *argv[]) {
  SDL_Event event;
  SDL_Rect r;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  SDL_Window *window = SDL_CreateWindow("SDL Pan/Zoom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

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

      if (quit) break;

      SDL_RenderClear(renderer);

      world.point(250, 250, 0xff, 0x00, 0x00);

      world.update_texture();
      world.render(renderer);
      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
