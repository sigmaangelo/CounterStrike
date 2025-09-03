#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GI.generated.h"


// 방 정보(세션) 기록할 구조체
USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString RoomName;
	
	UPROPERTY(BlueprintReadOnly)
	FString HostName;
	
	UPROPERTY(BlueprintReadOnly)
	FString PlayerCount;
	
	UPROPERTY(BlueprintReadOnly)
	int32 PingSpeed;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s , %d ms"), Index, *RoomName, *HostName, *PlayerCount, PingSpeed);
	}


};
// 세션 검색이 끝났을 때 호출될 Delegate
// 블루프린트(Dynamic)와 브로드캐스팅(Multicast) 기능을 사용한다면 DECLARE_Dynamic_Multicast_DELEGATE_OneParam
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, SessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);


UCLASS()
class COUNTERSTRIKE_API UGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGI();

	virtual void Init()override;

	// 세션(방) 설정
	IOnlineSessionPtr SessionInterface;

	// 세션의 주인(HOST) 이름 <- 방장이름
	FString MySessionName = "Mumyung";

	//세션 생성 함수
	void CreateMySession(FString RoomName, int32 PlayerCount);
	//세션 생성 완료 이벤트 콜백
	void OnCreateSessionComplete(FName SessionName,bool bWasSuccessful);
	
	// 방 검색
	TSharedPtr<class FOnlineSessionSearch> SessionSearch; // UPROPERTY를 쓸 수 없는 순수 C++ 클래스기 때문에 TSharedPtr 사용
	void FindOtherSessions();

	// 세션 검색 완료 시 이벤트 콜백
	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	// 방 찾기 완료되면 콜백을 등록 할 델리게이트
	FSearchSignature OnSearchCompleted;

	// 방 찾기 상태 콜백 델리게이트
	FSearchStateSignature OnSearchState;

	//세션 입장 함수
	void JoinSelectedSession(int32 RoomIndex);

	//세션 입장 델리게이트 콜백
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

};

