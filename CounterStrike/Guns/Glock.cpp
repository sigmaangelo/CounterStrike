// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/Glock.h"

#include "Animation/HandAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AGlock::AGlock()
{
	BulletREF->SetRelativeLocation(FVector(19, 0, 7));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> GlockMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPSPawn/Meshes/Glock_Rigged.Glock_Rigged'"));

	if (GlockMesh.Succeeded())
	{
		GunMeshComponent->SetSkeletalMesh(GlockMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> GlockShootMontage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/AM_Glock_Fire.AM_Glock_Fire'"));

	if (GlockShootMontage.Succeeded())
	{
		GunFireAnim = GlockShootMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> GlockReloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/AM_Glock_Reload.AM_Glock_Reload'"));

	if (GlockReloadMontage.Succeeded())
	{
		GunReloadAnim = GlockReloadMontage.Object;
	}

	MaxAmmo = 20;
	CurrentAmmo = 20;
	Damage = 30;
	GunType = EWeaponType::Secondary;
	GunName = EGunName::Glock;
	BasicSpread = 50;
	MaxSpread = 1000;
	RecoilValue = 0.1f;


}

void AGlock::BeginPlay()
{
	Super::BeginPlay();

}

void AGlock::Server_Fire()
{
	Super::Server_Fire();
	// 핸드 총쏘기 애니메이션 재생
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (CurrentAmmo >= 1 && Player->ShootingComponent->IsReloading == false)
		{
			auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
			Anim->PlayGlockFireAnimation();
		}
	}

}

void AGlock::SemiAutoFireAction()
{
	Super::SemiAutoFireAction();
}

void AGlock::Server_Reload()
{
	Super::Server_Reload();
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
		Anim->PlayGlockReloadAnimation();
	}
	
}
