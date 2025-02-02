/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2018 - 2021  Mikko "Tapsa" P

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "I_Headers.h"

#ifndef GENIE_PALFILE_H
#define GENIE_PALFILE_H

/*
#include <string>
#include <vector>
#include <stdint.h>
#include <memory>

#include "G_IFile.h"
*/

namespace genie
{

/// Element for pixel vector. The vector stores position (x, y) of
/// a pixel whose color is determined externally.
struct XY
{
  XY(uint32_t x, uint32_t y) :x(x), y(y)
  {
  }
  uint32_t x;
  uint32_t y;
};

inline bool operator<(const XY &l, const XY &r)
{
  return l.y == r.y ? l.x < r.x : l.y < r.y;
}

/// Element for pixel vector. The vector stores position (x, y) of
/// a pixel whose color is determined externally.
struct XY16
{
  XY16(uint16_t x, uint16_t y) :x(x), y(y)
  {
  }
  uint16_t x;
  uint16_t y;
};

inline bool operator<(const XY16 &l, const XY16 &r)
{
  return l.y == r.y ? l.x < r.x : l.y < r.y;
}

/// Element for color vector. The vector stores position (x, y) of
/// a color pixel and the palette index for the color.
struct ColorXY
{
  ColorXY(uint32_t x, uint32_t y, uint32_t index) :
    x(x), y(y), index(index)
  {
  }
  uint32_t x;
  uint32_t y;
  uint32_t index;
};

inline bool operator<(const ColorXY &l, const ColorXY &r)
{
  return l.y == r.y ? l.x < r.x : l.y < r.y;
}

/// Element for color vector. The vector stores position (x, y) of
/// a color pixel and the palette index for the color.
struct ColorXY16
{
  ColorXY16(uint16_t x, uint16_t y, uint16_t index) :
    x(x), y(y), index(index)
  {
  }
  uint16_t x;
  uint16_t y;
  uint16_t index;
};

inline bool operator<(const ColorXY16 &l, const ColorXY16 &r)
{
  return l.y == r.y ? l.x < r.x : l.y < r.y;
}

/// Element for color vector. The vector stores position (x, y) of
/// a color pixel and the palette index for the color.
struct Color8XY16
{
  Color8XY16(uint16_t x, uint16_t y, uint8_t index) :
    x(x), y(y), index(index)
  {
  }
  uint16_t x;
  uint16_t y;
  uint8_t index;
};

inline bool operator<(const Color8XY16 &l, const Color8XY16 &r)
{
  return l.y == r.y ? l.x < r.x : l.y < r.y;
}

class Logger;
class Color;

//------------------------------------------------------------------------------
/// Class for parsing aoe color palettes.
//
class PalFile : public IFile
{

public:
  //----------------------------------------------------------------------------
  /// Constructor
  //
  PalFile();

  //----------------------------------------------------------------------------
  /// Destructor
  //
  virtual ~PalFile();

  //----------------------------------------------------------------------------
  /// Returns a color at given index.
  ///
  /// @param index index of color in palette
  /// @return color object
  //
  Color& operator[](uint16_t index);
  std::vector<Color> getColors(void) const;

  //----------------------------------------------------------------------------
  /// Number of colors stored in this palette.
  ///
  /// @return size
  //
  size_t size(void) const;

  //----------------------------------------------------------------------------
  /// TODO: Somethings wrong...
  //
  virtual size_t objectSize(void);
private:

  static Logger &log;

  std::vector<Color> colors_;

  uint32_t num_colors_ = 0;

  std::string type_;
  std::string unknown_;

  std::string getHeader() const;
  std::string getHeader2() const; //TODO Unknown

  virtual void serializeObject(void);

  // TODO: Not implemented yet

  //----------------------------------------------------------------------------
  /// Get number of characters of a number serialized as a string.
  //
  size_t numOfChars(uint8_t number);
};

typedef std::shared_ptr<PalFile> PalFilePtr;

}

#endif // GENIE_PALFILE_H
