//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SUPERTUX_VIDEO_TEXTURE_MANAGER_HPP
#define HEADER_SUPERTUX_VIDEO_TEXTURE_MANAGER_HPP

#include <config.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "video/glutil.hpp"

class Texture;
class GLTexture;
class Rect;

class TextureManager
{
public:
  TextureManager();
  ~TextureManager();

  Texture* get(const std::string& filename);
  Texture* get(const std::string& filename, const Rect& rect);

#ifdef HAVE_OPENGL
  void register_texture(GLTexture* texture);
  void remove_texture(GLTexture* texture);

  void save_textures();
  void reload_textures();
#endif

private:
  friend class Texture;
  void release(Texture* texture);

  typedef std::map<std::string, Texture*> ImageTextures;
  ImageTextures image_textures;

  Texture* create_image_texture(const std::string& filename, const Rect& rect);

  /** on failure a dummy texture is returned and no exception is thrown */
  Texture* create_image_texture(const std::string& filename);

  /** throw an exception on error */
  Texture* create_image_texture_raw(const std::string& filename);
  Texture* create_image_texture_raw(const std::string& filename, const Rect& rect);

  Texture* create_dummy_texture();
  
#ifdef HAVE_OPENGL
  typedef std::set<GLTexture*> Textures;
  Textures textures;

  struct SavedTexture
  {
    GLTexture* texture;
    GLint width;
    GLint height;
    char* pixels;
    GLint border;

    GLint min_filter;
    GLint mag_filter;
    GLint wrap_s;
    GLint wrap_t;
  };
  std::vector<SavedTexture> saved_textures;

  void save_texture(GLTexture* texture);
#endif
};

#endif

/* EOF */
