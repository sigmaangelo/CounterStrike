// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COUNTERSTRIKE_API UPlayerMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//--------------------------------------Variable----------------------------------------
	UPROPERTY()
		class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInputMappingContext* InputMapping;

	UPROPERTY()
		UEnhancedInputComponent* InputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
		class UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Inputs")
		UInputAction* IA_Walk;

	UPROPERTY(EditDefaultsOnly)
		bool IsCrouch = false;

	//--------------------------------------Function-----------------------------------------
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void PlayerCrouch(const FInputActionValue& value);
	void PlayerUnCrouch(const FInputActionValue& value);
	void Walk(const FInputActionValue& value);
	void StopWalk(const FInputActionValue& value);

};
