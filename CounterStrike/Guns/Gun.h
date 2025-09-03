// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/PlayerShootingComponent.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class COUNTERSTRIKE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//----------------------------Variable--------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* BulletREF;

	UPROPERTY(EditAnywhere, Replicated, Category = Effect)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Replicated, Category = Effect)
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, Replicated, Category = Effect)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = Effect)
	UMaterialInterface* BulletDecal;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float GunRange = 20000;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 MaxAmmo;

	UPROPERTY(Replicated)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float BasicSpread;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float CurrentSpread;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float MaxSpread;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float SpeedSpreadFactor = 4.0f;

	UPROPERTY(EditAnywhere, Replicated)
	UAnimMontage* GunFireAnim;

	UPROPERTY(EditAnywhere, Replicated)
	UAnimMontage* GunReloadAnim;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> FireCameraShake;

	EWeaponType GunType;
	EGunName GunName;
	bool IsFiring = false;
	FTimerHandle WeaponFireHandle;

	UPROPERTY(EditAnywhere)
	float RecoilValue;

	//----------------------------Function--------------------------------

	UFUNCTION(Server, Reliable)
	virtual void Server_Fire();

	UFUNCTION(Client, Reliable)
	void Client_Fire();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Fire(const FHitResult& HitResult, FVector StartLoc, FVector EndLoc);

	UFUNCTION(Server, Reliable)
	virtual void Server_Reload();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Reload();

	virtual void SemiAutoFireAction();

	virtual void FullAutoFireAction();

	virtual void StopFireAction();

	virtual void FireCameraShakeAction();

	virtual void FireRecoil();

	void CalculateSpread();
};
