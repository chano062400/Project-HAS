#include "AbilitySystem/Ability/HASAbility_Teleport.h"

FString UHASAbility_Teleport::GetAbilityDescription(int32 InAbilityLevel)
{
    return FString::Printf(TEXT(
        "<Title> Teleport </> \n\n <Level> Rank : %d / 5 </> \n\n Becomes lightning and moves to the specified location."),
        InAbilityLevel);
}
