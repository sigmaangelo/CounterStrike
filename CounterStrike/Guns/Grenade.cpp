// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/Grenade.h"

#include "Animation/HandAnimInstance.h"
#include "Character/PlayerCharacter.h"

AGrenade::AGrenade()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> grenadeMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Grenade/Grenade.Grenade'"));

	if (grenadeMesh.Succeeded())
	{
		GunMeshComponent->SetSkeletalMesh(grenadeMesh.Object);

	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> grenadeThrow(TEXT("/Script/Engine.AnimMontage'/Game/Assets/Grenade/Animations/Grenade_Throw_Montage.Grenade_Throw_Montage'"));

	if (grenadeThrow.Succeeded())
	{
		GunFireAnim = grenadeThrow.Object;
	}
	
	ConstructorHelpers::FObjectFinder<USoundBase> grenadeSound(TEXT("/Script/Engine.SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));

	if (grenadeSound.Succeeded())
	{
		FireSound = grenadeSound.Object;
	}
	
}

void AGrenade::ThrowGrenade()
{

}
