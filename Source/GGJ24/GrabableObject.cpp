#include "GrabableObject.h"

#include "Components/BoxComponent.h"
#include "Player/MyProjectCharacter.h"

AGrabableObject::AGrabableObject()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	GrabPoint->SetupAttachment(BoxComponent);

}