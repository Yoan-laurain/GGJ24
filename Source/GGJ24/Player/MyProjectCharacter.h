#pragma once

#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyProjectCharacter.generated.h"

struct FInputMappingContextAndPriority;
class UCustomInputConfig;
struct FGameplayTag;
class UInputAction;

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Inputs
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EscapeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

#pragma endregion Inputs

public:
	AMyProjectCharacter();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float RadiusGrab;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool DebugMode;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int PlayerId;

	UFUNCTION(BlueprintCallable)
	void TryGrabObject();

	UFUNCTION(BlueprintCallable)
	void ReleaseObject();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class AGrabableObject* GrabbedObject;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool IsCarrying;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void Escape();

	UPROPERTY(EditAnywhere)
	TArray<FInputMappingContextAndPriority> DefaultInputMappings;
	
	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	TObjectPtr<UCustomInputConfig> InputConfig;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	
	virtual void BeginPlay() override;
};

