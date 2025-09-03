

#include "Widget/ReloadWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

void UReloadWidget::ShowReload(bool bIsShow)
{
	if (bIsShow)
	{
	
		Circle->SetVisibility(ESlateVisibility::Visible);
		

		return;
	}


	Circle->SetVisibility(ESlateVisibility::Hidden);




}
