// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "Character/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerShootingComponent.h"
#include "PlayerUIComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values for this component's properties
UPlayerMoveComponent::UPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

}


// Called when the game starts
void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<APlayerCharacter>();
	//Add enhanced Input Mapping
	/*check(InputMapping)
		ULocalPlayer* LocalPlayer = Player->GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, 2);
		}*/
	if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 1);
		}
	}
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerMoveComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (nullptr == InputComponent)
		{
			InputComponent = EnhancedInput;
		}
		check(IA_Crouch)
		check(IA_Walk)
		EnhancedInput->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::PlayerCrouch);
		EnhancedInput->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &UPlayerMoveComponent::PlayerUnCrouch);

		EnhancedInput->BindAction(IA_Walk, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::Walk);
		EnhancedInput->BindAction(IA_Walk, ETriggerEvent::Completed, this, &UPlayerMoveComponent::StopWalk);
	}
}

void UPlayerMoveComponent::PlayerCrouch(const FInputActionValue& value)
{
	Player->Crouch();
	Player->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	IsCrouch = true;
	// -------GDH_ CODE --------
	Player->UIComponent->Zoom();
	// -------GDH_ CODE --------
}

void UPlayerMoveComponent::PlayerUnCrouch(const FInputActionValue& value)
{
	Player->UnCrouch();
	Player->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	IsCrouch = false;
}

void UPlayerMoveComponent::Walk(const FInputActionValue& value)
{

	Player->GetCharacterMovement()->MaxWalkSpeed = 300.0f;


	// -------GDH_ CODE --------
	Player->UIComponent->Zoom();
	// -------GDH_ CODE --------

}

void UPlayerMoveComponent::StopWalk(const FInputActionValue& value)
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 600.0f;


}

