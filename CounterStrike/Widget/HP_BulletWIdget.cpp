// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HP_BulletWIdget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>
#include "../Character/PlayerCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../Component/PlayerStatComponent.h"

void UHP_BulletWIdget::DamageHP(int32 Damage)
{
	if (CurrentHP <= 0) 
	{
		return;
	}
	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0.0f, 100.0f);
	HP->SetText(FText::AsNumber(CurrentHP));
	HPBar->SetPercent(CurrentHP/100);
}

void UHP_BulletWIdget::UseBullet()
{
	if (CB <= 0)
	{
		return;
	}
	CB--;
	CB = FMath::Clamp(CB,0.0f, 30.0f);
	CurrentBullet->SetText(FText::AsNumber(CB));

}

void UHP_BulletWIdget::ResetBullet()
{
	CB = 30.0f;
	CurrentBullet->SetText(FText::AsNumber(CB));
}

void UHP_BulletWIdget::Magnification_AK47()
{

}

void UHP_BulletWIdget::Magnification_Gun()
{

}

void UHP_BulletWIdget::Magnification_Knife()
{

}

void UHP_BulletWIdget::Magnification_Granade()
{

}

void UHP_BulletWIdget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	

}

void UHP_BulletWIdget::NativeConstruct()
{
	//Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	
}
