// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerStatComponent.h"
#include "Character/PlayerCharacter.h"
#include "Animation/PlayerAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/PlayerShootingComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerUIComponent.h"
#include "../Widget/HP_BulletWIdget.h"
#include "../Widget/ChatMain.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...
	
	
	//컴포넌트도 Replicated 해줘야함 SetOwner는 안해줘도 이건 해줘야하나봄.
	//SetIsReplicated(true);
	//컴포넌트도 Replicated 해줘야함 


}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner<APlayerCharacter>();
}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UPlayerStatComponent, bHasGun);
	DOREPLIFETIME(UPlayerStatComponent, Hp);
}

void UPlayerStatComponent::DamageProcess(int32 Damage)
{
	Hp -= Damage;
	OnRep_Hp();
}

void UPlayerStatComponent::DeadProcess()
{
	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player->GetCharacterMovement()->DisableMovement();
	Player->AnimInstance->PlayDeadMontage();
	Player->ShootingComponent->Server_ReleaseGun();
	Player->UIComponent->ChatMain->OnRespawn();
}

void UPlayerStatComponent::OnRep_Hp()
{
	if (Player->IsLocallyControlled())
	{
		if (Player->UIComponent->ChatMain)
		{
			Player->UIComponent->ChatMain->WBP_HP_Bullet->DamageHP(34);
			//Player->UIComponent->HP_BulletWidget->DamageHP(34);
		}

		if (Hp <= 0)
		{
			DeadProcess();
		}
	}
}
