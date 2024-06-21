#include "AbilitySystem/Data/LevelXPInfo.h"
#include "Interfaces/HASCombatInterface.h"

int32 ULevelXPInfo::FindLevelByXP(int32 XP)
{
	int32 Level = 1;
	bool bFind = false;
	while (!bFind)
	{
		if (XP >= LevelXPInformations[Level].RequirementXP)
		{
			++Level;
		}
		else bFind = true;
	}

	return Level;
}
