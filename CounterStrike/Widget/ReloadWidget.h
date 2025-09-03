
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReloadWidget.generated.h"

/**
 * 
 */
UCLASS()
class COUNTERSTRIKE_API UReloadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// -------------Variable ------------
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* Circle;


	// -------------Function ------------
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationWidget();

	void ShowReload(bool bIsShow);


};
