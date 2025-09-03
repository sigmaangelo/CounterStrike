// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "PlayerShootingComponent.generated.h"

UENUM(BlueprintType)
enum class EGunName : uint8 {
	Glock = 0 UMETA(DisplayName = "Glock"),
	AK47 = 1  UMETA(DisplayName = "AK47"),
	MP5 = 2   UMETA(DisplayName = "MP5"),
	Knife = 3 UMETA(DisplayName = "Knife"),
	Grenade = 4   UMETA(DisplayName = "Grenade")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Primary = 0	   UMETA(DisplayName = "Primary"),
	Secondary = 1  UMETA(DisplayName = "Secondary"),
	Melee = 2	   UMETA(DisplayName = "Knife"),
	Grenade = 3	   UMETA(DisplayName = "Grenade"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COUNTERSTRIKE_API UPlayerShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//----------------------------Variable--------------------------------
	UPROPERTY()
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* InputMapping;

	UPROPERTY()
	UEnhancedInputComponent* InputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_PickUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_Drop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_PrimaryGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_SecondaryGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
	UInputAction* IA_Melee;

	UPROPERTY(VisibleAnywhere, Replicated)
	class AGun* CurrentGun;

	UPROPERTY(VisibleAnywhere, Replicated)
	AGun* PrimaryGun;

	UPROPERTY(VisibleAnywhere, Replicated)
	AGun* SecondaryGun;

	UPROPERTY(VisibleAnywhere, Replicated)
	class AKnife* Knife;

	/*UPROPERTY(Replicated)
	AGun* Knife;*/

	UPROPERTY(Replicated, EditDefaultsOnly)
	TSubclassOf<class AAK47> AK47Class;

	UPROPERTY(Replicated)
	AAK47* AK47;

	UPROPERTY(Replicated, EditDefaultsOnly)
	TSubclassOf<class AMP5> MP5Class;

	UPROPERTY(Replicated)
	AMP5* MP5;

	UPROPERTY(Replicated, EditDefaultsOnly)
	TSubclassOf<class AGlock> GlockClass;

	UPROPERTY(Replicated)
	AGlock* Glock;

	UPROPERTY(Replicated, EditDefaultsOnly)
	TSubclassOf<class AKnife> KnifeClass;

	

	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsReloading = false;

	UPROPERTY(Replicated, EditAnywhere)
	int WeaponEquipped;

	UPROPERTY(EditAnywhere)
	float DistanceCanGrabGun = 200;

	//---------------------------Function---------------------------------
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void Fire(const FInputActionInstance& value);
	void StopFire(const FInputActionInstance& value);
	void Reload(const FInputActionValue& value);
	void TakeGun(const FInputActionValue& value);
	void ReleaseGun(const FInputActionValue& value);
	void ButtonOne(const FInputActionValue& value);
	void ButtonTwo(const FInputActionValue& value);
	void ButtonThree(const FInputActionValue& value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AttachGun(AGun* Gun);
	void DetachGun(AGun* Gun);

	void ReloadFinish();

	UFUNCTION(Server, Reliable)
	void Server_ChoosePrimaryGun();

	UFUNCTION(Client, Reliable)
	void Client_ChoosePrimaryGun(AGun* PG, AGun* SG, AGun* ML);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChoosePrimaryGun(AGun* PG, AGun* SG, AGun* ML);

	UFUNCTION(Server, Reliable)
	void Server_ChooseSecondaryGun();

	UFUNCTION(Client, Reliable)
	void Client_ChooseSecondaryGun(AGun* PG, AGun* SG, AGun* ML);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChooseSecondaryGun(AGun* PG, AGun* SG, AGun* ML);

	UFUNCTION(Server, Reliable)
	void Server_ChooseMelee();

	UFUNCTION(Client, Reliable)
	void Client_ChooseMelee(AGun* PG, AGun* SG, AGun* ML);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChooseMelee(AGun* PG, AGun* SG, AGun* ML);

	// 총 줍기 RPC
	UFUNCTION(Server, Reliable)
	void Server_TakeGun();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_TakeGun(AGun* Gun);

	// 총 버리기 RPC
	UFUNCTION(Server, Reliable)
	void Server_ReleaseGun();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ReleaseGun(AGun* Gun);

	void LeftClickStarted();
	void LeftClickCompleted();
};
	
	
	//// 공격 RPC
	//UFUNCTION(Server, Reliable)
	//void Server_Fire();

	//UFUNCTION(NetMulticast, Reliable)
	//void Multi_Fire(bool bHasHit, const FHitResult& HitResult, const int32 InAmmoCount);

	//// 장전 RPC
	//UFUNCTION(Server, Reliable)
	//void Server_Reload();

	//UFUNCTION(NetMulticast, Reliable)
	//void Multi_Reload();

	
