// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CounterStrikePlayerController.generated.h"

class ACounterStrikeGameMode;

UCLASS()
class COUNTERSTRIKE_API ACounterStrikePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 관전자 서버 로직
	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator();

	// 플레이어 리스폰 서버 로직
	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	// 리스폰 역할을 하는 게임모드
	UPROPERTY()
	TObjectPtr<ACounterStrikeGameMode> CounterStrikeGameMode;


	// UI 통합을 위함.
	//GDH Code
	UPROPERTY(EditDefaultsOnly, Category ="UI")
	TSubclassOf<class UChatMain> ChatMainWidget;

	UPROPERTY(BlueprintReadWrite)
	class UChatMain* ChatMain;
	//GDH Code
};
