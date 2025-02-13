#pragma once

#include "../Basic/SettingsWidget.h"
#include "SettingsProgressBarWidget.generated.h"

class UProgressBar;
class UAnalogSlider;

UCLASS()
class  USettingsProgressBarWidget final : public UiSettingsParentClass
{
	GENERATED_BODY()

public :

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;
	
	UPROPERTY(meta = (BindWidget),EditAnywhere,BlueprintReadWrite)
	UAnalogSlider* Slider;

#pragma endregion WidgetComponents

	virtual void InitWidget(UGameSettingsItem* NewSettingsItem) override;
	virtual void UpdateHUD() override;

protected:
	
	UFUNCTION()
	void OnValueChanged(float Value);
	
	virtual void NativeOnInitialized() override;
};