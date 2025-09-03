// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/Gun.h"
#include "AK47.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API AAK47 : public AGun
{
	GENERATED_BODY()

public:
	AAK47();

	void BeginPlay() override;

	void Server_Fire() override;

	//void FullAutoFireAction() override;

	void Server_Reload() override;

};
