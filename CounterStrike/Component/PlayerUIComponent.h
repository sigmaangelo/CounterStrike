// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "PlayerUIComponent.generated.h"

UENUM(BlueprintType)
enum class ECrosshairState : uint8
{
	None UMETA(DisplayName = "NONE"),
	Move UMETA(DisplayName = "MOVE"),
	Shot UMETA(DisplayName = "SHOT"),
	Reload UMETA(DisplayName = "RELOAD")

};

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COUNTERSTRIKE_API UPlayerUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ----------------Function-----------------
	
	// ---- inhanced input ----
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	// ---- inhanced input ----

	void InitWidget();
	void Move();
	void ShootStart();
	void Shoot();
	void ShootComplete();
	void SetState(ECrosshairState Next);
	void Zoom();
	void Reload();
	UFUNCTION(BlueprintCallable)
	void Damage(APlayerCharacter* Enemy);

	UFUNCTION(Client, Reliable)
	void ClientDamageWidget(APlayerCharacter* Enemy);

	void SetVisible();
	void SetUnVisible();
	void FinishReload();
	void PlayReloadAnimation();
	void TestLog(const FInputActionValue& value);
	void SetVisibleChat(const FInputActionValue& value);

	// ----------------Variable-----------------
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCrosshair> CrosshairWidgetFactory;

	UPROPERTY(EditDefaultsOnly, Category ="UI")
	TSubclassOf<class UReloadWidget> ReloadWidgetFactory;

	UPROPERTY()
	class UCrosshair* CrosshairWidget;

	UPROPERTY()
	class UReloadWidget* ReloadWidget;

	UPROPERTY()
	class APlayerCharacter* Player;

	float Delta;

	ECrosshairState State;

	struct FTimerHandle Handle;

	UPROPERTY(EditDefaultsOnly, Category ="UI")
	TSubclassOf<class UHP_BulletWIdget> HP_BulletWidgetFactory;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	class UHP_BulletWIdget* HP_BulletWidget;


	// ---- inhanced input ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettingsInputs")
	class UInputAction* IA_Enter;

	// ---- inhanced input ----


	// UI 통합 중.. ChatMain을 불러오기 위한 PC 저장
	UPROPERTY()
	class UChatMain* ChatMain;
};
