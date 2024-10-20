// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HASAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class HAS_API AHASAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AHASAIController();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTree;
};
