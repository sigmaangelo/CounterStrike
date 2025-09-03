// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/Gun.h"
#include "MP5.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API AMP5 : public AGun
{
	GENERATED_BODY()


public:
	AMP5();

	void BeginPlay() override;

	void Server_Fire() override;
	
	//void FullAutoFireAction() override;

	void Server_Reload() override;
};
