// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/NetPlayerState.h"
#include "../GameInstance/GI.h"

void ANetPlayerState::BeginPlay()
{
	Super::BeginPlay();

	auto con = GetPlayerController();	 //PlayerState의 오너는 PlayerController
	if (con && con->IsLocalController()) // PC가 있고, 서버에서는 pc가 여러개이므로 나 자신인지
	{
		auto GI = GetWorld()->GetGameInstance<UGI>();
		ServerRPC_SetUserName(GI->MySessionName);
	
	
	}



}

void ANetPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
{

	SetPlayerName(name);

}
