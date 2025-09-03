// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Component/PlayerShootingComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Guns/AK47.h"
#include "Kismet/KismetMathLibrary.h"


void UHandAnimInstance::PlayAKFireAnimation()
{
	if (AKFireMontage)
	{
		Montage_Play(AKFireMontage);
	}
}

void UHandAnimInstance::PlayAKReloadAnimation()
{
	if (AKReloadMontage)
	{
		Montage_Play(AKReloadMontage);
	}
}

void UHandAnimInstance::PlayMP5FireAnimation()
{
	if (MP5FireMontage)
	{
		Montage_Play(MP5FireMontage);
	}
}

void UHandAnimInstance::PlayMP5ReloadAnimation()
{
	if (MP5ReloadMontage)
	{
		Montage_Play(MP5ReloadMontage);
	}
}

void UHandAnimInstance::PlayGlockFireAnimation()
{
	if (GlockFireMontage)
	{
		Montage_Play(GlockFireMontage);
	}
}

void UHandAnimInstance::PlayGlockReloadAnimation()
{
	if (GlockReloadMontage)
	{
		Montage_Play(GlockReloadMontage);
	}
}

void UHandAnimInstance::KnifeAttackAnimation()
{
	if (KnifeAttackMontage && KnifeAttackMontage2)
	{
		if(KnifeMotion)
		{
			Montage_Play(KnifeAttackMontage);
			KnifeMotion = false;
		}
		else
		{
			Montage_Play(KnifeAttackMontage2);
			KnifeMotion = true;
		}
	}
}

void UHandAnimInstance::PlayGrenadeThrowAnimation()
{
	if (GrenadeThrowMontage)
	{
		Montage_Play(GrenadeThrowMontage);
	}
}

void UHandAnimInstance::AnimNotify_OnReloadFinish()
{
	Player->ShootingComponent->ReloadFinish();
}

void UHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 오류코드
	//float VectorLength = UKismetMathLibrary::VSize(Player->GetVelocity());
	//IsInAir = Player->GetMovementComponent()->IsFalling();
	//IsInMoving = VectorLength > 0;
	if(Player && Player->ShootingComponent->CurrentGun)
	{
		WeaponSwitch = Player->ShootingComponent->WeaponEquipped;
		WeaponName = Player->ShootingComponent->CurrentGun->GunName;

	}

}
