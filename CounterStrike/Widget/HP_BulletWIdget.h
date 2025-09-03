// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HP_BulletWIdget.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UHP_BulletWIdget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// ------Variable-------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Bullet")
	class UTextBlock* CurrentBullet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Bullet")
	class UTextBlock* MaxBullet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Item")
	class UImage* AK47;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Item")
	class UImage* Gun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Item")
	class UImage* Knife;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Item")
	class UImage* Granade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "HP")
	class UTextBlock* HP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "HP")
	class UProgressBar* HPBar;
	UPROPERTY()
	class UImage* CurrentImage;
	float CurrentA = 1.0f;
	float CurrentHP = 100.0f;
	float CB = 30.0f;
	float MB = 30.0f;

	UPROPERTY()
	class APlayerCharacter* Player;
	// ------Variable-------

	// ------Function-------

	UFUNCTION(BlueprintCallable)
	void DamageHP(int32 Damage);
	UFUNCTION(BlueprintCallable)
	void UseBullet();
	UFUNCTION(BlueprintCallable)
	void ResetBullet();
	UFUNCTION(BlueprintCallable)
	void Magnification_AK47();
	UFUNCTION(BlueprintCallable)
	void Magnification_Gun();
	UFUNCTION(BlueprintCallable)
	void Magnification_Knife();
	UFUNCTION(BlueprintCallable)
	void Magnification_Granade();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void NativeConstruct()override;

	// ------Function-------

};
