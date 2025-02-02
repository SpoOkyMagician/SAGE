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

/*
#include "G_SlpFrame.h"

#include <iostream>
//Debug
#include <cassert>
#include <stdexcept>
#include <chrono>

#include "G_Color.h"
*/

namespace genie
{

    Logger& SlpFrame::log = Logger::getLogger("genie.SlpFrame");
    const char* CNT_SETS[] = { "CNT_LEFT", "CNT_SAME", "CNT_DIFF", "CNT_TRANSPARENT", "CNT_FEATHERING", "CNT_PLAYER", "CNT_SHIELD", "CNT_PC_OUTLINE", "CNT_SHADOW" };

    //------------------------------------------------------------------------------
    SlpFrame::SlpFrame()
    {
    }

    //------------------------------------------------------------------------------
    SlpFrame::~SlpFrame()
    {
    }

    void SlpFrame::findMaximumExtents(void)
    {
        full_hotspot_x_ = std::max<int32_t>(hotspot_x_, shadow_hotspot_x_);
        full_hotspot_y_ = std::max<int32_t>(hotspot_y_, shadow_hotspot_y_);

        int32_t opposite_hotspot_x = std::max<int32_t>(-hotspot_x_ + width_, -shadow_hotspot_x_ + shadow_width_);
        int32_t opposite_hotspot_y = std::max<int32_t>(-hotspot_y_ + height_, -shadow_hotspot_y_ + shadow_height_);

        full_width_ = static_cast<uint32_t>(full_hotspot_x_ + opposite_hotspot_x);
        full_height_ = static_cast<uint32_t>(full_hotspot_y_ + opposite_hotspot_y);

        offset_x_ = full_hotspot_x_ - hotspot_x_;
        offset_y_ = full_hotspot_y_ - hotspot_y_;
        shadow_offset_x_ = full_hotspot_x_ - shadow_hotspot_x_;
        shadow_offset_y_ = full_hotspot_y_ - shadow_hotspot_y_;
    }

    void SlpFrame::setSize(const uint32_t width, const uint32_t height)
    {
        width_ = width;
        height_ = height;
        if (is32bit())
        {
            img_data.bgra_channels.clear();
            img_data.bgra_channels.resize(width * height, 0);
        }
        else
        {
            img_data.pixel_indexes.clear();
            img_data.alpha_channel.clear();
            img_data.pixel_indexes.resize(width * height, 0);
            img_data.alpha_channel.resize(width * height, 0);
        }
    }

    void SlpFrame::enlarge(const uint32_t width, const uint32_t height, const int32_t offset_x, const int32_t offset_y)
    {
        if (width_ > width && height_ > height) return;

        if (is32bit())
        {
            //std::vector<uint32_t> new_bgra_channels(width * height, 0);
            //img_data.transparency_mask;
        }
        else
        {
            std::vector<uint8_t> new_pixel_indexes(width * height, 0);
            std::vector<uint8_t> new_alpha_channel(width * height, 0);
            // If offsets are negative, the old pixels are lost. Care to fix?
            for (size_t r = 0, row = offset_y; r < height_ && row < height; ++r, ++row)
            {
                for (size_t c = 0, col = offset_x; c < width_ && col < width; ++c, ++col)
                {
                    new_pixel_indexes[row * width + col] = img_data.pixel_indexes[r * width_ + c];
                    new_alpha_channel[row * width + col] = img_data.alpha_channel[r * width_ + c];
                }
            }
            img_data.pixel_indexes = new_pixel_indexes;
            img_data.alpha_channel = new_alpha_channel;
        }

        // You better not crop the frame.
        for (XY16& xy : img_data.shadow_mask)
        {
            xy.x += offset_x;
            xy.y += offset_y;
        }
        for (XY16& xy : img_data.shield_mask)
        {
            xy.x += offset_x;
            xy.y += offset_y;
        }
        for (XY16& xy : img_data.outline_pc_mask)
        {
            xy.x += offset_x;
            xy.y += offset_y;
        }
        for (Color8XY16& xy : img_data.player_color_mask)
        {
            xy.x += offset_x;
            xy.y += offset_y;
        }

        hotspot_x_ += offset_x;
        hotspot_y_ += offset_y;
        width_ = width;
        height_ = height;
    }

    void SlpFrame::enlargeForMerge(const SlpFrame& frame, int32_t& os_x, int32_t& os_y)
    {
        const int32_t hsdx = frame.hotspot_x_ - hotspot_x_, hsdy = frame.hotspot_y_ - hotspot_y_,
            hsrdx = int32_t(frame.getWidth()) - frame.hotspot_x_ - (int32_t(width_) - hotspot_x_),
            hsrdy = int32_t(frame.getHeight()) - frame.hotspot_y_ - (int32_t(height_) - hotspot_y_);
        uint32_t width = width_, height = height_;
        int32_t offset_x = 0, offset_y = 0;
        if (uint32_t(hsdx) < width_)
        {
            // Add space to left side.
            offset_x = hsdx;
            width += hsdx;
        }
        if (uint32_t(hsdy) < height_)
        {
            // Add space to top side.
            offset_y = hsdy;
            height += hsdy;
        }
        if (uint32_t(hsrdx) < width_)
        {
            // Add space to right side.
            width += hsrdx;
        }
        if (uint32_t(hsrdy) < height_)
        {
            // Add space to bottom side.
            height += hsrdy;
        }
        os_x = -hsdx + offset_x;
        os_y = -hsdy + offset_y;
        enlarge(width, height, offset_x, offset_y);
    }

    bool SlpFrame::is32bit(void) const
    {
        return (properties_ & 7) == 7;
    }

    void SlpFrame::serializeObject(void)
    {
    }

    void SlpFrame::setLoadParams(std::istream& istr)
    {
        setIStream(istr);
        setOperation(OP_READ);
    }

    void SlpFrame::buildSaveData(std::ostream& ostr, uint32_t& slp_offset, SlpSaveData& save_data)
    {
        setOStream(ostr);
        setOperation(OP_WRITE);
#ifndef NDEBUG
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
#endif

        assert(height_ < 4096);
        outline_table_offset_ = slp_offset;
        cmd_table_offset_ = slp_offset + 4 * height_;
        slp_offset = cmd_table_offset_ + 4 * height_;

        // Build integers from image data.
        save_data.left_edges.resize(height_);
        save_data.right_edges.resize(height_);
        save_data.cmd_offsets.resize(height_);
        save_data.commands.reserve(height_);
        uint32_t player_color_slot = 0;
        uint32_t shadow_slot = 0;
        uint32_t shield_slot = 0;
        uint32_t outline_pc_slot = 0;
        uint32_t transparent_slot = 0;
        // Ensure that all 8-bit masks get saved.
        for (XY16 const& pixel : img_data.outline_pc_mask)
            img_data.alpha_channel[pixel.y * width_ + pixel.x] = 255;
        for (XY16 const& pixel : img_data.shield_mask)
            img_data.alpha_channel[pixel.y * width_ + pixel.x] = 255;
        {
            std::vector<XY16> new_shadow_mask;
            new_shadow_mask.reserve(img_data.shadow_mask.size());
            for (XY16 const& pixel : img_data.shadow_mask)
            {
                size_t loc = pixel.y * width_ + pixel.x;
                if (img_data.alpha_channel[loc] == 0)
                {
                    new_shadow_mask.emplace_back(pixel);
                    img_data.alpha_channel[loc] = 255;
                }
            }
            img_data.shadow_mask = std::move(new_shadow_mask);
        }

        for (uint32_t row = 0; row < height_; ++row)
        {
            size_t line_size = save_data.commands.size();
            save_data.cmd_offsets[row] = slp_offset;
            // Count left edge
            save_data.left_edges[row] = 0;
            if (is32bit())
            {
                for (uint32_t col = 0; col < width_; ++col)
                {
                    if (img_data.bgra_channels[row * width_ + col] == 0)
                        ++save_data.left_edges[row];
                    else break;
                }
            }
            else
            {
                for (uint32_t col = 0; col < width_; ++col)
                {
                    if (img_data.alpha_channel[row * width_ + col] == 0)
                        ++save_data.left_edges[row];
                    else break;
                }
            }
            // Fully transparent row
            if (save_data.left_edges[row] == width_)
            {
                save_data.left_edges[row] = 0x8000;
                continue;
            }
            // Read colors and count right edge
            uint16_t color_index = 0x100;
            uint32_t bgra = 0;
            uint32_t pixel_set_size = 0;
            cnt_type count_type = CNT_LEFT;
            for (uint32_t col = save_data.left_edges[row]; col < width_; ++col)
            {
                ++pixel_set_size;
                uint16_t last_color = color_index;
                uint32_t last_bgra = bgra;
                cnt_type old_count = count_type;

                if (player_color_slot < img_data.player_color_mask.size())
                {
                    if (img_data.player_color_mask[player_color_slot].x == col
                        && img_data.player_color_mask[player_color_slot].y == row)
                    {
                        count_type = CNT_PLAYER;
                        ++player_color_slot;
                        goto COUNT_SWITCH;
                    }
                }
                if (outline_pc_slot < img_data.outline_pc_mask.size())
                {
                    if (img_data.outline_pc_mask[outline_pc_slot].x == col
                        && img_data.outline_pc_mask[outline_pc_slot].y == row)
                    {
                        count_type = CNT_PC_OUTLINE;
                        ++outline_pc_slot;
                        goto COUNT_SWITCH;
                    }
                }
                if (shield_slot < img_data.shield_mask.size())
                {
                    if (img_data.shield_mask[shield_slot].x == col
                        && img_data.shield_mask[shield_slot].y == row)
                    {
                        count_type = CNT_SHIELD;
                        ++shield_slot;
                        goto COUNT_SWITCH;
                    }
                }
                if (shadow_slot < img_data.shadow_mask.size())
                {
                    if (img_data.shadow_mask[shadow_slot].x == col
                        && img_data.shadow_mask[shadow_slot].y == row)
                    {
                        count_type = CNT_SHADOW;
                        ++shadow_slot;
                        goto COUNT_SWITCH;
                    }
                }
                if (is32bit())
                {
                    bgra = img_data.bgra_channels[row * width_ + col];
                    if (transparent_slot < img_data.transparency_mask.size())
                    {
                        if (img_data.transparency_mask[transparent_slot].x == col
                            && img_data.transparency_mask[transparent_slot].y == row)
                        {
                            count_type = CNT_FEATHERING;
                            ++transparent_slot;
                            goto COUNT_SWITCH;
                        }
                    }
                    count_type = last_bgra == bgra ? CNT_SAME : CNT_DIFF;
                }
                else
                {
                    if (img_data.alpha_channel[row * width_ + col] == 0)
                    {
                        count_type = CNT_TRANSPARENT;
                        goto COUNT_SWITCH;
                    }
                    color_index = img_data.pixel_indexes[row * width_ + col];
                    count_type = last_color == color_index ? CNT_SAME : CNT_DIFF;
                    goto KEEP_COLOR;
                }

            COUNT_SWITCH:
                color_index = 0x100;
            KEEP_COLOR:
                if (old_count != count_type)
                {
                    switch (old_count)
                    {
                    case CNT_LEFT:
                        break;
                    case CNT_DIFF:
                        if (count_type == CNT_SAME)
                        {
                            handleColors(CNT_DIFF, row, col - 1, pixel_set_size - 2, save_data.commands);
                            pixel_set_size = 2;
                        }
                        else
                        {
                            handleColors(CNT_DIFF, row, col, --pixel_set_size, save_data.commands);
                            pixel_set_size = 1;
                        }
                        break;
                    default:
                        handleColors(old_count, row, col, --pixel_set_size, save_data.commands);
                        pixel_set_size = 1;
                        break;
                    }
                }
            }
            // Handle last colors
            if (is32bit() ? bgra == 0 : count_type == CNT_TRANSPARENT)
            {
                save_data.right_edges[row] = pixel_set_size;
            }
            else
            {
                save_data.right_edges[row] = 0;
                handleColors(count_type, row, width_, pixel_set_size, save_data.commands);
            }
            // End of line
            save_data.commands.push_back(0x0F);
            line_size = save_data.commands.size() - line_size;
            slp_offset += static_cast<uint32_t>(line_size);
        }
#ifndef NDEBUG
        std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
        log.debug("Frame (%u bytes) encoding took [%u] milliseconds", slp_offset - outline_table_offset_, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
#endif
    }

    //------------------------------------------------------------------------------
    void SlpFrame::serializeHeader(void)
    {
        serialize<uint32_t>(cmd_table_offset_);
        serialize<uint32_t>(outline_table_offset_);
        serialize<uint32_t>(palette_offset_);

        // 0x00 = use default palette
        // 0x08 = only 1 pcs in TC, seems to be useless leftover from AoE 1, mostly containing player colors.
        // 0x10 = tree SLPs 147 and 152 in RoR have two shadows, mask and black pixels. Has pure black shadow? No
        // 0x18 = use default palette, 0x08 uses outline? No
        // 0x78 = has embedded palette at palette offset: 4 bytes tell RGB count, then 3 bytes per each RGB
        serialize<uint32_t>(properties_);

        serialize<uint32_t>(width_);
        serialize<uint32_t>(height_);

        serialize<int32_t>(hotspot_x_);
        serialize<int32_t>(hotspot_y_);

        /*#ifndef NDEBUG
          log.debug("Frame header [%u], [%u], [%u], [%u], [%u], [%u], [%d], [%d], ",
            cmd_table_offset_, outline_table_offset_, palette_offset_, properties_,
            width_, height_, hotspot_x, hotspot_y);
        #endif*/
    }

    //------------------------------------------------------------------------------
    void SlpFrame::serializeShadowHeader(void)
    {
        serialize<uint32_t>(shadow_cmd_table_offset_);
        serialize<uint32_t>(shadow_outline_table_offset_);
        serialize<uint32_t>(shadow_palette_offset_);

        // uint16_t properties & 0xFFFF
        // int8_t palette id >> 16 & 255
        // int8_t image type >> 24 & 255
        serialize<uint32_t>(shadow_properties_);

        serialize<uint32_t>(shadow_width_);
        serialize<uint32_t>(shadow_height_);
        serialize<int32_t>(shadow_hotspot_x_);
        serialize<int32_t>(shadow_hotspot_y_);
    }

    //------------------------------------------------------------------------------
    size_t SlpFrame::load(uint16_t properties)
    {
        std::istream& istr = *getIStream();
        programmed_decay = properties == 32;

        if (is32bit())
        {
            img_data.bgra_channels.resize(width_ * height_, 0);
        }
        else
        {
            img_data.pixel_indexes.resize(width_ * height_);
            img_data.alpha_channel.resize(width_ * height_, 0);
        }

        uint16_t integrity = 0;
        istr.seekg(slp_file_pos_ + std::streampos(outline_table_offset_));
        std::vector<uint16_t> left_edges(height_);
        std::vector<uint16_t> right_edges(height_);
        for (uint32_t row = 0; row < height_; ++row)
        {
            serialize<uint16_t>(left_edges[row]);
            serialize<uint16_t>(right_edges[row]);
            integrity |= left_edges[row];
        }

        istr.seekg(slp_file_pos_ + std::streampos(cmd_table_offset_));
        std::vector<uint32_t> cmd_offsets(height_);
        serialize<uint32_t>(cmd_offsets, height_);

        // Read embedded palette
        if (properties_ == 0x78)
        {
            istr.seekg(slp_file_pos_ + std::streampos(palette_offset_));
            img_data.palette.resize(read<uint32_t>());
            for (Color& rgba : img_data.palette)
            {
                rgba.r = read<uint8_t>();
                rgba.g = read<uint8_t>();
                rgba.b = read<uint8_t>();
            }
        }

        if (integrity != 0x8000) // At least one visible row.
            // Each row has its commands, 0x0F signals the end of a rows commands.
            for (uint32_t row = 0; row < height_; ++row)
            {
                istr.seekg(slp_file_pos_ + std::streampos(cmd_offsets[row]));
                assert(!istr.eof());
                // Transparent rows apparently read one byte anyway. NO THEY DO NOT! Ignore and use seekg()
                if (0x8000 == left_edges[row] || 0x8000 == right_edges[row]) // Remember signedness!
                {
                    continue; // Pretend it does not exist.
                }
                uint32_t pix_pos = left_edges[row]; //pos where to start putting pixels

                uint8_t data = 0;
                while (true)
                {
                    data = read<uint8_t>();

                    if (data == 0x0F) break;

                    uint8_t cmd = data & 0x0F;
                    uint8_t sub = data & 0xF0;

                    uint32_t pix_cnt = 0;

                    switch (cmd) //0x00
                    {
                    case 0x0: // Lesser block copy
                    case 0x4:
                    case 0x8:
                    case 0xC:
                        pix_cnt = (data & 0xFC) >> 2;
                        if (is32bit())
                            readPixelsToImage32(row, pix_pos, pix_cnt);
                        else
                            readPixelsToImage(row, pix_pos, pix_cnt);
                        break;

                    case 0x1: // Lesser skip (making pixels transparent)
                    case 0x5:
                    case 0x9:
                    case 0xD:
                        pix_cnt = (data & 0xFC) >> 2;
                        pix_pos += pix_cnt;
                        break;

                    case 0x2: // Greater block copy
                        pix_cnt = (sub << 4) + read<uint8_t>();
                        if (is32bit())
                            readPixelsToImage32(row, pix_pos, pix_cnt);
                        else
                            readPixelsToImage(row, pix_pos, pix_cnt);
                        break;

                    case 0x3: // Greater skip
                        pix_cnt = (sub << 4) + read<uint8_t>();
                        pix_pos += pix_cnt;
                        break;

                    case 0x6: // Copy and transform (player color)
                        pix_cnt = getPixelCountFromData(data);
                        if (is32bit())
                            readPixelsToImage32(row, pix_pos, pix_cnt, 1);
                        else
                            readPixelsToImage(row, pix_pos, pix_cnt, true);
                        break;

                    case 0x7: // Run of plain color
                        pix_cnt = getPixelCountFromData(data);
                        if (is32bit())
                            setPixelsToColor32(row, pix_pos, pix_cnt);
                        else
                            setPixelsToColor(row, pix_pos, pix_cnt);
                        break;

                    case 0xA: // Transform block (player color)
                        pix_cnt = getPixelCountFromData(data);
                        if (is32bit())
                            setPixelsToColor32(row, pix_pos, pix_cnt, true);
                        else
                            setPixelsToColor(row, pix_pos, pix_cnt, true);
                        break;

                    case 0xB: // Shadow pixels
                        pix_cnt = getPixelCountFromData(data);
                        setPixelsToShadow(row, pix_pos, pix_cnt);
                        break;

                    case 0xE: // Extended commands
                        switch (data)
                        {
                        case 0x0E: // Forward draw
                        case 0x1E: // Reverse draw
                            log.error("Cmd [%X] is obsolete", data);
                            return purge();

                        case 0x2E: // Normal transform
                        case 0x3E: // Alternative transform
                            log.error("Cmd [%X] is obsolete", data);
                            return purge();

                        case 0x4E:
                            setPixelsToPcOutline(row, pix_pos, 1);
                            break;
                        case 0x6E:
                            setPixelsToShield(row, pix_pos, 1);
                            break;

                        case 0x5E:
                            pix_cnt = read<uint8_t>();
                            setPixelsToPcOutline(row, pix_pos, pix_cnt);
                            break;
                        case 0x7E:
                            pix_cnt = read<uint8_t>();
                            setPixelsToShield(row, pix_pos, pix_cnt);
                            break;

                        case 0x8E: // Dither
                            log.error("Cmd [%X] not implemented", data);
                            return purge();

                        case 0x9E: // Premultiplied alpha
                        case 0xAE: // Original alpha
                            pix_cnt = read<uint8_t>();
                            if (is32bit())
                            {
                                readPixelsToImage32(row, pix_pos, pix_cnt, 2);
                                break;
                            }

                        default:
                            log.error("Cmd [%X] is unknown", data);
                            return purge();
                        }
                        break;

                    default:
                        log.error("Unknown cmd [%X]", data);
                        std::cerr << "SlpFrame: Unknown cmd at " << std::hex << std::endl;
                        return purge();
                    }
                }
            }

        full_width_ = width_;
        full_height_ = height_;
        full_hotspot_x_ = hotspot_x_;
        full_hotspot_y_ = hotspot_y_;

        size_t pixel_memory = img_data.pixel_indexes.capacity() * sizeof(uint8_t);
        size_t alpha_memory = img_data.alpha_channel.capacity() * sizeof(uint8_t);
        size_t bgra_memory = img_data.bgra_channels.capacity() * sizeof(uint32_t);

        size_t shadow_memory = img_data.shadow_mask.capacity() * sizeof(XY16);
        size_t shield_memory = img_data.shield_mask.capacity() * sizeof(XY16);
        size_t outline_memory = img_data.outline_pc_mask.capacity() * sizeof(XY16);
        size_t transparency_memory = img_data.transparency_mask.capacity() * sizeof(XY16);

        size_t player_memory = img_data.player_color_mask.capacity() * sizeof(Color8XY16);
        size_t special_memory = img_data.special_shadow_mask.capacity() * sizeof(Color8XY16);
        size_t palette_memory = img_data.palette.capacity() * sizeof(genie::Color);

        return sizeof(SlpFrame) + pixel_memory + alpha_memory + bgra_memory +
            shadow_memory + shield_memory + outline_memory + transparency_memory +
            player_memory + special_memory + palette_memory;
    }

    //------------------------------------------------------------------------------
    size_t SlpFrame::loadSpecialShadow(void)
    {
        uint16_t properties = shadow_properties_ & 0xFFFF;
        int8_t palette_id = shadow_properties_ >> 16 & 255;
        int8_t image_type = shadow_properties_ >> 24 & 255;

        assert(shadow_palette_offset_ == 0);

        std::istream& istr = *getIStream();

        uint16_t integrity = 0;
        istr.seekg(slp_file_pos_ + std::streampos(shadow_outline_table_offset_));
        std::vector<uint16_t> left_edges(shadow_height_);
        std::vector<uint16_t> right_edges(shadow_height_);
        for (uint32_t row = 0; row < shadow_height_; ++row)
        {
            serialize<uint16_t>(left_edges[row]);
            serialize<uint16_t>(right_edges[row]);
            integrity |= left_edges[row];
        }

        istr.seekg(slp_file_pos_ + std::streampos(shadow_cmd_table_offset_));
        std::vector<uint32_t> cmd_offsets(shadow_height_);
        serialize<uint32_t>(cmd_offsets, shadow_height_);

        // Should be no embedded palette
        assert(properties != 0x78);

        if (integrity != 0x8000) // At least one visible row.
            // Each row has its commands, 0x0F signals the end of a rows commands.
            for (uint32_t row = 0; row < shadow_height_; ++row)
            {
                istr.seekg(slp_file_pos_ + std::streampos(cmd_offsets[row]));
                assert(!istr.eof());
                if (0x8000 == left_edges[row] || 0x8000 == right_edges[row])
                {
                    continue; // Pretend it does not exist.
                }
                uint32_t pix_pos = left_edges[row]; //pos where to start putting pixels

                uint8_t data = 0;
                while (pix_pos < shadow_width_)
                {
                    data = read<uint8_t>();

                    if (data == 15) break;

                    uint8_t cmd = data & 15;
                    uint8_t sub = data & 240;
                    uint32_t pix_cnt = 0;

                    switch (cmd)
                    {
                    case 0x0: // Lesser block copy
                    case 0x4:
                    case 0x8:
                    case 0xC:
                        pix_cnt = data >> 2 & 63;
                        readPixelsToSpecialShadow(row, pix_pos, pix_cnt);
                        break;

                    case 0x1: // Lesser skip (making pixels transparent)
                    case 0x5:
                    case 0x9:
                    case 0xD:
                        pix_cnt = data >> 2 & 63;
                        pix_pos += pix_cnt;
                        break;

                    case 0x2: // Greater block copy
                        pix_cnt = (sub << 4) + read<uint8_t>();
                        readPixelsToSpecialShadow(row, pix_pos, pix_cnt);
                        break;

                    case 0x3: // Greater skip
                        pix_cnt = (sub << 4) + read<uint8_t>();
                        pix_pos += pix_cnt;
                        break;

                    case 0x7: // Run of plain color
                        pix_cnt = getPixelCountFromData(data);
                        setPixelsToSpecialShadow(row, pix_pos, pix_cnt);
                        break;

                    default:
                        log.error("Unknown special cmd [%X]", data);
                        std::cerr << "SlpFrame: Unknown special cmd at " << std::hex << std::endl;
                        std::vector<Color8XY16>().swap(img_data.special_shadow_mask);
                        return img_data.special_shadow_mask.capacity();
                    }
                }
            }

        findMaximumExtents();

        size_t pixel_memory = img_data.pixel_indexes.capacity() * sizeof(uint8_t);
        size_t shadow_memory = img_data.special_shadow_mask.capacity() * sizeof(Color8XY16);
        log.info("Shadow memory %zu", shadow_memory);
        return shadow_memory;
    }

    //------------------------------------------------------------------------------
    void SlpFrame::readPixelsToImage(uint32_t row, uint32_t& col,
        uint32_t count, bool player_col)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            if (programmed_decay)
            {
                read<uint8_t>();
            }
            uint8_t color_index = read<uint8_t>();
            assert(row * width_ + col < img_data.pixel_indexes.size());
            img_data.pixel_indexes[row * width_ + col] = color_index;
            img_data.alpha_channel[row * width_ + col] = 255;
            if (player_col)
            {
                img_data.player_color_mask.emplace_back(col, row, color_index);
            }
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::readPixelsToImage32(uint32_t row, uint32_t& col,
        uint32_t count, uint8_t special)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            uint32_t bgra = read<uint32_t>();
            assert(row * width_ + col < img_data.bgra_channels.size());
            img_data.bgra_channels[row * width_ + col] = bgra;
            if (special == 1)
            {
                img_data.player_color_mask.emplace_back(col, row, 0);
            }
            else if (special == 2)
            {
                img_data.transparency_mask.emplace_back(col, row);
            }
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::readPixelsToSpecialShadow(uint32_t row, uint32_t& col, uint32_t count)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            uint16_t color_index = read<uint8_t>() << 2;
            img_data.special_shadow_mask.emplace_back(col, row, 255 - color_index);
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToColor(uint32_t row, uint32_t& col, uint32_t count,
        bool player_col)
    {
        if (programmed_decay)
        {
            read<uint8_t>();
        }
        uint8_t color_index = read<uint8_t>();
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            assert(row * width_ + col < img_data.pixel_indexes.size());
            img_data.pixel_indexes[row * width_ + col] = color_index;
            img_data.alpha_channel[row * width_ + col] = 255;
            if (player_col)
            {
                img_data.player_color_mask.emplace_back(col, row, color_index);
            }
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToColor32(uint32_t row, uint32_t& col, uint32_t count,
        bool player_col)
    {
        uint32_t bgra = read<uint32_t>();
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            assert(row * width_ + col < img_data.bgra_channels.size());
            img_data.bgra_channels[row * width_ + col] = bgra;
            if (player_col)
            {
                img_data.player_color_mask.emplace_back(col, row, 0);
            }
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToShadow(uint32_t row, uint32_t& col, uint32_t count)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            img_data.shadow_mask.emplace_back(col, row);
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToSpecialShadow(uint32_t row, uint32_t& col, uint32_t count)
    {
        uint16_t color_index = read<uint8_t>() << 2;
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            img_data.special_shadow_mask.emplace_back(col, row, 255 - color_index);
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToShield(uint32_t row, uint32_t& col, uint32_t count)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            img_data.shield_mask.emplace_back(col, row);
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::setPixelsToPcOutline(uint32_t row, uint32_t& col, uint32_t count)
    {
        uint32_t to_pos = col + count;
        while (col < to_pos)
        {
            img_data.outline_pc_mask.emplace_back(col, row);
            ++col;
        }
    }

    //------------------------------------------------------------------------------
    uint8_t SlpFrame::getPixelCountFromData(uint8_t data)
    {
        uint8_t pix_cnt;

        data = (data & 0xF0) >> 4;

        if (data == 0)
            pix_cnt = read<uint8_t>();
        else
            pix_cnt = data;

        return pix_cnt;
    }

    //------------------------------------------------------------------------------
    size_t SlpFrame::purge(void)
    {
        assert(false);
        std::vector<uint8_t>().swap(img_data.pixel_indexes);
        std::vector<uint8_t>().swap(img_data.alpha_channel);
        std::vector<uint32_t>().swap(img_data.bgra_channels);
        std::vector<XY16>().swap(img_data.shadow_mask);
        std::vector<XY16>().swap(img_data.shield_mask);
        std::vector<XY16>().swap(img_data.outline_pc_mask);
        std::vector<XY16>().swap(img_data.transparency_mask);
        std::vector<Color8XY16>().swap(img_data.player_color_mask);
        std::vector<Color8XY16>().swap(img_data.special_shadow_mask);
        std::vector<genie::Color>().swap(img_data.palette);
        return sizeof(SlpFrame);
    }

    //------------------------------------------------------------------------------
    void SlpFrame::handleColors(cnt_type count_type, uint32_t row, uint32_t col,
        uint32_t count, std::vector<uint8_t>& commands)
    {
        if (count == 0) return;
        switch (count_type)
        {
        case CNT_TRANSPARENT:
            if (count > 0x3F) // Greater skip.
            {
                commands.push_back(0x3 | (count & 0xF00) >> 4);
                commands.push_back(count);
            }
            else // Lesser skip.
            {
                commands.push_back(0x1 | count << 2);
            }
            break;
        case CNT_SAME:
            handleSpecial(0x7, row, col, count, 1, commands);
            break;
        case CNT_DIFF:
            if (count > 0x3F) // Greater copy.
            {
                commands.push_back(0x2 | (count & 0xF00) >> 4);
                commands.push_back(count);
                pushPixelsToBuffer(row, col, count, commands);
            }
            else // Lesser copy.
            {
                commands.push_back(count << 2);
                pushPixelsToBuffer(row, col, count, commands);
            }
            break;
        case CNT_FEATHERING:
            commands.push_back(0x9E);
            commands.push_back(count);
            pushPixelsToBuffer(row, col, count, commands);
            break;
        case CNT_PLAYER:
            handleSpecial(0x6, row, col, count, count, commands);
            break;
        case CNT_SHIELD:
            if (count == 1)
            {
                commands.push_back(0x6E);
            }
            else
            {
                commands.push_back(0x7E);
                commands.push_back(count);
            }
            break;
        case CNT_PC_OUTLINE:
            if (count == 1)
            {
                commands.push_back(0x4E);
            }
            else
            {
                commands.push_back(0x5E);
                commands.push_back(count);
            }
            break;
        case CNT_SHADOW:
            handleSpecial(0xB, row, col, count, 0, commands);
            break;
        default: break;
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::handleSpecial(uint8_t cmd, uint32_t row, uint32_t col,
        uint32_t count, uint32_t pixs, std::vector<uint8_t>& commands)
    {
        while (count > 0xFF)
        {
            count -= 0xFF;
            commands.push_back(cmd);
            commands.push_back(0xFF);
            pushPixelsToBuffer(row, col, pixs, commands);
        }
        if (count > 0xF)
        {
            commands.push_back(cmd);
            commands.push_back(count);
            pushPixelsToBuffer(row, col, pixs, commands);
        }
        else
        {
            commands.push_back(cmd | count << 4);
            pushPixelsToBuffer(row, col, pixs, commands);
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::pushPixelsToBuffer(uint32_t row, uint32_t col, uint32_t count,
        std::vector<uint8_t>& commands)
    {
        if (is32bit())
        {
            for (uint32_t pix = col - count; pix < col; ++pix)
            {
                uint32_t bgra = img_data.bgra_channels[row * width_ + pix];
                commands.push_back(bgra);
                commands.push_back(bgra >> 8);
                commands.push_back(bgra >> 16);
                commands.push_back(bgra >> 24);
            }
        }
        else
        {
            for (uint32_t pix = col - count; pix < col; ++pix)
            {
                commands.push_back(img_data.pixel_indexes[row * width_ + pix]);
            }
        }
    }

    //------------------------------------------------------------------------------
    void SlpFrame::save(SlpSaveData& save_data)
    {
#ifndef NDEBUG
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
#endif

        //Write edges
        for (uint32_t row = 0; row < height_; ++row)
        {
            serialize<uint16_t>(save_data.left_edges[row]);
            serialize<uint16_t>(save_data.right_edges[row]);
        }

        //Write cmd offsets
        serialize<uint32_t>(save_data.cmd_offsets, height_);

        for (uint8_t command : save_data.commands)
            serialize<uint8_t>(command);

#ifndef NDEBUG
        std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
        log.debug("SLP frame data saving took [%u] milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
#endif
    }

    //------------------------------------------------------------------------------
    SlpFramePtr SlpFrame::mirrorX(void)
    {
        SlpFramePtr mirrored(new genie::SlpFrame());

        mirrored->properties_ = properties_;
        mirrored->width_ = width_;
        uint32_t swapper = width_ - 1;
        mirrored->height_ = height_;
        mirrored->hotspot_x_ = swapper - hotspot_x_;
        mirrored->hotspot_y_ = hotspot_y_;

        genie::SlpFrameData& new_data = mirrored->img_data;
        new_data.bgra_channels.resize(img_data.bgra_channels.size());
        new_data.pixel_indexes.resize(img_data.pixel_indexes.size());
        new_data.alpha_channel.resize(img_data.alpha_channel.size());

        for (uint32_t row = 0; row < height_; ++row)
        {
            for (uint32_t c1 = 0, c2 = width_; c2-- > 0; ++c1)
            {
                if (is32bit())
                {
                    new_data.bgra_channels[row * width_ + c1] = img_data.bgra_channels[row * width_ + c2];
                }
                else
                {
                    new_data.pixel_indexes[row * width_ + c1] = img_data.pixel_indexes[row * width_ + c2];
                    new_data.alpha_channel[row * width_ + c1] = img_data.alpha_channel[row * width_ + c2];
                }
            }
        }

        // Let std::set sort the pixels for us.
        std::set<XY16> new_shadow_mask;
        for (XY16 pixel : img_data.shadow_mask)
        {
            pixel.x = swapper - pixel.x;
            new_shadow_mask.emplace(pixel);
        }
        new_data.shadow_mask = std::vector<XY16>(new_shadow_mask.begin(), new_shadow_mask.end());

        std::set<XY16> new_shield_mask;
        for (XY16 pixel : img_data.shield_mask)
        {
            pixel.x = swapper - pixel.x;
            new_shield_mask.emplace(pixel);
        }
        new_data.shield_mask = std::vector<XY16>(new_shield_mask.begin(), new_shield_mask.end());

        std::set<XY16> new_outline_pc_mask;
        for (XY16 pixel : img_data.outline_pc_mask)
        {
            pixel.x = swapper - pixel.x;
            new_outline_pc_mask.emplace(pixel);
        }
        new_data.outline_pc_mask = std::vector<XY16>(new_outline_pc_mask.begin(), new_outline_pc_mask.end());

        std::set<XY16> new_transparency_mask;
        for (XY16 pixel : img_data.transparency_mask)
        {
            pixel.x = swapper - pixel.x;
            new_transparency_mask.emplace(pixel);
        }
        new_data.transparency_mask = std::vector<XY16>(new_transparency_mask.begin(), new_transparency_mask.end());

        std::set<Color8XY16> new_player_color_mask;
        for (Color8XY16 pixel : img_data.player_color_mask)
        {
            pixel.x = swapper - pixel.x;
            new_player_color_mask.emplace(pixel);
        }
        new_data.player_color_mask = std::vector<Color8XY16>(new_player_color_mask.begin(), new_player_color_mask.end());

        return mirrored;
    }

}
