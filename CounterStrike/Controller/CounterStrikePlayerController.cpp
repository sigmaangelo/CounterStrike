// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CounterStrikePlayerController.h"
#include "GameMode/CounterStrikeGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "../Widget/ChatMain.h"

void ACounterStrikePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 서버일때만 게임모드 인스턴스 할당
	if (HasAuthority())
	{
		CounterStrikeGameMode = Cast<ACounterStrikeGameMode>(GetWorld()->GetAuthGameMode());

	}
	//GDH CODE
	//자기자신일때, UI 생성
	/*if (IsLocalController()) 
	{
		ChatMain = Cast<UChatMain>(CreateWidget(GetWorld(),ChatMainWidget));
		ChatMain->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
	}*/
	//GDH CODE

}

void ACounterStrikePlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	// 1. 기존 플레이어
	auto ExistingPlayer = GetPawn();

	// 2. 빙의 해제
	UnPossess();

	// 3. 기존 플레이어 삭제
	ExistingPlayer->Destroy();

	// 4. 새로운 플레이어 빙의
	CounterStrikeGameMode->RestartPlayer(this);
}

void ACounterStrikePlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	// 1. 기존 폰 가져오기
	auto ExistingPlayer = GetPawn();

	// 2. 관전자 스폰
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(CounterStrikeGameMode->SpectatorClass, ExistingPlayer->GetActorTransform(), Params);

	// 3. 빙의
	Possess(Spectator);

	// 4. 기존 폰 제거
	ExistingPlayer->Destroy();

	// 5. 리스폰
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACounterStrikePlayerController::ServerRPC_RespawnPlayer_Implementation, 5.0f, false);
}
