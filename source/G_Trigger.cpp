/*
    genieutils - A library for reading and writing data files of genie
               engine games.
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
#include "G_Trigger.h"

#include <fstream>
*/

namespace genie
{

Trigger::Trigger()
{
}

Trigger::~Trigger()
{
}

void Trigger::serializeObject(void)
{
  serialize<int32_t>(startingState);
  serialize<uint8_t>(looping);
  serialize<int32_t>(stringTableID);
  serialize<uint8_t>(isObjective);
  serialize<int32_t>(descriptionOrder);
  if (scn_trigger_ver > 1.5f)
    serialize<int32_t>(startingTime);
  serializeForcedString<int32_t>(description);
  serializeForcedString<int32_t>(name);
  serializeSize<int32_t>(numEffects_, effects.size());
  serializeSub<TriggerEffect>(effects, numEffects_);
  if (scn_trigger_ver > 1.2f)
    serialize<int32_t>(effectDisplayOrder, numEffects_);
  serializeSize<int32_t>(numConditions_, conditions.size());
  serializeSub<TriggerCondition>(conditions, numConditions_);
  if (scn_trigger_ver > 1.2f)
    serialize<int32_t>(conditionDisplayOrder, numConditions_);
}

TriggerCondition::TriggerCondition()
{
}

TriggerCondition::~TriggerCondition()
{
}

void TriggerCondition::serializeObject(void)
{
  serialize<int32_t>(type);
  if (scn_trigger_ver > 1.0f)
  {
    if (isOperation(OP_WRITE)) // Automatic compression.
    {
      usedVariables = 16;
      int32_t *browser = &aiSignal;
      while (usedVariables > 0)
      {
        if (*browser != -1) break;
        --usedVariables;
        --browser;
      }
    }
    serialize<int32_t>(usedVariables);
  }
  else usedVariables = 13;

  int32_t *start = &amount;
  serialize<int32_t>(&start, usedVariables);
}

TriggerEffect::TriggerEffect()
{
}

TriggerEffect::~TriggerEffect()
{
}

void TriggerEffect::serializeObject(void)
{
  serialize<int32_t>(type);
  if (scn_trigger_ver > 1.0f)
  {
    if (isOperation(OP_WRITE)) // Automatic compression.
    {
      usedVariables = 23;
      int32_t *browser = &instructionPanel;
      while (usedVariables > 0)
      {
        if (*browser != -1) break;
        --usedVariables;
        --browser;
      }
    }
    serialize<int32_t>(usedVariables);
  }
  else usedVariables = 16;

  int32_t *start = &aiGoal;
  serialize<int32_t>(&start, usedVariables);
  serializeForcedString<int32_t>(message);
  serializeForcedString<int32_t>(soundFile);
  if (scn_trigger_ver > 1.1f && usedVariables >= 5 && setObjects > 0)
    serialize<int32_t>(selectedUnits, setObjects);
}

}
