// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfoDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatChangedSignature, const FGameplayAttributeData&, NewValue);

/**
 * 
 */
UCLASS()
class HAS_API UAttributeMenuWidgetController : public UHASWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;

	virtual void BindCallBacks() override;

	UPROPERTY(BlueprintAssignable)
	FStatChangedSignature StatChanged;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfoDataAsset> AttributeInfo;
};
