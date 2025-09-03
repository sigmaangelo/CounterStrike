// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/PlayerShootingComponent.h"
#include "HandAnimInstance.generated.h"


enum class GunName : uint8;
/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	// 소유중인 폰 저장 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
		class APlayerCharacter* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
		bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
		bool IsInMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
		int WeaponSwitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAnimSettings")
		EGunName WeaponName;


	//-----------------------------AK47---------------------------------------
	// 총쏘기에서 사용할 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* AKFireMontage;
	// 재장전 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* AKReloadMontage;
	// 총쏘기 재생함수
	void PlayAKFireAnimation();
	void PlayAKReloadAnimation();

	//-----------------------------MP5-----------------------------------------
	// 총쏘기에서 사용할 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* MP5FireMontage;
	// 재장전 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* MP5ReloadMontage;
	// 총쏘기 재생함수
	void PlayMP5FireAnimation();
	void PlayMP5ReloadAnimation();

	//-----------------------------Glock-----------------------------------------
	// 총쏘기에서 사용할 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* GlockFireMontage;
	// 재장전 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* GlockReloadMontage;
	// 총쏘기 재생함수
	void PlayGlockFireAnimation();
	void PlayGlockReloadAnimation();

	//-----------------------------Knife-----------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
		UAnimMontage* KnifeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
		UAnimMontage* KnifeAttackMontage2;

	bool KnifeMotion = true;

	void KnifeAttackAnimation();

	//----------------------------Grenade----------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* GrenadeThrowMontage;

	void PlayGrenadeThrowAnimation();

	// 재장전 끝났을 때 호출될 Notify 함수
	UFUNCTION()
	void AnimNotify_OnReloadFinish();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	





};
