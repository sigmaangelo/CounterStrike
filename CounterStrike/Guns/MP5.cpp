// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/MP5.h"

#include "Animation/HandAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AMP5::AMP5()
{
	BulletREF->SetRelativeLocation(FVector(40, 0, 11));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MP5Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPSPawn/Meshes/MP5_rigged.MP5_rigged'"));

	if (MP5Mesh.Succeeded())
	{
		GunMeshComponent->SetSkeletalMesh(MP5Mesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> Mp5ShootMontage(TEXT("/Script/Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/AM_AK_Fire.AM_AK_Fire'"));

	if (Mp5ShootMontage.Succeeded())
	{
		GunFireAnim = Mp5ShootMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> Mp5ReloadMontage(TEXT("/ Script / Engine.AnimMontage'/Game/Assets/FPSPawn/Animations/MP5_Reload_anim_Montage.MP5_Reload_anim_Montage'"));

	if (Mp5ReloadMontage.Succeeded())
	{
		GunReloadAnim = Mp5ReloadMontage.Object;
	}

	MaxAmmo = 30;
	CurrentAmmo = 30;
	Damage = 27;
	GunType = EWeaponType::Primary;
	GunName = EGunName::MP5;
	BasicSpread = 100;
	MaxSpread = 1500;
	RecoilValue = 0.2f;
	// MP5 750 RPM
	FireRate = 750;
}

void AMP5::BeginPlay()
{
	Super::BeginPlay();

}

void AMP5::Server_Fire()
{
	Super::Server_Fire();
	// 핸드 총쏘기 애니메이션 재생
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (CurrentAmmo >= 1 && Player->ShootingComponent->IsReloading == false)
		{
			auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
			Anim->PlayMP5FireAnimation();
		}
	}
}

void AMP5::Server_Reload()
{
	Super::Server_Reload();
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
		Anim->PlayMP5ReloadAnimation();
	}
}
