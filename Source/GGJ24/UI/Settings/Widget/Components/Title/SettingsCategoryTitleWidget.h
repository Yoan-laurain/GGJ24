#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SettingsCategoryTitleWidget.generated.h"

UCLASS()
class  USettingsCategoryTitleWidget final : public UUserWidget
{
	GENERATED_BODY()

public :

	UPROPERTY( meta = (BindWidget) )
	UTextBlock* TitleTextBlock;
	
	void SetTitle(const FText& NewTitle) const { TitleTextBlock->SetText(NewTitle); }

protected :
	FText Title;
};