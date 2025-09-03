// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/Knife.h"

#include "Animation/HandAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/PlayerStatComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKnife::AKnife()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> knifeMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPSPawn/Meshes/Knife_Rigged.Knife_Rigged'"));

	if (knifeMesh.Succeeded())
	{
		GunMeshComponent->SetSkeletalMesh(knifeMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> knifeDecal(TEXT("/Script/Engine.Material'/Game/Blueprints/Effect/M_KnifeDecal.M_KnifeDecal'"));
	if (knifeDecal.Succeeded())
	{
		KnifeDecal = knifeDecal.Object;
	}

	GunRange = 100;
	Damage = 30;
	GunType = EWeaponType::Melee;
	GunName = EGunName::Knife;

	GunMeshComponent->bOnlyOwnerSee = true;
}

void AKnife::Server_KnifeAttackAction_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Server!! KnifeAttackAction!!"));

	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		/*auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
		Anim->KnifeAttackAnimation();*/

		FHitResult HitResult;
		FVector StartLoc = Player->GetFollowCamera()->GetComponentLocation();
		FVector EndLoc = StartLoc + Player->GetFollowCamera()->GetForwardVector() * KnifeRange;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Player);
		Params.AddIgnoredActor(this);
		bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);
		if (bHasHit)
		{
			//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 1.0f);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnifeHitSound, BulletREF->GetComponentLocation());
			// 맞은 대상이 상대방일 경우 데미지 처리
			auto OtherPlayer = Cast<APlayerCharacter>(HitResult.GetActor());
			if (OtherPlayer)
			{
				OtherPlayer->StatComponent->DamageProcess(Damage);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherPlayer->GetActorNameOrLabel());
			}
		}
		
		Multi_KnifeAttackAction(HitResult, StartLoc, EndLoc);
	}
}

void AKnife::Multi_KnifeAttackAction_Implementation(const FHitResult& HitResult, FVector StartLoc, FVector EndLoc)
{
	if (auto Player = Cast<APlayerCharacter>(GetOwner()))
	{
		auto Anim = Cast<UHandAnimInstance>(Player->HandComponent->GetAnimInstance());
		Anim->KnifeAttackAnimation();

		FRotator Rotator = UKismetMathLibrary::Conv_VectorToRotator(Player->GetFollowCamera()->GetForwardVector());
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), KnifeDecal, FVector(20, 10, 10), HitResult.ImpactPoint, Rotator, 5);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnifeAirSound, BulletREF->GetComponentLocation());
	}
	
}
	
	


