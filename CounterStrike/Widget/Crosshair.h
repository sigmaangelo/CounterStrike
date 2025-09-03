// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	


public:
	UFUNCTION(BlueprintCallable)
	void SpreadLine(bool bIsSpread, float DeltaSpeed);
	
	// -------------Variable ------------
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* LineDown;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* LineUp;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* LineRight;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* LineLeft;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	float CurrentA;
	// -------------Function ------------


	void ShowCrosshair(bool bIsShow);

};
