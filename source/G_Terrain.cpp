/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2021  Mikko "Tapsa" P

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
#include <G_Terrian.h>
*/

namespace genie
{

    unsigned short Terrain::terrain_count_ = 0;;
    //------------------------------------------------------------------------------
    Terrain::Terrain() : ElevationGraphics(TILE_TYPE_COUNT),
        TerrainUnitID(TERRAIN_UNITS_SIZE), TerrainUnitDensity(TERRAIN_UNITS_SIZE),
        TerrainUnitMaskedDensity(TERRAIN_UNITS_SIZE),
        TerrainUnitCentering(TERRAIN_UNITS_SIZE)
    {
    }

    //------------------------------------------------------------------------------
    Terrain::~Terrain()
    {
    }

    void Terrain::setGameVersion(GameVersion gv)
    {
        ISerializable::setGameVersion(gv);

        if (gv != GV_CCV && gv != GV_CCV2 && gv != GV_TCV)
            Borders.resize(getTerrainCount(gv), 0);
    }

    //------------------------------------------------------------------------------
    void Terrain::setTerrainCount(unsigned short cnt)
    {
        terrain_count_ = cnt;
    }

    //------------------------------------------------------------------------------
    unsigned short Terrain::getTerrainCount(GameVersion gv)
    {
        if (terrain_count_)
            return terrain_count_;
        if (gv == GV_SWGB || gv == GV_CC)
            return 55;
        if (gv == GV_CCV || gv == GV_CCV2 || gv == GV_TCV)
            return 252;
        if (gv >= GV_T2 && gv <= GV_LatestTap)
            return 96;
        if (gv >= GV_C8 && gv <= GV_LatestDE2)
            return 200;
        if (gv >= GV_C2 && gv <= GV_C7)
            return 110;
        if (gv == GV_Cysion)
            return 100;
        if (gv == GV_TC)
            return 42;
        return 32;
    }

    //------------------------------------------------------------------------------
    unsigned short Terrain::getNameSize()
    {
        if (getGameVersion() >= GV_SWGB)
            return 17;
        else
            return 13;
    }

    //------------------------------------------------------------------------------
    void Terrain::serializeObject(void)
    {
        GameVersion gv = getGameVersion();

        serialize<uint8_t>(Enabled);
        serialize<uint8_t>(Random);

        if (gv > GV_LatestTap && gv < GV_C2 || gv < GV_Tapsa || gv > GV_LatestDE2)
        {
            serialize(Name, getNameSize());
            serialize(Name2, getNameSize());
        }
        else
        {
            if (gv >= GV_T2 && gv < GV_C2 || gv >= GV_C8 && gv <= GV_LatestDE2)
            {
                serialize<uint8_t>(IsWater);
                serialize<uint8_t>(HideInEditor);
                serialize<int32_t>(StringID);
            }
            if (gv >= GV_T2 && gv < GV_C2)
            {
                serialize<int16_t>(BlendPriorityS16);
                serialize<int16_t>(BlendTypeS16);

                BlendPriority = BlendPriorityS16;
                BlendType = BlendTypeS16;
            }
            serializeDebugString(Name);
            serializeDebugString(Name2);
        }

        if (gv >= GV_AoEB)
            serialize<int32_t>(SLP);
        serialize<int32_t>(ShapePtr);
        serialize<int32_t>(SoundID);
        if (gv <= GV_LatestDE2)
        {
            if (gv >= GV_C6)
            {
                if (gv >= GV_C11)
                {
                    serialize<uint32_t>(WwiseSoundID);
                }
                serialize<uint32_t>(WwiseSoundStopID);
            }
        }

        if (gv >= GV_AoKB)
        {
            serialize<int32_t>(BlendPriority);
            serialize<int32_t>(BlendType);

            if (gv >= GV_C3 && gv <= GV_LatestDE2)
            {
                serializeDebugString(OverlayMaskName);
            }
        }

        serialize<uint8_t>(Colors, 3);
        serializePair<uint8_t>(CliffColors);
        serialize<uint8_t>(PassableTerrain);
        serialize<uint8_t>(ImpassableTerrain);

        serialize<uint8_t>(IsAnimated);
        serialize<int16_t>(AnimationFrames);
        serialize<int16_t>(PauseFames);
        serialize<float>(Interval);
        serialize<float>(PauseBetweenLoops);
        serialize<int16_t>(Frame);
        serialize<int16_t>(DrawFrame);
        serialize<float>(AnimateLast);
        serialize<uint8_t>(FrameChanged);
        serialize<uint8_t>(Drawn);

        serializeSub<FrameData>(ElevationGraphics, TILE_TYPE_COUNT);
        serialize<int16_t>(TerrainToDraw);
        serializePair<int16_t>(TerrainDimensions);
        if (gv >= GV_C8 && gv <= GV_LatestDE2)
        {
            serialize<int16_t>(TerrainUnitMaskedDensity, TERRAIN_UNITS_SIZE);
        }
        else
        {
            if (isOperation(OP_READ))
            {
                switch (gv)
                {
                case GV_CCV:
                case GV_CCV2:
                    serialize<int16_t>(Borders, 55);
                    break;
                case GV_TCV:
                    serialize<int16_t>(Borders, 42);
                    break;
                default:
                    serialize<int16_t>(Borders, getTerrainCount(gv));
                    break;
                }
            }
            else
            {
                serialize<int16_t>(Borders, Borders.size());
            }
        }
        serialize<int16_t>(TerrainUnitID, TERRAIN_UNITS_SIZE);
        serialize<int16_t>(TerrainUnitDensity, TERRAIN_UNITS_SIZE);
        serialize<uint8_t>(TerrainUnitCentering, TERRAIN_UNITS_SIZE);
        serialize<int16_t>(NumberOfTerrainUnitsUsed);

        if (gv < GV_SWGB)
            serialize<int16_t>(Phantom);
    }

}
