#include "AbilitySystem/Ability/HASAbility_Teleport.h"

FString UHASAbility_Teleport::GetAbilityDescription(int32 InAbilityLevel)
{
    float ManaCost = GetManaCost(InAbilityLevel);
    float Cooldown = GetCooldown(InAbilityLevel);


    return FString::Printf(TEXT(
        "<Title> Teleport </> \n\n"
        
        "<Level> Rank : %d / 5 </> \n\n"
        
        "Becomes lightning and moves to the target location.\n\n"
        
        "Cost : <Cost>%.2f</> \n"

        "Cooldown : <Cooldown>%.2f</>"
    ),
        InAbilityLevel,
        -ManaCost,
        Cooldown
    );
}
