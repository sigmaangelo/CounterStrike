// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/AK47.h"

#include "Animation/HandAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AAK47::AAK47()
{
	BulletREF->SetRelativeLocation(FVector(74, -1, 10));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> AK47Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPSPawn/Meshes/AK_rigged.AK_rigged'"));

	if(AK47Mesh.Succeeded())
	{
		GunMeshComponent->SetSkeletalMesh(AK47Mesh.Object);
	}


	ConstructorHelpers::FObjectFinder<UAnimMontage> Ak47ShootMontage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/AM_AK_Fire.AM_AK_Fire'"));

	if(Ak47ShootMontage.Succeeded())
	{
		GunFireAnim = Ak47ShootMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> Ak47ReloadMontage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/AM_AK_Reload.AM_AK_Reload'"));

	if (Ak47ShootMontage.Succeeded())
	{
		GunReloadAnim = Ak47ReloadMontage.Object;
	}

	MaxAmmo = 30;
	CurrentAmmo = 30;
	Damage = 36;
	GunType = EWeaponType::Primary;
	GunName = EGunName::AK47;
	BasicSpread = 150;
	MaxSpread = 2500;
	RecoilValue = 0.3f;
	// AK47 600 RPM
	FireRate = 600;
}

void AAK47::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAK47::Server_Fire()
{
	Super::Server_Fire();
	// 핸드 총쏘기 애니메이션 재생
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (CurrentAmmo >= 1 && Player->ShootingComponent->IsReloading == false)
		{
			auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
			Anim->PlayAKFireAnimation();
		}
	}
	
}

void AAK47::Server_Reload()
{
	Super::Server_Reload();
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
		Anim->PlayAKReloadAnimation();
	}
	
}

