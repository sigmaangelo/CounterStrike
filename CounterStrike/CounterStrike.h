// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 카테고리를 우리가 만들어서 사용가능 UE_LOG(LogTemp 부분을 NetLog로 바꿔주겠다는 의미.)
DECLARE_LOG_CATEGORY_EXTERN(NetLog, Log, All);

#define LOCALROLE (UEnum::GetValueAsString<ENetRole>(GetLocalRole()))
#define REMOTEROLE (UEnum::GetValueAsString<ENetRole>(GetRemoteRole()))

//클라이언트창에서 실행인지 서버인지
#define NETMODE (GetWorld()->GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetWorld()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"))


// 라이프 사이클 확인용
// 함수 호출되는 정보
//주의 보라색 이후에 한칸 띄어줘야함.(정보)
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// 정보 출력하는 함수 메크로
// 얘는 함수기 때문에 붙여서 씀. \ 역슬레시는 뒤에 있는 문장을 한문장으로 쓰기 위함.
#define PRINTLOG(fmt, ...) \
UE_LOG(NetLog, Warning, TEXT("[%s]%s %s"),NETMODE,*CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))

//FSTRING은 *을 붙여야하지만, TEXT는 안붙여도됨!