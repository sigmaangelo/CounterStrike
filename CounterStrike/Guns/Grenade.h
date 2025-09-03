// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/Gun.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API AGrenade : public AGun
{
	GENERATED_BODY()

	AGrenade();

public:

	void ThrowGrenade();


};
