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
#include "G_Unit.h"
*/

namespace genie
{

    //------------------------------------------------------------------------------
    Unit::Unit() : ResourceStorages(3)
    {
    }

    //------------------------------------------------------------------------------
    Unit::~Unit()
    {
    }

    //------------------------------------------------------------------------------
    void Unit::setGameVersion(GameVersion gv)
    {
        ISerializable::setGameVersion(gv);

        updateGameVersion(DamageGraphics);
        DeadFish.setGameVersion(gv);
        Bird.setGameVersion(gv);
        Type50.setGameVersion(gv);
        Projectile.setGameVersion(gv);
        Creatable.setGameVersion(gv);
        Building.setGameVersion(gv);
    }

    //------------------------------------------------------------------------------
    void Unit::serializeObject(void)
    {
        GameVersion gv = getGameVersion();

        //Type 10+
        if (gv < GV_AoEB && isOperation(OP_WRITE)) Type /= 10;
        serialize<uint8_t>(Type); // 7 = 70 in AoE alphas etc
        if (gv < GV_AoEB && isOperation(OP_READ)) Type *= 10;

        int16_t name_len = 0;
        if (gv > GV_LatestTap && gv < GV_C2 || gv < GV_Tapsa || gv > GV_LatestDE2)
            serializeSize<int16_t>(name_len, Name);
        serialize<int16_t>(ID);
        if (gv <= GV_LatestDE2 && gv >= GV_C15)
        {
            serialize<int32_t>(LanguageDLLName);
            serialize<int32_t>(LanguageDLLCreation);
        }
        else
        {
            serialize<int16_t>(LanguageDLLNameU16);
            LanguageDLLName = LanguageDLLNameU16;
            if (gv >= GV_MATT)
            {
                serialize<int16_t>(LanguageDLLCreationU16);
                LanguageDLLCreation = LanguageDLLCreationU16;
            }
        }
        serialize<int16_t>(Class);
        serializePair<int16_t>(StandingGraphic, (gv >= GV_AoKB) ? false : true);
        serialize<int16_t>(DyingGraphic);
        serialize<int16_t>(UndeadGraphic);
        serialize<uint8_t>(UndeadMode);
        serialize<int16_t>(HitPoints);
        serialize<float>(LineOfSight);
        serialize<uint8_t>(GarrisonCapacity);
        serialize<float>(CollisionSize.x);
        serialize<float>(CollisionSize.y);
        serialize<float>(CollisionSize.z);
        serialize<int16_t>(TrainSound);
        if (gv >= GV_AoKE3)
            serialize<int16_t>(DamageSound);
        serialize<int16_t>(DeadUnitID);
        if (gv >= GV_T6 && gv <= GV_LatestTap || gv >= GV_C7 && gv <= GV_LatestDE2)
            serialize<int16_t>(BloodUnitID);
        serialize<uint8_t>(SortNumber);
        serialize<uint8_t>(CanBeBuiltOn);
        serialize<int16_t>(IconID);
        serialize<uint8_t>(HideInEditor);
        serialize<int16_t>(OldPortraitPict);
        serialize<uint8_t>(Enabled);

        if (gv >= GV_AoK) // 11.48
            serialize<uint8_t>(Disabled);

        if (gv >= GV_MIK)
            serializePair<int16_t>(PlacementSideTerrain);
        serializePair<int16_t>(PlacementTerrain); // Before AoE, this also contains side terrain.
        serializePair<float>(ClearanceSize);
        serialize<uint8_t>(HillMode);
        serialize<uint8_t>(FogVisibility);
        serialize<int16_t>(TerrainRestriction);
        serialize<uint8_t>(FlyMode);
        serialize<int16_t>(ResourceCapacity);
        serialize<float>(ResourceDecay);
        serialize<uint8_t>(BlastDefenseLevel);
        serialize<uint8_t>(CombatLevel);
        serialize<uint8_t>(InteractionMode);
        serialize<uint8_t>(MinimapMode);
        serialize<uint8_t>(InterfaceKind);
        serialize<float>(MultipleAttributeMode);
        serialize<uint8_t>(MinimapColor);
        if (gv >= GV_AoEB) // 7.04
        {
            serialize<int32_t>(LanguageDLLHelp);
            serialize<int32_t>(LanguageDLLHotKeyText);
            serialize<int32_t>(HotKey);
            serialize<uint8_t>(Recyclable);
            serialize<uint8_t>(EnableAutoGather);
            serialize<uint8_t>(CreateDoppelgangerOnDeath);
            serialize<uint8_t>(ResourceGatherGroup);

            if (gv >= GV_AoKE3) // 9.15
            {
                serialize<uint8_t>(OcclusionMode);
                if (gv >= GV_AoKA) // 9.53
                {
                    serialize<uint8_t>(ObstructionType);
                    serialize<uint8_t>(ObstructionClass); // 9.56
                    if (gv >= GV_TC) // 11.55
                    {
                        serialize<uint8_t>(Trait);
                        serialize<uint8_t>(Civilization);
                        serialize<int16_t>(Nothing);
                    }
                }
            }
            else if (gv >= GV_T7 && gv <= GV_LatestTap)
            {
                serialize<uint8_t>(ObstructionType);
                serialize<uint8_t>(ObstructionClass);
            }

            serialize<uint8_t>(SelectionEffect);
            serialize<uint8_t>(EditorSelectionColour);
            serialize<float>(OutlineSize.x);
            serialize<float>(OutlineSize.y);
            serialize<float>(OutlineSize.z);

            if (gv >= GV_CK && gv <= GV_LatestDE2)
            {
                // This data is for scenario triggers.
                int32_t data = -2;
                serialize<int32_t>(data);
                data = 0;
                serialize<int32_t>(data);
            }
        }

        serializeSub<ResourceStorage>(ResourceStorages, 3);

        uint8_t damage_graphic_count;
        serializeSize<uint8_t>(damage_graphic_count, DamageGraphics.size());
        serializeSub<unit::DamageGraphic>(DamageGraphics, damage_graphic_count);

        serialize<int16_t>(SelectionSound);
        serialize<int16_t>(DyingSound);
        if (gv >= GV_C4 && gv <= GV_LatestDE2)
        {
            serialize<uint32_t>(WwiseTrainSoundID);
            serialize<uint32_t>(WwiseDamageSoundID);
            serialize<uint32_t>(WwiseSelectionSoundID);
            serialize<uint32_t>(WwiseDyingSoundID);
        }
        serialize<uint8_t>(OldAttackReaction);
        serialize<uint8_t>(ConvertTerrain);

        if (gv > GV_LatestTap && gv < GV_C2 || gv < GV_Tapsa || gv > GV_LatestDE2)
        {
            serialize(Name, name_len);

            if (gv >= GV_SWGB)
            {
                serializeSize<int16_t>(name_len, Name2);
                serialize(Name2, name_len);

                serialize<int16_t>(Unitline);
                serialize<uint8_t>(MinTechLevel);
            }
        }
        else
        {
            serializeDebugString(Name);
        }

        if (gv >= GV_AoE) // 7.12
            serialize<int16_t>(CopyID);

        if (gv >= GV_AoKA) // 9.49
            serialize<int16_t>(BaseID);

        if (gv >= GV_T5 && gv <= GV_LatestTap)
            serialize<int16_t>(TelemetryID);

        if (Type == UT_AoeTrees)
            return;

        if (Type >= UT_Flag)
            serialize<float>(Speed);
        else
            return;

        if (Type >= UT_Dead_Fish)
            serialize<ISerializable>(DeadFish);

        if (Type >= UT_Bird)
            serialize<ISerializable>(Bird);

        if (Type >= UT_Combatant)
            serialize<ISerializable>(Type50);

        if (Type == UT_Projectile)
            serialize<ISerializable>(Projectile);

        if (Type >= UT_Creatable)
            serialize<ISerializable>(Creatable);

        if (Type == UT_Building)
            serialize<ISerializable>(Building);
    }

}
