#pragma once

#include "GGJ24/UI/Settings/Category/GameSettingsCollection.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "CustomSettingKeyboardInput.generated.h"

class UObject;

UCLASS()
class UCustomSettingKeyboardInput : public UGameSettingsItem
{
	GENERATED_BODY()

public:
	UCustomSettingKeyboardInput();

	void InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& QueryOptions);

	FText GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const;
	
	virtual void StoreInitial() ;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial();
	virtual void CancelChanges() override;
	virtual void SetInitialIndex(const int Value) override;

	bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey);
	void GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const;
	TArray<UCustomSettingKeyboardInput*> GetAllMappedItemFromKey(TArray<FName>& OutActionNames);
	
	FText GetSettingDisplayName() const;
	FText GetSettingDisplayCategory() const;

	const FKeyMappingRow* FindKeyMappingRow() const;
	UEnhancedPlayerMappableKeyProfile* FindMappableKeyProfile() const;
	UEnhancedInputUserSettings* GetUserSettings() const;

	virtual bool ValueHasChangedCompareToStart() override;
	virtual bool ValueHasChangedCompareToDefault() override;

	void SetIsKeyboard(bool InIsKeyboard) { isKeyboard = InIsKeyboard; }
	bool GetIsKeyboard() const { return isKeyboard; }

	void RegisterNewBinding(const FKey& InKey);
	
protected:

	bool isKeyboard;
	bool HasChanged;

protected:

	/** The name of this action's mappings */
	FName ActionMappingName;

	/** The query options to filter down keys on this setting for */
	FPlayerMappableKeyQueryOptions QueryOptions;

	/** The profile identifier that this key setting is from */
	FGameplayTag ProfileIdentifier;

	/** Store the initial key mappings that are set on this for each slot */
	TMap<EPlayerMappableKeySlot, FKey> InitialKeyMappings;
};
