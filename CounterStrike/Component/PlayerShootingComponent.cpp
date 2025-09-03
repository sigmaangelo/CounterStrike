// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShootingComponent.h"
#include "PlayerStatComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "Animation/PlayerAnimInstance.h"
#include "Animation/HandAnimInstance.h"
#include "PlayerUIComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Guns/AK47.h"
#include "Guns/Glock.h"
#include "Guns/Gun.h"
#include "Guns/Knife.h"
#include "Kismet/GameplayStatics.h"
#include <Widget/HP_BulletWIdget.h>
#include "../Widget/ChatMain.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerShootingComponent::UPlayerShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicated(true);
}


// Called when the game starts
void UPlayerShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<APlayerCharacter>();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 2);
		}
	}

	if (Player->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetActorNameOrLabel())
			Knife = GetWorld()->SpawnActor<AKnife>(KnifeClass);
		check(Knife);;
		//Knife->GetRootComponent()->SetVisibility(false);
		CurrentGun = Knife;
		CurrentGun->SetOwner(Player);
		AttachGun(Knife);
	}
	
}

// Called every frame
void UPlayerShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	 
}

void UPlayerShootingComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (nullptr == InputComponent)
		{
			InputComponent = EnhancedInput;
		}
		check(IA_Shoot)
		check(IA_Reload)
		
		EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Started, this, &UPlayerShootingComponent::Fire);
		EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Completed, this, &UPlayerShootingComponent::StopFire);
		EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Started, this, &UPlayerShootingComponent::LeftClickStarted);
		EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Completed, this,		&UPlayerShootingComponent::LeftClickCompleted);
		EnhancedInput->BindAction(IA_Reload, ETriggerEvent::Started, this, &UPlayerShootingComponent::Reload);
		EnhancedInput->BindAction(IA_PickUp, ETriggerEvent::Started, this, &UPlayerShootingComponent::TakeGun);
		EnhancedInput->BindAction(IA_Drop, ETriggerEvent::Started, this, &UPlayerShootingComponent::ReleaseGun);
		EnhancedInput->BindAction(IA_PrimaryGun, ETriggerEvent::Started, this, &UPlayerShootingComponent::ButtonOne);
		EnhancedInput->BindAction(IA_SecondaryGun, ETriggerEvent::Started, this, &UPlayerShootingComponent::ButtonTwo);
		EnhancedInput->BindAction(IA_Melee, ETriggerEvent::Started, this, &UPlayerShootingComponent::ButtonThree);
	}
}

void UPlayerShootingComponent::Fire(const FInputActionInstance& value)
{
	// 예외 처리
	//if (!Player->StatComponent->bHasGun) { return; }
	//if (Player->StatComponent->bIsReload) { return; }
	if (Player->StatComponent->bIsDead) { return; }
	if (CurrentGun == nullptr) { return; }
	if (IsReloading) { return; };

	

	if(CurrentGun == Knife)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentGun->GetActorNameOrLabel());
		Knife->Server_KnifeAttackAction();

	}
	else if (CurrentGun == PrimaryGun)
	{
		CurrentGun->FullAutoFireAction();
	}
	else if(CurrentGun == SecondaryGun)
	{
		CurrentGun->SemiAutoFireAction();
	}

	//Player->UIComponent->Shoot();
}

//void UPlayerShootingComponent::Server_Fire_Implementation()
//{
//	if (CurrentGun->GunName == EGunName::AK47 || CurrentGun->GunName == EGunName::MP5)
//	{
//		CurrentGun->FullAutoFireAction();
//	}
//	else if (CurrentGun->GunName == EGunName::Glock)
//	{
//		CurrentGun->SemiAutoFireAction();
//	}
//	else
//	{
//		Knife->Server_KnifeAttackAction();
//	}
//
//}
//
//void UPlayerShootingComponent::Multi_Fire_Implementation(bool bHasHit, const FHitResult& HitResult, const int32 InAmmoCount)
//{
//	// 총알 갱신
//	CurrentGun->CurrentAmmo = InAmmoCount;
//
//	// 데칼 스폰
//	const FRotator Rotator = HitResult.ImpactNormal.Rotation();
//	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CurrentGun->BulletDecal, FVector(10, 5, 5), HitResult.ImpactPoint, Rotator, 5);
//
//	// 총 애니메이션 재생
//	if (CurrentGun->GunFireAnim)
//	{
//		CurrentGun->GunMeshComponent->PlayAnimation(CurrentGun->GunFireAnim, false);
//	}
//	
//	// 공격 애니메이션 실행
//	if (Player->AnimInstance)
//	{
//		Player->AnimInstance->PlayFireMontage();
//	}
//}

void UPlayerShootingComponent::StopFire(const FInputActionInstance& value)
{
	if(!CurrentGun)
	{
		return;
	}
	CurrentGun->StopFireAction();
}

void UPlayerShootingComponent::Reload(const FInputActionValue& value)
{
	// 예외 처리
	/*if (Player->StatComponent->bIsReload) { return; }
	Player->StatComponent->bIsReload = true;*/

	if (!CurrentGun || IsReloading || CurrentGun->CurrentAmmo == CurrentGun->MaxAmmo)
	{
		return;
	}
	IsReloading = true;
	CurrentGun->Server_Reload();
	//Server_Reload();

	/*auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
	Anim->PlayAKReloadAnimation();*/

	//UE_LOG(LogTemp, Warning, TEXT("%d"), IsReloading);
	// GDH CODE
	Player->UIComponent->Reload();
}

//void UPlayerShootingComponent::Server_Reload_Implementation()
//{
//	// 클라이언트 RPC
//	Multi_Reload();
//}
//
//void UPlayerShootingComponent::Multi_Reload_Implementation()
//{
//	// 장전 애니메이션 실행
//	if (Player->AnimInstance)
//	{
//		Player->AnimInstance->PlayReloadMontage();
//	}
//}

void UPlayerShootingComponent::TakeGun(const FInputActionValue& value)
{
	// 서버 로직
	Server_TakeGun();

	//GDH CODE
	Player->UIComponent->SetVisible();
	//GDH CODE
}

void UPlayerShootingComponent::Server_TakeGun_Implementation()
{
	TArray<AActor*> AllGuns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGun::StaticClass(), AllGuns);

	for (auto TempGun : AllGuns)
	{
		float Distance = FVector::Dist(Player->GetActorLocation(), TempGun->GetActorLocation());

		if (Distance > DistanceCanGrabGun)
		{
			continue;
		}
		auto ThisGun = Cast<AGun>(TempGun);
		if (!PrimaryGun && ThisGun->GunType == EWeaponType::Primary)
		{
			PrimaryGun = ThisGun;
			//PrimaryGun->GetRootComponent()->SetVisibility(false);
			Multi_TakeGun(PrimaryGun);
			break;
		}
		else if (!SecondaryGun && ThisGun->GunType == EWeaponType::Secondary)
		{
			SecondaryGun = ThisGun;
			//SecondaryGun->GetRootComponent()->SetVisibility(false);
			Multi_TakeGun(SecondaryGun);
			break;
		}

	}
}

void UPlayerShootingComponent::Multi_TakeGun_Implementation(AGun* Gun)
{
	// 총 1인칭만 보이게
	Gun->GunMeshComponent->SetOnlyOwnerSee(true);

	if(Gun->GunType == EWeaponType::Primary)
	{
		PrimaryGun = Gun;
		PrimaryGun->GetRootComponent()->SetVisibility(false);
	}

	if(Gun->GunType == EWeaponType::Secondary)
	{
		SecondaryGun = Gun;
		SecondaryGun->GetRootComponent()->SetVisibility(false);
	}
	AttachGun(Gun);
}

void UPlayerShootingComponent::ReleaseGun(const FInputActionValue& value)
{
	if(!PrimaryGun && !SecondaryGun || IsReloading || !CurrentGun || CurrentGun == Knife)
	{
		return;
	}
	// 서버 로직
	Server_ReleaseGun();

}

void UPlayerShootingComponent::Server_ReleaseGun_Implementation()
{
	if (CurrentGun == PrimaryGun)
	{
		//PrimaryGun->GetRootComponent()->SetVisibility(true);
		PrimaryGun = nullptr;
	}
	else if (CurrentGun == SecondaryGun)
	{
		//SecondaryGun->GetRootComponent()->SetVisibility(true);
		SecondaryGun = nullptr;
	}
	else if(CurrentGun == Knife)
	{
		return;
	}
	Multi_ReleaseGun(CurrentGun);
	CurrentGun->SetOwner(nullptr);
	CurrentGun = nullptr;
	Server_ChooseMelee();
}

void UPlayerShootingComponent::Multi_ReleaseGun_Implementation(AGun* Gun)
{
	// 총 1인칭만 보이게
	Gun->GunMeshComponent->SetOnlyOwnerSee(false);

	if (CurrentGun == PrimaryGun)
	{
		PrimaryGun->GetRootComponent()->SetVisibility(true);
		//PrimaryGun = nullptr;
	}
	else if (CurrentGun == SecondaryGun)
	{
		SecondaryGun->GetRootComponent()->SetVisibility(true);
		//SecondaryGun = nullptr;
	}
	DetachGun(CurrentGun);
}

void UPlayerShootingComponent::ButtonOne(const FInputActionValue& value)
{
	if (!CurrentGun) { return; }
	if (PrimaryGun == nullptr) { return; }
	if (CurrentGun->IsFiring) { return; }
	if (IsReloading) { return; }

	Server_ChoosePrimaryGun();
}

void UPlayerShootingComponent::ButtonTwo(const FInputActionValue& value)
{
	if (!CurrentGun) { return; }
	if (SecondaryGun == nullptr) { return; }
	if (CurrentGun->IsFiring) { return; }
	if (IsReloading) { return; }


	Server_ChooseSecondaryGun();
}

void UPlayerShootingComponent::ButtonThree(const FInputActionValue& value)
{
	if (!CurrentGun) { return; }
	if (CurrentGun->IsFiring) { return; }
	if (IsReloading) { return; }

	UE_LOG(LogTemp, Warning, TEXT("three called"))
	
	Server_ChooseMelee();
}

void UPlayerShootingComponent::AttachGun(AGun* Gun)
{
	auto MeshComp = Gun->GetComponentByClass<USkeletalMeshComponent>();
	MeshComp->SetSimulatePhysics(false);
	MeshComp->AttachToComponent(Player->GunComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void UPlayerShootingComponent::DetachGun(AGun* Gun)
{
	auto MeshComp = Gun->GetComponentByClass<USkeletalMeshComponent>();
	MeshComp->SetSimulatePhysics(true);
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);


}

void UPlayerShootingComponent::ReloadFinish()
{
	IsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("%d"), IsReloading);
	Player->UIComponent->ChatMain->WBP_HP_Bullet->ResetBullet();
	//Player->UIComponent->HP_BulletWidget->ResetBullet();
}




void UPlayerShootingComponent::Server_ChoosePrimaryGun_Implementation()
{
	if (!PrimaryGun || IsReloading || CurrentGun == PrimaryGun)
	{
		return;
	}
	WeaponEquipped = 0;

	/*if (SecondaryGun)
	{
		SecondaryGun->GetRootComponent()->SetVisibility(false);
	}
	if (CurrentGun == Knife)
	{
		Knife->GetRootComponent()->SetVisibility(false);
	}
	PrimaryGun->GetRootComponent()->SetVisibility(true);*/
	CurrentGun = PrimaryGun;
	CurrentGun->SetOwner(Player);
	Multi_ChoosePrimaryGun(CurrentGun, SecondaryGun, Knife);

}

void UPlayerShootingComponent::Client_ChoosePrimaryGun_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (SG)
	{
		SG->GetRootComponent()->SetVisibility(false);
	}
	if (ML)
	{
		ML->GetRootComponent()->SetVisibility(false);
	}
	PG->GetRootComponent()->SetVisibility(true);

	CurrentGun = PG;
}

void UPlayerShootingComponent::Multi_ChoosePrimaryGun_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (SG)
	{
		SG->GetRootComponent()->SetVisibility(false);
	}
	if (ML)
	{
		ML->GetRootComponent()->SetVisibility(false);
	}
	PG->GetRootComponent()->SetVisibility(true);

	CurrentGun = PG;

}

void UPlayerShootingComponent::Server_ChooseSecondaryGun_Implementation()
{
	if (!SecondaryGun || IsReloading || CurrentGun == SecondaryGun)
	{
		return;
	}
	WeaponEquipped = 1;
	/*if (PrimaryGun)
	{
		PrimaryGun->GetRootComponent()->SetVisibility(false);
	}
	if (CurrentGun == Knife)
	{
		Knife->GetRootComponent()->SetVisibility(false);
	}
	SecondaryGun->GetRootComponent()->SetVisibility(true);*/
	
	CurrentGun = SecondaryGun;
	CurrentGun->SetOwner(Player);
	Multi_ChooseSecondaryGun(PrimaryGun, CurrentGun, Knife);
}

void UPlayerShootingComponent::Client_ChooseSecondaryGun_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (PG)
	{
		PG->GetRootComponent()->SetVisibility(false);
	}
	if (ML)
	{
		ML->GetRootComponent()->SetVisibility(false);
	}
	SG->GetRootComponent()->SetVisibility(true);

	CurrentGun = SG;;
}

void UPlayerShootingComponent::Multi_ChooseSecondaryGun_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (PG)
	{
		PG->GetRootComponent()->SetVisibility(false);
	}
	if (ML)
	{
		ML->GetRootComponent()->SetVisibility(false);
	}
	SG->GetRootComponent()->SetVisibility(true);

	CurrentGun = SG;;

}

void UPlayerShootingComponent::Server_ChooseMelee_Implementation()
{
	if(!Knife || IsReloading || CurrentGun == Knife)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Chose melee"))
	WeaponEquipped = 2;
	/*if (PrimaryGun)
	{
		PrimaryGun->GetRootComponent()->SetVisibility(false);
	}
	if (SecondaryGun)
	{
		SecondaryGun->GetRootComponent()->SetVisibility(false);
	}
	Knife->GetRootComponent()->SetVisibility(true)*/
	CurrentGun = Knife;
	CurrentGun->SetOwner(Player);
	Multi_ChooseMelee(PrimaryGun, SecondaryGun, CurrentGun);
}

void UPlayerShootingComponent::Client_ChooseMelee_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (PG)
	{
		PG->GetRootComponent()->SetVisibility(false);
	}
	if (SG)
	{
		SG->GetRootComponent()->SetVisibility(false);
	}
	ML->GetRootComponent()->SetVisibility(true);

	CurrentGun = ML;
}


void UPlayerShootingComponent::Multi_ChooseMelee_Implementation(AGun* PG, AGun* SG, AGun* ML)
{
	if (PG)
	{
		PG->GetRootComponent()->SetVisibility(false);
	}
	if (SG)
	{
		SG->GetRootComponent()->SetVisibility(false);
	}
	ML->GetRootComponent()->SetVisibility(true);

	CurrentGun = ML;

}


void UPlayerShootingComponent::LeftClickStarted()
{
	if(!CurrentGun || CurrentGun->CurrentAmmo <= 0)
	{
		return;
	}
	Player->UIComponent->ShootStart();
}

void UPlayerShootingComponent::LeftClickCompleted()
{
	Player->UIComponent->ShootComplete();
}

void UPlayerShootingComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerShootingComponent, CurrentGun);
	DOREPLIFETIME(UPlayerShootingComponent, PrimaryGun);
	DOREPLIFETIME(UPlayerShootingComponent, SecondaryGun);
	DOREPLIFETIME(UPlayerShootingComponent, AK47);
	DOREPLIFETIME(UPlayerShootingComponent, MP5);
	DOREPLIFETIME(UPlayerShootingComponent, Glock);
	DOREPLIFETIME(UPlayerShootingComponent, AK47Class);
	DOREPLIFETIME(UPlayerShootingComponent, MP5Class);
	DOREPLIFETIME(UPlayerShootingComponent, GlockClass);
	DOREPLIFETIME(UPlayerShootingComponent, Knife);
	DOREPLIFETIME(UPlayerShootingComponent, KnifeClass);
	DOREPLIFETIME(UPlayerShootingComponent, IsReloading);
	DOREPLIFETIME(UPlayerShootingComponent, WeaponEquipped);

}
