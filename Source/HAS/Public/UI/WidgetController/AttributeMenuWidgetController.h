// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HASWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfoDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FHASAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatChangedSignature, const FGameplayTag&, AttributeTag, const FGameplayAttributeData&, NewValue);

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
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable)
	FStatChangedSignature StatChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfoDataAsset> AttributeInfo;
};
