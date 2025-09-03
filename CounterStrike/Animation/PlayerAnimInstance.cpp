// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Component/PlayerStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		MovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		const FVector Velocity = PlayerCharacter->GetVelocity();
		const FVector RightVector = PlayerCharacter->GetActorRightVector();
		Direction = FVector::DotProduct(RightVector, Velocity);

		const FVector ForwardVector = PlayerCharacter->GetActorForwardVector();
		Speed = FVector::DotProduct(ForwardVector, Velocity);

		// 지면 속도가 작은 임계값 이상인 경우(속도가 엄청나게 작은 경우 애니메이션이 트리거되지 않도록 방지)와 현재 가속(입력)이 적용된 경우에만 이동
		const float Threshold = 3.0f;
		bShouldMove = (Speed > Threshold && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

		bIsFalling = MovementComponent->IsFalling();
		bIsCrouch = MovementComponent->IsCrouching();

	}
}

void UPlayerAnimInstance::PlayFireMontage()
{
	if (FireMontage)
	{
		Montage_Play(FireMontage);
	}
}

void UPlayerAnimInstance::PlayReloadMontage()
{
	if (ReloadMontage)
	{
		Montage_Play(ReloadMontage);
	}
}

void UPlayerAnimInstance::PlayDeadMontage()
{
	if (DeadMontage)
	{
		Montage_Play(DeadMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_OnReloadEnd()
{
}

void UPlayerAnimInstance::AnimNotify_OnDeadEnd()
{
}
