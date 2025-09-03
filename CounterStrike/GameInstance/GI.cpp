// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/GI.h"
#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h>
#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include "../CounterStrike.h"
#include <../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h>

UGI::UGI()
{

}

void UGI::Init()
{
	Super::Init();
	//세션에 몇명을 받은건지
	// 랜 통신을 로컬에서 할건지, 스팀으로 할건지
	// 온라인 세션 인터페이스 만들기
	auto Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		SessionInterface = Subsys->GetSessionInterface();
		//콜백
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UGI::OnCreateSessionComplete);

		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UGI::OnFindSessionsComplete);
		
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI::OnJoinSessionCompleted);
	}

	//세션 생성 함수 호출 (비동기 처리)
	// 테스트 용이었음. 이제 안쓸거임.
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
	{
		//CreateMySession(MySessionName, 10);
		//FindOtherSessions();

	}), 2, false);

}

void UGI::CreateMySession(FString RoomName, int32 PlayerCount)
{
	//PRINTLOG(TEXT("RoomName : %s, pc : %d"), *RoomName, PlayerCount);
	
	
	//세션 설정
	FOnlineSessionSettings SessionSettings;

	// 1. Dedicated Server 사용 여부
	SessionSettings.bIsDedicated = false;
	// 2. 랜선 매칭 (로컬) , 스팀 매칭 사용할지 여부
	FName SubsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSettings.bIsLANMatch = SubsysName == "NULL"; // Default Engine.ini를 보면 OnlineSubsystem에서 확인 가능
	// 3. 매칭이 온라인을 통해 노출될 지 여부 (false일 경우 초대를 통해서만 입장 가능)
	SessionSettings.bShouldAdvertise = true;
	// 4. 온라인 상태 정보를 활용하게 할지 여부 (게임중. 자리비움 등..)
	SessionSettings.bUsesPresence = true;
	// 5. 게임 진행 중에 Join 할 수 있는지 여부
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence= true;
	// 6. 최대 허용 인원 수
	SessionSettings.NumPublicConnections = PlayerCount;
	// 7. 커스텀 옵션 -> 룸 네임 , 핑 체크 , 호스트 이름
	SessionSettings.Set(FName("Room_Name"), RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("Host_Name"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	//이부분이 진짜 방 만드는 곳인가봄.
	// 세션 생성 -> 특정 친구를 초대하거나, 보이스 챗을 해야 할 때, NetID가 있어야 하므로 NetID를 가져온다.
	FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(*NetID, FName(MySessionName), SessionSettings);


}

void UGI::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PRINTLOG(TEXT("Create Session Start : %s , %d"), *SessionName.ToString(), bWasSuccessful);

	// 방 안에 들어가도록 레벨을 열어보자.
	if (bWasSuccessful)
	{
		//작음 따옴표 바깥에 있는 것을 지우고, 맵 경로끝에 ?listen 치면 SereverTravel이 가능하다.
		GetWorld()->ServerTravel(TEXT("/Game/Maps/GameMap?listen"));
	}

}

void UGI::FindOtherSessions()
{

	// 검색 조건 설정 (모든 방 , PRIVATE 방)
	SessionSearch = MakeShareable(new FOnlineSessionSearch);// TSharedPtr 언리얼 스마트 포인터는 이렇게 선언한다.

	// 1. Presence 설정이 True 로 되어있는 세션
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. LanMatching 여부
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	//3. 최대 세션 검색 수
	SessionSearch->MaxSearchResults = 10;

	// 4. 검색
	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());

	// 5. 방 검색 중임을 알린다.
	OnSearchState.Broadcast(true);

}

void UGI::OnFindSessionsComplete(bool bWasSuccessful)
{

	// 찾기 실패 시
	if (bWasSuccessful == false)
	{

		PRINTLOG(TEXT("Session Search Failed..."));
		return;
	}


	//세션 검색 결과
	const auto Results = SessionSearch->SearchResults;
	PRINTLOG(TEXT("Search Result Count : %d"), Results.Num());


	// 배열에 각각의 세션이 들어감. 하나씩 검사
	for (int i = 0;i < Results.Num();i++)
	{
		auto sr = Results[i];

		if (sr.IsValid() == false)
		{
			continue;
		}

		FSessionInfo SessionInfo;
		SessionInfo.Index = i;
	

		// 1. 방 이름
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), SessionInfo.RoomName);
		
		// PC 소유자 이름
		FString OwnerName = sr.Session.OwningUserName;
		
		// 2. 호스트 이름
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), SessionInfo.HostName);
		
		// 3. 플레이어 수 (최대 가능 수 - 현재 입장 가능 수)
		int32 MaxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		int32 CurrentPlayerCount = MaxPlayerCount - sr.Session.NumOpenPublicConnections;
		
		SessionInfo.PlayerCount = FString::Printf(TEXT("(%d/%d)"),MaxPlayerCount, CurrentPlayerCount);
		
		//4. 핑 (스팀 매칭일 떄는 9999 이런 식으로 들어온다.)
		SessionInfo.PingSpeed = sr.PingInMs;

		PRINTLOG(TEXT("%s"), *SessionInfo.ToString());

		// 슬롯 추가 브로드캐스팅인듯함. (델리게이트 즉, 이벤트를 여기서 발생 시키겠다.인듯?)
		OnSearchCompleted.Broadcast(SessionInfo);

	}

	OnSearchState.Broadcast(false);

}

void UGI::JoinSelectedSession(int32 RoomIndex)
{
	//여러개의 방 목록 중 하나를 선택
	auto sr = SessionSearch->SearchResults[RoomIndex];

	// 세션에 들어가는 함수
	SessionInterface->JoinSession(0,FName(MySessionName), sr);
}



void UGI::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// Client 가 방으로 들어간다.
	// -> Client Travel
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		//서버가 만든 세션의 URL이 필요함.
		FString URL;
		SessionInterface->GetResolvedConnectString(SessionName, URL);
		
		if (URL.IsEmpty() == false)
		{
			//들어가자 방으로, 클라가 방으로 들어갈 때는 플레이어 컨트롤러가 있어야 한다.
			auto pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(URL,TRAVEL_Absolute);
		}

	}
	else
	{

		PRINTLOG(TEXT("Join Session Failed... %d"), Result );
	}

}
