#ifndef PIXELWORLD_H
#define PIXELWORLD_H

#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "lodepng.h"

class PixelWorld {
 public:
  PixelWorld(std::shared_ptr<SDL_Renderer> renderer, int window_width, int window_height, int width, int height);
  ~PixelWorld();

  void point(int x, int y, Uint8 r, Uint8 g, Uint8 b);
  void update_position(int xrel, int yrel);
  void zoom_in(int mousex, int mousey);
  void zoom_out(int mousex, int mousey);
  void render();

  int height() { return m_height; }
  int width() { return m_width; }

  bool save(const std::string &filename);

 protected:
  int m_x, m_y, m_width, m_height;
  std::shared_ptr<SDL_Renderer> m_renderer;
  SDL_Texture *m_texture;
  SDL_Surface *m_surface;
  Uint32 *m_pixels;
  double m_scale = 1;

 private:
  static constexpr double zoom_scale = 1;
};

#endif