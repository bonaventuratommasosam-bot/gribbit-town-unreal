#include "GribbitInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GribbitNeedsComponent.h"

UGribbitInteractionComponent::UGribbitInteractionComponent()
{
}

void UGribbitInteractionComponent::TryInteract()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = OwnerChar->GetActorLocation();
	FVector End = Start + OwnerChar->GetActorForwardVector() * InteractionRange;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);

	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// Basic interaction effect: restore some hunger when interacting
			if (UGribbitNeedsComponent* Needs = OwnerChar->FindComponentByClass<UGribbitNeedsComponent>())
			{
				Needs->ModifyNeed("Hunger", 15.f);
				UE_LOG(LogTemp, Log, TEXT("Interacted! Hunger restored."));
			}
		}
	}

	DrawDebugLine(World, Start, End, FColor::Green, false, 1.5f);
}
