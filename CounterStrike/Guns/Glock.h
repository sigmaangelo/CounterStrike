// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/Gun.h"
#include "Glock.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API AGlock : public AGun
{
	GENERATED_BODY()

	AGlock();

	void BeginPlay() override;

	void Server_Fire() override;

	void SemiAutoFireAction() override;

	void Server_Reload() override;
};
