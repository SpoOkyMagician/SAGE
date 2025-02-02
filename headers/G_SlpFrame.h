/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2021  Mikko "Tapsa" P

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

#ifndef GENIE_SLPFRAME_H
#define GENIE_SLPFRAME_H

/*
#include "G_ISerializable.h"
#include "G_Logger.h"

#include <istream>
#include <vector>
#include <set>
#include <stdint.h>

#include "G_PalFile.h"
*/

namespace genie
{

struct SlpFrameData
{
  std::vector<uint8_t> pixel_indexes;
  std::vector<uint8_t> alpha_channel;
  std::vector<uint32_t> bgra_channels;

  std::vector<XY16> shadow_mask;
  std::vector<XY16> shield_mask;
  std::vector<XY16> outline_pc_mask;
  std::vector<XY16> transparency_mask;

  std::vector<Color8XY16> player_color_mask;
  std::vector<Color8XY16> special_shadow_mask;
  std::vector<genie::Color> palette;
};

struct SlpSaveData
{
  std::vector<uint16_t> left_edges;
  std::vector<uint16_t> right_edges;
  std::vector<uint32_t> cmd_offsets;
  std::vector<uint8_t> commands;
};

//------------------------------------------------------------------------------
/// Class for reading a frame of a slp file. Once loaded the image can be
/// obtained as a pixel array. A pixel is stored as the index of a color
/// in a palette.
///
/// TODO: Check file corruption
/// TODO: One (or more??) mask for player color and outline (and shadow?)
//
class SlpFrame : protected ISerializable
{

public:
  //----------------------------------------------------------------------------
  /// Constructor
  ///
  SlpFrame();

  //----------------------------------------------------------------------------
  /// Destructor
  //
  virtual ~SlpFrame();

  //----------------------------------------------------------------------------
  /// Set slp file position inside the stream. It's necesarry for calculating
  /// file offsets.
  //
  inline void setSlpFilePos(std::streampos pos) { slp_file_pos_ = pos; }

  //----------------------------------------------------------------------------
  /// Loads header data. The headers of frames are stored after the header of
  /// the slp file.
  //
  void serializeHeader(void);

  //----------------------------------------------------------------------------
  /// Loads shadow header data. The headers of shadow frames are stored
  /// after the main frame data of the slp file.
  //
  void serializeShadowHeader(void);

  //----------------------------------------------------------------------------
  /// Set stream parameters for loading frame data.
  //
  void setLoadParams(std::istream &istr);

  //----------------------------------------------------------------------------
  /// Loads frame data and creates an image. Frame data is located after all
  /// frame headers of the slp file.
  //
  size_t load(uint16_t properties);

  //----------------------------------------------------------------------------
  /// Loads shadow frame data and creates an image. Shadow frame data is
  /// located after all shadow frame headers of the slp file.
  //
  size_t loadSpecialShadow(void);

  //----------------------------------------------------------------------------
  /// Builds frame data from an image.
  //
  void buildSaveData(std::ostream &ostr, uint32_t &slp_offset, SlpSaveData &save_data);

  //----------------------------------------------------------------------------
  /// Saves the frame data.
  //
  void save(SlpSaveData &save_data);

  /// Clears buffers.
  size_t purge(void);

  //----------------------------------------------------------------------------
  /// Get image's width.
  //
  inline uint32_t getWidth(void) const { return full_width_; }
  inline uint32_t getMainLayerWidth(void) const { return width_; }

  //----------------------------------------------------------------------------
  /// Get image's height.
  //
  inline uint32_t getHeight(void) const { return full_height_; }

    //----------------------------------------------------------------------------
  /// Get image's hotspot x.
  //
  inline int32_t getHotspotX(void) const { return full_hotspot_x_; }

  //----------------------------------------------------------------------------
  /// Get image's hotspot y.
  //
  inline int32_t getHotspotY(void) const { return full_hotspot_y_; }

  inline int32_t getMainLayerOffsetX(void) const { return offset_x_; }
  inline int32_t getMainLayerOffsetY(void) const { return offset_y_; }
  inline int32_t getShadowLayerOffsetX(void) const { return shadow_offset_x_; }
  inline int32_t getShadowLayerOffsetY(void) const { return shadow_offset_y_; }

  void findMaximumExtents(void);
  void setSize(const uint32_t width, const uint32_t height);
  void enlarge(const uint32_t width, const uint32_t height, const int32_t offset_x, const int32_t offset_y);
  void enlargeForMerge(const SlpFrame &frame, int32_t &os_x, int32_t &os_y);

  inline uint32_t getProperties(void) const { return properties_; }
  inline uint32_t getPaletteOffset(void) const { return palette_offset_; }
  bool is32bit(void) const;

  SlpFrameData img_data;

  std::shared_ptr<SlpFrame> mirrorX(void);

private:
  static Logger &log;

  std::streampos slp_file_pos_;
  bool programmed_decay;

  uint32_t cmd_table_offset_;
  uint32_t outline_table_offset_;
  uint32_t palette_offset_;
  uint32_t properties_;

  uint32_t width_;
  uint32_t height_;
  uint32_t full_width_ = 0;
  uint32_t full_height_ = 0;
  int32_t hotspot_x_;
  int32_t hotspot_y_;
  int32_t full_hotspot_x_ = 0;
  int32_t full_hotspot_y_ = 0;
  int32_t offset_x_ = 0;
  int32_t offset_y_ = 0;

  uint32_t shadow_cmd_table_offset_;
  uint32_t shadow_outline_table_offset_;
  uint32_t shadow_palette_offset_;
  uint32_t shadow_properties_;

  uint32_t shadow_width_;
  uint32_t shadow_height_;
  int32_t shadow_hotspot_x_;
  int32_t shadow_hotspot_y_;
  int32_t shadow_offset_x_ = 0;
  int32_t shadow_offset_y_ = 0;

  virtual void serializeObject(void);

  //----------------------------------------------------------------------------
  /// Reads pixel indexes from file and sets the pixels according to the
  /// colors from the palette.
  /// It is assumed that the stream pointer is at the start of the pixel array.
  ///
  /// @param row row to set pixels at
  /// @param col column to set pixels from
  /// @param count how many pixels should be read
  /// @param player_col if true, pixel will be written to player color image
  //
  void readPixelsToImage(uint32_t row, uint32_t &col, uint32_t count,
                         bool player_col = false);
  void readPixelsToImage32(uint32_t row, uint32_t &col, uint32_t count,
                         uint8_t special = 0);
  void readPixelsToSpecialShadow(uint32_t row, uint32_t &col, uint32_t count);

  //----------------------------------------------------------------------------
  /// Sets the next count of pixels to given color without reading from stream.
  ///
  /// @param row row to set pixels at
  /// @param col column to set pixels from
  /// @param count how many pixels should be set
  /// @param color color to set
  /// @param player_col if true, pixel will be written to player color image
  //
  void setPixelsToColor(uint32_t row, uint32_t &col, uint32_t count,
                        bool player_col = false);
  void setPixelsToColor32(uint32_t row, uint32_t &col, uint32_t count,
                        bool player_col = false);
  void setPixelsToSpecialShadow(uint32_t row, uint32_t &col, uint32_t count);

  //----------------------------------------------------------------------------
  /// Sets the next count of pixels to shadow without reading from stream.
  void setPixelsToShadow(uint32_t row, uint32_t &col, uint32_t count);
  void setPixelsToShield(uint32_t row, uint32_t &col, uint32_t count);
  void setPixelsToPcOutline(uint32_t row, uint32_t &col, uint32_t count);

  //----------------------------------------------------------------------------
  /// This method returns either the count stored in command byte or (if not
  /// stored in command) the value of the next byte.
  ///
  /// @param data command byte
  //
  uint8_t getPixelCountFromData(uint8_t data);

  enum cnt_type {CNT_LEFT, CNT_SAME, CNT_DIFF, CNT_TRANSPARENT, CNT_FEATHERING, CNT_PLAYER, CNT_SHIELD, CNT_PC_OUTLINE, CNT_SHADOW};
  void handleColors(cnt_type count_type, uint32_t row, uint32_t col, uint32_t count, std::vector<uint8_t> &commands);
  void handleSpecial(uint8_t cmd, uint32_t row, uint32_t col, uint32_t count, uint32_t pixs, std::vector<uint8_t> &commands);
  void pushPixelsToBuffer(uint32_t row, uint32_t col, uint32_t count, std::vector<uint8_t> &commands);
};

typedef std::shared_ptr<SlpFrame> SlpFramePtr;

}

#endif // GENIE_SLPFRAME_H
