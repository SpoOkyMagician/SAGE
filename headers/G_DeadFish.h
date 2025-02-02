/*
    geniedat - A library for reading and writing data files of genie
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

#ifndef GENIE_DEADFISH_H
#define GENIE_DEADFISH_H

/*
#include "G_ISerializable.h"
*/

namespace genie
{

namespace unit
{

/// Attributes for Unit of type >= DeadFish.
class DeadFish : public ISerializable
{
public:
  DeadFish();
  virtual ~DeadFish();
  virtual void setGameVersion(GameVersion gv);

  int16_t WalkingGraphic = -1;
  int16_t RunningGraphic = -1;
  float RotationSpeed = 0;
  uint8_t OldSizeClass = 0;
  int16_t TrackingUnit = -1;
  uint8_t TrackingUnitMode = 0;
  float TrackingUnitDensity = 0;
  uint8_t OldMoveAlgorithm = 0;

  // All can change while playing
  float TurnRadius = 0;
  float TurnRadiusSpeed = 3.402823466e+38f;
  float MaxYawPerSecondMoving = 3.402823466e+38f;
  float StationaryYawRevolutionTime = 0;
  float MaxYawPerSecondStationary = 3.402823466e+38f;
  float MinCollisionSizeMultiplier = 1.0f;

protected:
  virtual void serializeObject(void);
};

}

}

#endif // GENIE_DEADFISH_H
