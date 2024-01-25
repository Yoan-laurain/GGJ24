#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerMappableInputConfig.h" // KEEP THIS !! 
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "..\UI\Settings\Category\Bindings\Configuration\CustomInputComponent.h"
#include "GGJ24/GrabableObject.h"
#include "GGJ24/UI/Settings/LocalSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserSettings/EnhancedInputUserSettings.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter() :
	RadiusGrab(500.f),
	DebugMode(false),
	PlayerId(0),
	IsCarrying(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (InputComponent != nullptr)
	{
		InitializePlayerInput(InputComponent);
	}
}

void AMyProjectCharacter::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	
	for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
	{
		if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
		{
			if (Mapping.bRegisterWithSettings)
			{
				if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
				{
					Settings->RegisterInputMappingContext(IMC);
				}
				
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				// Actually add the config to the local player							
				Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
			}
		}
	}
	
	ULocalSettings::Get()->RegisterInputMappingContextsForLocalPlayer( LP );
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
		
		//Escape
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Escape);

		//Grab
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &AMyProjectCharacter::TryGrabObject);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &AMyProjectCharacter::ReleaseObject);
		 
	}
}

void AMyProjectCharacter::TryGrabObject()
{
	TArray<FHitResult> OutHits;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FVector Center = GetActorLocation() + GetActorForwardVector() * 50.f;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Center, Center, RadiusGrab,
	                                       UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
	                                       EDrawDebugTrace::None, OutHits, true);

	if ( DebugMode )
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Center, RadiusGrab, 12, FLinearColor::Red, 5.f, 1.f);
	}

	for (FHitResult Hit : OutHits)
	{
		if ( Hit.GetActor()->IsA(AGrabableObject::StaticClass()) )
		{
			if (AGrabableObject* GrabableObject = Cast<AGrabableObject>(Hit.GetActor()))
			{
				GrabbedObject = GrabableObject;
				

				GrabbedObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabSocket"));
				GrabbedObject->SetActorLocation(GrabbedObject->GrabPoint->GetComponentLocation());
				
				
				IsCarrying = true;
				break;
			}
		}
	}
}

void AMyProjectCharacter::ReleaseObject()
{
	if ( GrabbedObject )
	{
		GrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedObject = nullptr;
		IsCarrying = false;
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
