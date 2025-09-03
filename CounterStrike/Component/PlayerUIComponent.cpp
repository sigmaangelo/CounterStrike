// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUIComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "Controller/CounterStrikePlayerController.h"
#include "../Widget/Crosshair.h"
#include "../Widget/ReloadWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/TimerHandle.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ArrowComponent.h>
#include "../Widget/HP_BulletWIdget.h"
#include "../Widget/ChatMain.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

UPlayerUIComponent::UPlayerUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

}


void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();

	/*Player = GetOwner<APlayerCharacter>();
	Pc = Cast<ACounterStrikePlayerController>(Player->GetController());*/

	/*if (Pc&&Pc->IsLocalController())
	{
		ChatMain = Pc->ChatMain;
	}*/
	//InitWidget();
	SetState(ECrosshairState::None);


	// ----inhanced input ----
	if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 3);
		}
	}
	// ----inhanced input ----


}
void UPlayerUIComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		EnhancedInput->BindAction(IA_Enter, ETriggerEvent::Completed, this, &UPlayerUIComponent::SetVisibleChat);
	}
}


void UPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	switch (State)
	{
	case ECrosshairState::None:
		break;

	case ECrosshairState::Move:
		Move();
		break;
	case ECrosshairState::Shot:
		Shoot();
		break;
	case ECrosshairState::Reload:
		break;

	}
}

void UPlayerUIComponent::InitWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("InitWidge() Player : %s"), Player->Controller ?TEXT("Valid") : TEXT("Invalid"));

	if (!Player->Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWidget() Fail"));
		return;
	}
	auto pc = Cast<ACounterStrikePlayerController>(Player->Controller);
	if (pc->ChatMainWidget)
	{
		if (!pc->ChatMain)
		{
			pc->ChatMain = Cast<UChatMain>(CreateWidget(GetWorld(), pc->ChatMainWidget));
			pc->ChatMain->AddToViewport();

			UE_LOG(LogTemp, Warning, TEXT("InitWidget() AddToViewport"));
		}
		pc->ChatMain->WBP_HP_Bullet->HP->SetText(FText::AsNumber(100));
		pc->ChatMain->WBP_HP_Bullet->HPBar->SetPercent(1);
		ChatMain = pc->ChatMain;

		if (CrosshairWidgetFactory && ReloadWidgetFactory && HP_BulletWidgetFactory)
		{
			//캐스팅 안하고 부모클래스형식으로 원래 담아졌었나?
			//CrosshairWidget = Cast<UCrosshair>(CreateWidget(GetWorld(), CrosshairWidgetFactory));
			//CrosshairWidget->AddToViewport();
			ChatMain->WBP_Zoom->ShowCrosshair(false);
			//CrosshairWidget->ShowCrosshair(false);
			ReloadWidget = Cast<UReloadWidget>(CreateWidget(GetWorld(), ReloadWidgetFactory));
			ReloadWidget->AddToViewport();
			ReloadWidget->ShowReload(false);
			Player->UITargeting->SetVisibility(false);
			//HP_BulletWidget = Cast<UHP_BulletWIdget>(CreateWidget(GetWorld(), HP_BulletWidgetFactory));
			//HP_BulletWidget->AddToViewport();
		}
	}
}

void UPlayerUIComponent::Move()
{
	Delta = GetWorld()->GetDeltaSeconds();

	if (Player->GetVelocity().IsZero())
	{
		ChatMain->WBP_Zoom->SpreadLine(false, Delta * 3);
		//CrosshairWidget->SpreadLine(false, Delta * 3);
		//UE_LOG(LogTemp, Warning, TEXT("Stop"));
		return;
	}
	ChatMain->WBP_Zoom->SpreadLine(true, Delta * 3);
	//CrosshairWidget->SpreadLine(true, Delta*3);
	//UE_LOG(LogTemp, Warning, TEXT("Move"));

}
//앉을때, 천천히 갈때 아직 미구현 상태
void UPlayerUIComponent::Zoom()
{

}
//트리거 start에서 한번 호출 예정
void UPlayerUIComponent::ShootStart()
{
	//Move 상태 일때만, 실행 가능하도록 하기
	if (State != ECrosshairState::Move)
	{
		return;
	}
	SetState(ECrosshairState::Shot);


}
//트리거 TRIGGER에서 연사 호출 예정
void UPlayerUIComponent::Shoot()
{
	ChatMain->WBP_Zoom->SpreadLine(true, Delta * 5);
	//CrosshairWidget->SpreadLine(true, Delta*5);

}
//트리거 complete에서 한번 호출 예정
void UPlayerUIComponent::ShootComplete()
{
	//Shot 상태 일때만, 실행 가능하도록 하기
	if (State != ECrosshairState::Shot)
	{
		return;
	}
	SetState(ECrosshairState::Move);
}

void UPlayerUIComponent::SetState(ECrosshairState Next)
{
	State = Next;
}

void UPlayerUIComponent::Reload()
{
	//어떤 상태에서든 다 올수 있게끔
	if (State == ECrosshairState::None || State == ECrosshairState::Reload)
	{
		return;
	}
	SetState(ECrosshairState::Reload);
	ChatMain->WBP_Zoom->ShowCrosshair(false);
	//CrosshairWidget->ShowCrosshair(false);
	ReloadWidget->ShowReload(true);
	ReloadWidget->PlayAnimationWidget();
	//타이머 함수는 액터를 상속 받은 애에서 사용가능하므로, 접근방식을 이렇게함.
	//2초 뒤에 다시 크로스헤어를 보여준다.
	// 상태를 리로드에서 벗어나 준다.
	//리로드 위젯은 숨긴다.
	Player->GetWorldTimerManager().SetTimer(Handle, this,&UPlayerUIComponent::FinishReload, 2.0f,false);
	// 위젯 애니메이션을 실행시킨다.
	PlayReloadAnimation();
}

void UPlayerUIComponent::ClientDamageWidget_Implementation(APlayerCharacter* Enemy)
{
	if (Player->IsLocallyControlled())
	{
		Damage(Enemy);
	}
}

// 외부 호출
void UPlayerUIComponent::Damage(APlayerCharacter* Enemy)
{
	Player->UITargeting->SetVisibility(true);

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector MyLoc = Player->GetActorLocation();
	EnemyLoc.Z = 0.0f;
	MyLoc.Z = 0.0f;
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Loc : %f, %f, %f"), EnemyLoc.X , EnemyLoc.Y, EnemyLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("My Loc :    %f, %f, %f"), MyLoc.X , MyLoc.Y, MyLoc.Z);
	//적 방향을 구한다
	FVector Dir = EnemyLoc - MyLoc;
	Dir.Normalize();
	//UE_LOG(LogTemp, Warning, TEXT("Dir :    %f, %f, %f"), Dir.X, Dir.Y, Dir.Z);
	float Dot = FVector::DotProduct(Dir, Player->GetActorForwardVector());
	float AcosAngle = FMath::Acos(Dot);
	float AngleDegree = FMath::RadiansToDegrees(AcosAngle);

	if (FVector::DotProduct(Player->GetActorRightVector(), Dir)<0) 
	{
		
		AngleDegree *= -1;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Dot : %.1f, AcosAngle : %.1f, AngleDegree : %.1f"), Dot, AcosAngle, AngleDegree);
	// 
	// 그 방향으로 위젯을 회전 시킨다
	Player->UITargeting->SetRelativeRotation(FRotator(0,0, AngleDegree));
	//UE_LOG(LogTemp, Warning, TEXT("AngleDegree : %.1f"),AngleDegree);
	// 그 위젯을 피치를 90도로 돌린다.
	// 이걸 틱으로 돌린다.

	FTimerHandle handle;
	Player->GetWorldTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda([&]
		{
			Player->UITargeting->SetVisibility(false);
		}), 1, false);
}

//총을 잡았을때 실행됨.
void UPlayerUIComponent::SetVisible()
{
	ChatMain->WBP_Zoom->ShowCrosshair(true);
	//CrosshairWidget->ShowCrosshair(true);
	SetState(ECrosshairState::Move);

}
//총을 놓았을때 실행됨.
void UPlayerUIComponent::SetUnVisible()
{
	ChatMain->WBP_Zoom->ShowCrosshair(false);
	//CrosshairWidget->ShowCrosshair(false);
	SetState(ECrosshairState::None);

}

void UPlayerUIComponent::FinishReload()
{
	ChatMain->WBP_Zoom->ShowCrosshair(true);
	//CrosshairWidget->ShowCrosshair(true);
	ReloadWidget->ShowReload(false);
	SetState(ECrosshairState::Move);
}

void UPlayerUIComponent::PlayReloadAnimation()
{
	ReloadWidget->PlayAnimationWidget();
}

void UPlayerUIComponent::TestLog(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Test"));
}

void UPlayerUIComponent::SetVisibleChat(const FInputActionValue& value)
{

}
