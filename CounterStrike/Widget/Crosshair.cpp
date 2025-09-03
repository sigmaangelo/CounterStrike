// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Crosshair.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>

void UCrosshair::SpreadLine(bool bIsSpread, float DeltaSpeed)
{
	
	if (true == bIsSpread)
	{
# 

		CurrentA = FMath::Lerp(CurrentA,30.0f, DeltaSpeed);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), DeltaSpeed);
		
		UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(LineRight->Slot);
		Canvas->SetPosition(FVector2D(CurrentA,0.0f));
		Canvas = Cast<UCanvasPanelSlot>(LineLeft->Slot);
		Canvas->SetPosition(FVector2D(-1 * CurrentA, 0.0f));
		Canvas = Cast<UCanvasPanelSlot>(LineUp->Slot);
		Canvas->SetPosition(FVector2D( 0.0f,-1* CurrentA ));
		Canvas = Cast<UCanvasPanelSlot>(LineDown->Slot);
		Canvas->SetPosition(FVector2D(0.0f, CurrentA));
		//UE_LOG(LogTemp, Warning, TEXT("Spread"));
		


	}
	else {
		CurrentA = FMath::Lerp(CurrentA, 0.0f, DeltaSpeed);

		UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(LineRight->Slot);
		Canvas->SetPosition(FVector2D(CurrentA, 0.0f));
		Canvas = Cast<UCanvasPanelSlot>(LineLeft->Slot);
		Canvas->SetPosition(FVector2D(-1 * CurrentA, 0.0f));
		Canvas = Cast<UCanvasPanelSlot>(LineUp->Slot);
		Canvas->SetPosition(FVector2D(0.0f, -1 * CurrentA));
		Canvas = Cast<UCanvasPanelSlot>(LineDown->Slot);
		Canvas->SetPosition(FVector2D(0.0f, CurrentA));
		//UE_LOG(LogTemp, Warning, TEXT("NotSpread"));

	
	}


}

void UCrosshair::ShowCrosshair(bool bIsShow)
{

	if (bIsShow) 
	{

		LineDown->SetVisibility(ESlateVisibility::Visible);
		LineUp->SetVisibility(ESlateVisibility::Visible);
		LineRight->SetVisibility(ESlateVisibility::Visible);
		LineLeft->SetVisibility(ESlateVisibility::Visible);

		return;
	}

	LineDown->SetVisibility(ESlateVisibility::Hidden);
	LineUp->SetVisibility(ESlateVisibility::Hidden);
	LineRight->SetVisibility(ESlateVisibility::Hidden);
	LineLeft->SetVisibility(ESlateVisibility::Hidden);


}
