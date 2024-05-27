#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "HASCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS()
class HAS_API AHASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AHASCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StartVitalAttribute;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StartPrimrayAttribute;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StartSecondaryAttribute;
	
	virtual void InitializeStartAttributes();

	virtual void InitAbilityActorInfo();

	virtual void ApplyAttributes(TSubclassOf<UGameplayEffect> EffectClass, AActor* SourceObject);

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
