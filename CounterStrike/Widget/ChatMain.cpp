// Fill out your copyright notice in the Description page of Project Settings.



#include "Widget/ChatMain.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include "Controller/CounterStrikePlayerController.h"

void UChatMain::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChatMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{	
													
	Super::NativeTick(MyGeometry, InDeltaTime);
													
	//나는 모든 플레이어의 이름을 띄우고 싶다.
	// 다른 사용자들의 GameInstance에 있는 세션네임을 가져오고 싶다.
	// GameMode는 서버에서만 존재하므로 GameState와 PlayerState를 이용해서 가져온다.
																	
	auto PlayerArr = GetWorld()->GetGameState()->PlayerArray;
	FString Name;								
																	
	for (auto pState : PlayerArr) //모든 사용자의 플레이어 정보를 가져온다.
	{												
		Name.Append(FString::Printf(TEXT("%s : %d\n"), *pState->GetPlayerName(), (int32)pState->GetScore()));
	}
																	
	Txt_Users->SetText(FText::FromString(Name));	
					


}

void UChatMain::OnRespawn()
{
	auto CounterStrikePlayerController = Cast<ACounterStrikePlayerController>(GetWorld()->GetFirstPlayerController());

	if (CounterStrikePlayerController)
	{
		CounterStrikePlayerController->SetShowMouseCursor(false);
		CounterStrikePlayerController->ServerRPC_ChangeToSpectator();
	}
}
