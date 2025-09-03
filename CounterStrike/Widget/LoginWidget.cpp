// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LoginWidget.h"
#include "../GameInstance/GI.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Slider.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "SessionSlotWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UGI>(GetWorld()->GetGameInstance());
	// 슬롯 추가 이벤트 콜백 등록하기
	GI->OnSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotWidget);
	Btn_Find->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedFindSession);
	GI->OnSearchState.AddDynamic(this, &ULoginWidget::OnChangeButtonEnabled);

	Btn_CreateRoom->OnClicked.AddDynamic(this,&ULoginWidget::CreateRoom);
	Slider_PlayerCount->OnValueChanged.AddDynamic(this,&ULoginWidget::OnValueChanged);

	Btn_CreateSession->OnClicked.AddDynamic(this,&ULoginWidget::SwitchCreatePanel);
	Btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::SwitchFindPanel);
	Btn_Back->OnClicked.AddDynamic(this, &ULoginWidget::SwitchMainPanel);
	Btn_Back2->OnClicked.AddDynamic(this, &ULoginWidget::SwitchMainPanel);

}

void ULoginWidget::AddSlotWidget(const struct FSessionInfo& SessionInfo)
{
	//위젯 생성
	if (SessionSlotWidget)
	{
		auto slot = CreateWidget<USessionSlotWidget>(this, SessionSlotWidget);
		slot->Set(SessionInfo);
		//스크롤 박스에 추가
		ScrollRoomList->AddChild(slot);

	}


}

void ULoginWidget::CreateRoom()
{
	//GameInstance 에 있는 CreateMySession함수 호출
	if (GI && Edit_RoomName->GetText().IsEmpty() == false)
	{
		const FString RoomName = Edit_RoomName->GetText().ToString();
		GI->CreateMySession(RoomName, (int32) Slider_PlayerCount->GetValue());

	}


}

void ULoginWidget::OnValueChanged(const float Value)
{
	Txt_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULoginWidget::SwitchCreatePanel()
{
	//유저 이름 갱신
	FString UserName = Edit_UserName->GetText().ToString();
	if (UserName.IsEmpty() == false)
	{

		GI->MySessionName = UserName;
	}

	WidgetSwitcher->SetActiveWidgetIndex(1);

}

void ULoginWidget::SwitchFindPanel()
{
	//유저 이름 갱신
	// 방 목록 검색
	FString UserName = Edit_UserName->GetText().ToString();
	if (UserName.IsEmpty() == false)
	{

		GI->MySessionName = UserName;
	}

	WidgetSwitcher->SetActiveWidgetIndex(2);
	GI->FindOtherSessions();

}

void ULoginWidget::SwitchMainPanel()
{

	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULoginWidget::OnClickedFindSession()
{
	// 방 목록 리셋
	ScrollRoomList->ClearChildren();

	// 방 목록 검색

	if (GI)
	{
		GI->FindOtherSessions();
	}
}

void ULoginWidget::OnChangeButtonEnabled(bool bIsSearching)
{
	Btn_Find->SetIsEnabled(!bIsSearching);
	if (bIsSearching)
	{
		Txt_FindingMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		Txt_FindingMsg->SetVisibility(ESlateVisibility::Hidden);

	}
}
