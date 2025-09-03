// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void PlayFireMontage();
	void PlayReloadMontage();
	void PlayDeadMontage();

	UFUNCTION()
	void AnimNotify_OnReloadEnd();

	UFUNCTION()
	void AnimNotify_OnDeadEnd();

	// ¾É±â
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCrouch;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Player Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class APlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement Component", Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Value", Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Value", Meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Value", Meta = (AllowPrivateAccess = true))
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "Value", Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DeadMontage;

	

};
