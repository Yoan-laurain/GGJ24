#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabableObject.generated.h"

class AMyProjectCharacter;

UCLASS()
class GGJ24_API AGrabableObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AGrabableObject();

	UPROPERTY()
	AMyProjectCharacter* Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* GrabPoint;
	
};