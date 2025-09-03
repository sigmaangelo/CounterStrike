// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/Gun.h"

#include "AK47.h"
#include "Character/PlayerCharacter.h"
#include "Animation/PlayerAnimInstance.h"
#include "TracerRound.h"
#include "Camera/CameraComponent.h"
#include "Animation/HandAnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/PlayerStatComponent.h"
#include "../Component/PlayerUIComponent.h"
#include "../Widget/HP_BulletWIdget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../PlayerState/NetPlayerState.h"
#include "../Widget/ChatMain.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundAttenuation.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	SetRootComponent(GunMeshComponent);
	GunMeshComponent->SetCollisionProfileName(TEXT("Gun"));
	GunMeshComponent->SetSimulatePhysics(true);

	BulletREF = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletREF"));
	BulletREF->SetupAttachment(GunMeshComponent);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> bulletDecal(TEXT("/Script/Engine.Material'/Game/Blueprints/Effect/M_BulletDecal.M_BulletDecal'"));
	if (bulletDecal.Succeeded())
	{
		BulletDecal = bulletDecal.Object;
	}

	bAlwaysRelevant = true;
	bReplicates = true;
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	// RPM 분당 발사 수
	FireRate = 60 / FireRate;

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Server_Fire_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Server!! GunAttackAction!!"));

	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (CurrentAmmo >= 1 && Player->ShootingComponent->IsReloading == false)
		{
			FireRecoil();
			CalculateSpread();
			//UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentSpread);
			float X = UKismetMathLibrary::RandomFloatInRange(CurrentSpread * -1, CurrentSpread);
			float Y = UKismetMathLibrary::RandomFloatInRange(CurrentSpread * -1, CurrentSpread);
			float Z = UKismetMathLibrary::RandomFloatInRange(CurrentSpread * -1, CurrentSpread);
			FHitResult HitResult;
			FVector StartLoc = Player->GetFollowCamera()->GetComponentLocation();
			FVector EndLoc = FVector(X, Y, Z) + StartLoc + Player->GetFollowCamera()->GetForwardVector() * GunRange;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Player);
			Params.AddIgnoredActor(this);
			
			bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);

			if (bHasHit)
			{
				//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 1.0f);

				// 맞은 대상이 상대방일 경우 데미지 처리
				auto OtherPlayer = Cast<APlayerCharacter>(HitResult.GetActor());
				if (OtherPlayer)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherPlayer->GetActorNameOrLabel());
					OtherPlayer->UIComponent->ClientDamageWidget(Player);
					OtherPlayer->StatComponent->DamageProcess(Damage);
				
					//GDH Code UI 점수 처리
					//맞은 상대방이 죽었을 때, 
					if (OtherPlayer->StatComponent->Hp <= 0)
					{
						auto ps = Player->GetPlayerState<ANetPlayerState>();
						ps->SetScore(ps->GetScore() + 1);
					}
					//GDH Code

				}
			}

			FireCameraShakeAction();
			CurrentAmmo--;
			if(Player->HasAuthority())
			{
				Client_Fire();
			}
			Multi_Fire(HitResult, StartLoc, EndLoc);
		}
	}
}

void AGun::Client_Fire_Implementation()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, BulletREF, NAME_None, FVector(ForceInit), FRotator(FRotator::ZeroRotator), FVector(0.05));
}

void AGun::Multi_Fire_Implementation(const FHitResult& HitResult, FVector StartLoc, FVector EndLoc)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, BulletREF->GetComponentLocation());

	const FRotator Rotator = HitResult.ImpactNormal.Rotation();
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecal, FVector(10, 5, 5), HitResult.ImpactPoint, Rotator, 10);
	// 예광탄 효과 발사
	GetWorld()->SpawnActor<ATracerRound>(ATracerRound::StaticClass(), BulletREF->GetComponentLocation(), UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));
	
	auto Player = Cast<APlayerCharacter>(GetOwner());
	if (Player->AnimInstance)
	{
		Player->AnimInstance->PlayFireMontage();
	}

	//GDH CODE
	if (Player->IsLocallyControlled())
	{
		Player->UIComponent->ChatMain->WBP_HP_Bullet->UseBullet();
	}
	//Player->UIComponent->HP_BulletWidget->UseBullet();
	//GDH CODE
}

void AGun::Server_Reload_Implementation()
{
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (CurrentAmmo == MaxAmmo) { return; }

		CurrentAmmo = MaxAmmo;

		if (GunReloadAnim)
		{
			GunMeshComponent->PlayAnimation(GunReloadAnim, false);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, BulletREF->GetComponentLocation());
		}

		Multi_Reload();
	}
	
}

void AGun::Multi_Reload_Implementation()
{
	if (CurrentAmmo == MaxAmmo) { return; }

	if (GunReloadAnim)
	{
		GunMeshComponent->PlayAnimation(GunReloadAnim, false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, BulletREF->GetComponentLocation());
	}

	auto Player = Cast<APlayerCharacter>(GetOwner());
	if (Player->AnimInstance)
	{
		Player->AnimInstance->PlayReloadMontage();
	}
}

void AGun::SemiAutoFireAction()
{
	if (!IsFiring)
	{
		Server_Fire();
		IsFiring = true;
	}
}

void AGun::FullAutoFireAction()
{
	if(!IsFiring)
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AGun::Server_Fire, FireRate, true);
		Server_Fire();
		IsFiring = true;
	}
}

void AGun::StopFireAction()
{
	GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
	IsFiring = false;
}

void AGun::FireCameraShakeAction()
{
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (FireCameraShake)
		{
			auto PC = Cast<APlayerController>(Player->Controller);
			PC->ClientStartCameraShake(FireCameraShake);
		}
	}
}

void AGun::FireRecoil()
{
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->AddControllerPitchInput(-RecoilValue);
	}
}

void AGun::CalculateSpread()
{
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		CurrentSpread = Player->GetCharacterMovement()->Velocity.Size() * SpeedSpreadFactor;
		CurrentSpread = FMath::Clamp(CurrentSpread, BasicSpread, MaxSpread);
	}
	
}


void AGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGun, CurrentAmmo);
	DOREPLIFETIME(AGun, GunFireAnim);
	DOREPLIFETIME(AGun, GunReloadAnim);
}