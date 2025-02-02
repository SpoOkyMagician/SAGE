/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015 - 2021  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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
#include "G_ScnPlayerData.h"
#include "G_ScnFile.h"
*/

namespace genie
{

bool CombinedResources::playerInfo = false;

ScnMainPlayerData::ScnMainPlayerData() : playerNames(16)
{
}

ScnMainPlayerData::~ScnMainPlayerData()
{
  delete bitmap;
  delete bmpHeader;
}

void ScnMainPlayerData::serializeObject(void)
{
  serializePlayerDataVersion();
  if (scn_plr_data_ver > 1.13f)
  {
    for (unsigned int i=0; i<16; ++i)
      serialize(playerNames[i], 256); // 1.14 <-- this is read much later in AoE 1
    if (scn_plr_data_ver > 1.15f)
      serialize<int32_t>(playerNamesStringTable, 16);
    CombinedResources::playerInfo = true;
    serializeSub<CombinedResources>(resourcesPlusPlayerInfo, 16);
  }
  if (scn_plr_data_ver > 1.06f)
    serialize<uint8_t>(conquestVictory);
  serialize<ISerializable>(unknownData);
  serializeSizedString<uint16_t>(originalFileName, false);

  if (scn_plr_data_ver > 1.15f)
  {
    serialize<int32_t>(instructionsStringTable);
    serialize<int32_t>(hintsStringTable);
    serialize<int32_t>(victoryStringTable);
    serialize<int32_t>(lossStringTable);
    serialize<int32_t>(historyStringTable);

    if (scn_plr_data_ver > 1.21f)
      serialize<int32_t>(scoutsStringTable);
  }

  serializeSizedString<uint16_t>(instructions, false);
  if (scn_plr_data_ver > 1.1f)
  {
    serializeSizedString<uint16_t>(hints, false);
    serializeSizedString<uint16_t>(victory, false);
    serializeSizedString<uint16_t>(loss, false);
    serializeSizedString<uint16_t>(history, false);

    if (scn_plr_data_ver > 1.21f)
      serializeSizedString<uint16_t>(scouts, false);
  }

  if (scn_plr_data_ver < 1.03f)
  {
    serializeSizedString<uint16_t>(oldFilename1, false);
    serializeSizedString<uint16_t>(oldFilename2, false);
    serializeSizedString<uint16_t>(oldFilename3, false);
  }

  serializeSizedString<uint16_t>(pregameCinematicFilename, false);
  serializeSizedString<uint16_t>(victoryCinematicFilename, false);
  serializeSizedString<uint16_t>(lossCinematicFilename, false);
  if (scn_plr_data_ver > 1.08f)
    serializeSizedString<uint16_t>(backgroundFilename, false);
  if (scn_plr_data_ver > 1.0f)
    serializeBitmap();

  serializeSizedStrings<uint16_t>(aiNames, 16, false);
  serializeSizedStrings<uint16_t>(cityNames, 16, false);
  if (scn_plr_data_ver > 1.07f)
    serializeSizedStrings<uint16_t>(personalityNames, 16, false);
  serializeSub(aiFiles, 16);
  if (scn_plr_data_ver > 1.1f)
    serialize<uint8_t>(aiTypes, 16);
  if (scn_plr_data_ver > 1.01f)
    serialize<uint32_t>(separator_);
  // <- here actually switches the reading function in exe

  if (scn_plr_data_ver < 1.14f)
  {
    for (unsigned int i=0; i<16; ++i)
      serialize(playerNames[i], 256);
    serializeSub<CombinedResources>(resourcesPlusPlayerInfo, 16);
  }
  else
  {
    CombinedResources::playerInfo = false;
    serializeSub<CombinedResources>(resourcesPlusPlayerInfo, 16);
  }
  if (scn_plr_data_ver > 1.01f)
    serialize<uint32_t>(separator_);
  serialize<ISerializable>(victoryConditions);
  serialize<ISerializable>(diplomacy);
  if (scn_plr_data_ver > 1.01f)
    serialize<uint32_t>(separator_);
  serialize<uint32_t>(alliedVictory, scn_plr_data_ver < 1.02f ? 16*16 : 16);
  if (scn_plr_data_ver > 1.03f)
  {
    if (scn_plr_data_ver > 1.22f)
      serialize<uint32_t>(unused1);
    serialize<ISerializable>(disables);
    if (scn_plr_data_ver > 1.04f)
    {
      serialize<uint32_t>(unused1);
      if (scn_plr_data_ver > 1.11f)
      {
        serialize<uint32_t>(unused2);
        serialize<uint32_t>(allTechs);
      }
      if (scn_plr_data_ver > 1.05f)
        serialize<uint32_t>(startingAge, 16);
    }
  }
  if (scn_plr_data_ver > 1.01f)
    serialize<uint32_t>(separator_);
  if (scn_plr_data_ver > 1.18f)
  {
    serialize<int32_t>(player1CameraX);
    serialize<int32_t>(player1CameraY);
    if (scn_plr_data_ver > 1.2f)
    {
      serialize<int32_t>(aiType);
      if (scn_plr_data_ver > 1.23f)
        serialize<uint8_t>(aiTypes, 16);
    }
  }
  // <- here actually switches the reading function in exe
}

CombinedResources::CombinedResources()
{
}

CombinedResources::~CombinedResources()
{
}

void CombinedResources::serializeObject(void)
{
  if (playerInfo || scn_plr_data_ver < 1.14f)
    serialize<uint32_t>(state);
  if (!playerInfo || scn_plr_data_ver < 1.14f)
  {
    serialize<uint32_t>(gold);
    serialize<uint32_t>(wood);
    serialize<uint32_t>(food);
    serialize<uint32_t>(stone);
  }
  if (playerInfo || scn_plr_data_ver < 1.14f)
  {
    serialize<uint32_t>(type);
    serialize<uint32_t>(civilizationID);
    serialize<uint32_t>(unknown1);
  }
  if (!playerInfo && scn_plr_data_ver > 1.16f)
  {
    serialize<uint32_t>(ore);
    serialize<uint32_t>(goods);
    if (scn_plr_data_ver > 1.23f)
      serialize<uint32_t>(goods);
  }
}

UnknownData1::UnknownData1()
{
}

UnknownData1::~UnknownData1()
{
}

void UnknownData1::serializeObject(void)
{
  serialize<int16_t>(unknownCount);
  serialize<int16_t>(unknown2);
  serialize<float>(unknown3);

  /*/ 48 bytes? Lots of data if count is over 0
  ReadData((HANDLE)_hScenFile, hUnknown, 4u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 4, 1u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 6, 2u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 8, 1u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 12, 4u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 16, 4u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 20, 4u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 24, 2u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 32, 2u);
  ReadData((HANDLE)_hScenFile, &hScenFile, 2u);
  ReadData((HANDLE)_hScenFile, (char *)hUnknown + 40, 2u);
  ReadData((HANDLE)_hScenFile, &v15, 2u);*/
}

void ScnMainPlayerData::serializeBitmap(void)
{
  serialize<uint32_t>(bitmapIncluded);

  serialize<uint32_t>(bitmapWidth);
  serialize<uint32_t>(bitmapHeigth);
  serialize<int16_t>(unknown1);

  if (bitmapIncluded == 0)
    return;

  serialize<char>(&bmpHeader, 0x28);
  if (isOperation(OP_READ))
  {
    bitmapByteSize = *reinterpret_cast<uint16_t *>(bmpHeader + 20);

    bitmap = new char[bitmapByteSize];

    if (bitmapByteSize > 0x28)
      for (unsigned int i=0; i < 0x28; ++i)
        bitmap[i] = bmpHeader[i];

    char *bitmapStart = (bitmap + 0x28);

    serialize<char>(&bitmapStart, bitmapByteSize - 0x28);
  }
  else if (isOperation(OP_WRITE))
  {
    serialize<char>(&bitmap, bitmapByteSize);
  }
}

AiFile::AiFile()
{
}

AiFile::~AiFile()
{
}

void AiFile::serializeObject(void)
{
  serializeSize<uint32_t>(aiFilenameSize, aiFilename, true);
  serializeSize<uint32_t>(cityFileSize, cityFilename, true);
  if (scn_plr_data_ver > 1.07f)
    serializeSize<uint32_t>(perFileSize, perFilename, true);

  // crap in exe, says these are >= 1.15
  serialize(aiFilename, aiFilenameSize);
  serialize(cityFilename, cityFileSize);
  if (scn_plr_data_ver > 1.07f)
    serialize(perFilename, perFileSize);
}

ScnVictory::ScnVictory()
{
}

ScnVictory::~ScnVictory()
{
}

void ScnVictory::serializeObject(void)
{
  {
    serialize<uint32_t>(conquestRequired);
    serialize<uint32_t>(unused1);
    serialize<uint32_t>(numRelicsRequired);
    serialize<uint32_t>(unused2);
    serialize<uint32_t>(exploredPerCentRequired);
    serialize<uint32_t>(unused3);
  }
  serialize<uint32_t>(allConditionsRequired);
  if (scn_plr_data_ver > 1.12f)
  {
    serialize<uint32_t>(victoryMode);
    serialize<uint32_t>(scoreRequired);
    serialize<uint32_t>(timeForTimedGame);
  }
}

ScnDiplomacy::ScnDiplomacy()
{
}

ScnDiplomacy::~ScnDiplomacy()
{
}

void ScnDiplomacy::serializeObject(void)
{
  serialize<uint32_t>(stances, 16, 16); // Diplomacy (16*16*4)
  serialize<uint32_t>(individualVictory, 16, 180); // Individual Victory (12*60)
/* Individual victory conditions were eventually reformed into triggers.
00 +12 Quantity
01 +16 Resource
02 +20 Set Object
03 +24 Next Object
04 +28 Object Constant
05 +32 Source Player
06 +36 Technology
07 +40 Timer
08 +44 Trigger
09 +48 Area X (FROM)
10 +52 Area Y (FROM)
11 +56 Area X (TO)
12 +60 Area Y (TO)
13 +64 Object Group
14 +68 Object Type
15 +72 AI Signal
*/
}

ScnDisables::ScnDisables()
{
}

ScnDisables::~ScnDisables()
{
}

void ScnDisables::serializeObject(void)
{
  if (scn_plr_data_ver > 1.17f)
    serialize<uint32_t>(numDisabledTechs, 16);
  serialize<uint32_t>(disabledTechs, 16, scn_plr_data_ver < 1.04f ? 20 : scn_plr_data_ver < 1.3f ? 30 : 60);
  if (scn_plr_data_ver > 1.17f)
  {
    serialize<uint32_t>(numDisabledUnits, 16);
    serialize<uint32_t>(disabledUnits, 16, scn_plr_data_ver < 1.3f ? 30 : 60);
    serialize<uint32_t>(numDisabledBuildings, 16);
    serialize<uint32_t>(disabledBuildings, 16, scn_plr_data_ver < 1.3f ? 20 : 60);
  }
}

ScnMorePlayerData::ScnMorePlayerData()
{
}

ScnMorePlayerData::~ScnMorePlayerData()
{
}

void ScnMorePlayerData::serializeObject(void)
{
  serializeForcedString<uint16_t>(playerName);
  serialize<float>(initCameraX);
  serialize<float>(initCameraY);
  serialize<int16_t>(initCameraX2);
  serialize<int16_t>(initCameraY2);
  serialize<uint8_t>(alliedVictory);
  serializeSize<int16_t>(playerCount_, diplomacy1.size());
  serialize<uint8_t>(diplomacy1, playerCount_);
  serialize<uint32_t>(diplomacy2, playerCount_);
  serialize<uint32_t>(playerColor);
  serialize<float>(unknown1);
  serializeSize<int16_t>(unknownCount_, unknown3.size() / 44);
  serialize<uint8_t>(unknown2, 8);
  serialize<uint8_t>(unknown3, unknownCount_ * 44);
  serialize<uint8_t>(unknown4, 7);
  serialize<int32_t>(unknown5);
}

}
