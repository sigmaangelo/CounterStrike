// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMain.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UChatMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Users;
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnRespawn();

	// child 위젯 바인딩
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Bullet")
	class UHP_BulletWIdget* WBP_HP_Bullet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , meta= (BindWidget), Category = "Bullet")
	class UCrosshair* WBP_Zoom;
	
	// child 위젯 바인딩


};
