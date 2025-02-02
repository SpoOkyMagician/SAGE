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
#include "G_UnitCommand.h"
*/

namespace genie
{

    //------------------------------------------------------------------------------
    Task::Task()
    {
    }

    //------------------------------------------------------------------------------
    Task::~Task()
    {
    }

    //------------------------------------------------------------------------------
    void Task::setGameVersion(GameVersion gv)
    {
        ISerializable::setGameVersion(gv);
    }

    //------------------------------------------------------------------------------
    void Task::serializeObject(void)
    {
        GameVersion gv = getGameVersion();

        serialize<int16_t>(TaskType);
        serialize<int16_t>(ID);
        serialize<uint8_t>(IsDefault);
        serialize<int16_t>(ActionType);
        serialize<int16_t>(ClassID);
        serialize<int16_t>(UnitID);
        serialize<int16_t>(TerrainID);
        serialize<int16_t>(ResourceIn);
        serialize<int16_t>(ResourceMultiplier);
        serialize<int16_t>(ResourceOut);
        serialize<int16_t>(UnusedResource);
        serialize<float>(WorkValue1);
        serialize<float>(WorkValue2);
        serialize<float>(WorkRange);
        serialize<uint8_t>(AutoSearchTargets);
        serialize<float>(SearchWaitTime);
        serialize<uint8_t>(EnableTargeting);
        serialize<uint8_t>(CombatLevelFlag);
        serialize<int16_t>(GatherType);
        serialize<int16_t>(WorkFlag2);
        serialize<uint8_t>(TargetDiplomacy);
        serialize<uint8_t>(CarryCheck);
        serialize<uint8_t>(PickForConstruction);
        serialize<int16_t>(MovingGraphicID);
        serialize<int16_t>(ProceedingGraphicID);
        serialize<int16_t>(WorkingGraphicID);
        serialize<int16_t>(CarryingGraphicID);
        serialize<int16_t>(ResourceGatheringSoundID);
        serialize<int16_t>(ResourceDepositSoundID);
        if (gv >= GV_C4 && gv <= GV_LatestDE2)
        {
            serialize<uint32_t>(WwiseResourceGatheringSoundID);
            serialize<uint32_t>(WwiseResourceDepositSoundID);
        }
    }

}
