/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2022  Mikko "Tapsa" P

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

#ifndef GENIE_RESEARCH_H
#define GENIE_RESEARCH_H

/*
#include "G_ISerializable.h"
#include "G_ResourceUsage.h"
*/

namespace genie
{

//TODO description. Just adding to vector in file works?
class Tech : public ISerializable
{
public:
  Tech();
  virtual ~Tech();
  virtual void setGameVersion(GameVersion gv);

  /// Size of RequiredTechs vector (4 in aoe/ror and 6 in games >= aok)
  unsigned short getRequiredTechsSize(void);

  /// IDs of researches that are required for this tech to appear.
  /// The size of the array is 4 in aoe/ror and 6 at games >= aok
  std::vector<int16_t> RequiredTechs;

  typedef ResourceUsage<int16_t, int16_t, uint8_t> ResearchResourceCost;

  /// Resource cost in a list of max 3
  std::vector<ResearchResourceCost> ResourceCosts;

  /// Holds the number of how much of the required technologies you need.
  int16_t RequiredTechCount = 0;

  /// ID of the civilization that gets this technologie. -1 if unused
  /// MinGameVersion: aok
  int16_t Civ = -1;

  /// Set to 1 if this tech is enabled in full tech mode or 0 if not.
  /// MinGameVersion: aok
  int16_t FullTechMode = 0;

  /// ID of the unit where this research is available to buy.
  int16_t ResearchLocation = -1;

  union
  {
    /// Index of the name in language.dll
    int32_t LanguageDLLName = 7000;
    int16_t LanguageDLLNameU16;
  };
  union
  {
    /// Index of the description in language.dll
    int32_t LanguageDLLDescription = 8000;
    int16_t LanguageDLLDescriptionU16;
  };

  /// Number of seconds it takes to research
  int16_t ResearchTime = 0;

  /// Holds the techage id that corresponds to this data
  int16_t EffectID = -1;

  /// Age tech or not: 0 for regular and 2 for age.
  int16_t Type = 0;

  /// ID of the research's icon in icon.slp (frame number - 1)
  int16_t IconID = -1;

  /// ID of the button
  uint8_t ButtonID = 0;

  /// The first number is 100 000 plus the Language FIle ID for the name/description.
  int32_t LanguageDLLHelp = 107000;
  /// The second number is 150 000 plus the Language File ID for TheSecondName.
  int32_t LanguageDLLTechTree = 150000;
  /// The third number has been -1 in every technology so far.
  int32_t HotKey = -1;

  /// Internal name
  std::string Name = "";

  /// Second internal name
  /// MinGameVersion: SWGB
  std::string Name2 = "";

  uint8_t Repeatable = false;

private:
  virtual void serializeObject(void);
};

}

#endif // GENIE_RESEARCH_H
