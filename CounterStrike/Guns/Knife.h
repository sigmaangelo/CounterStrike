// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/Gun.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API AKnife : public AGun
{
	GENERATED_BODY()

	AKnife();
	

public:
	UPROPERTY(EditAnywhere)
	float KnifeRange = 200;

	UPROPERTY(EditAnywhere, Category = Effect)
	UMaterialInterface* KnifeDecal;

	UPROPERTY(EditAnywhere, Category = Effect)
	USoundBase* KnifeAirSound;

	UPROPERTY(EditAnywhere, Category = Effect)
	USoundBase* KnifeHitSound;

	UFUNCTION(Server, Reliable)
	void Server_KnifeAttackAction();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_KnifeAttackAction(const FHitResult& HitResult, FVector StartLoc, FVector EndLoc);
};
