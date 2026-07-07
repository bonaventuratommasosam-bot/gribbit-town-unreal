#include "GribbitInteractionComponent.h"
#include "GribbitInteractable.h"
#include "GribbitCharacter.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

UGribbitInteractionComponent::UGribbitInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGribbitInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only the locally controlled client runs focus tracing.
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
	{
		UpdateFocus();
	}
}

void UGribbitInteractionComponent::UpdateFocus()
{
	AGribbitCharacter* Char = Cast<AGribbitCharacter>(GetOwner());
	if (!Char) return;

	FVector Start = Char->GetActorLocation();
	FRotator ViewRot;
	Char->GetActorEyesViewPoint(Start, ViewRot);
	// Use camera location if available for accuracy.
	if (UCameraComponent* Camera = Char->GetFollowCamera())
	{
		Start = Camera->GetComponentLocation();
	}

	const FVector End = Start + ViewRot.Vector() * TraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), true, Char);
	Params.AddIgnoredActor(Char);

	FHitResult Hit;
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit, Start, End, FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TraceRadius),
		Params);

	AGribbitInteractable* NewFocus = nullptr;
	if (bHit && Hit.GetActor())
	{
		if (AGribbitInteractable* Inter = Cast<AGribbitInteractable>(Hit.GetActor()))
		{
			const float Dist = FVector::Dist(Char->GetActorLocation(), Inter->GetActorLocation());
			if (Inter->bEnabled && Dist <= Inter->InteractionRange && Inter->CanInteract(Char))
			{
				NewFocus = Inter;
			}
		}
	}

	if (NewFocus != CurrentFocus)
	{
		CurrentFocus = NewFocus;
		OnFocusChanged.Broadcast(CurrentFocus);
	}
}

void UGribbitInteractionComponent::TryInteract()
{
	// Run on the server: only the owning client sets focus, but interaction
	// must be authoritative.
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		// Forward to server via the character if needed; for listen-server this is fine.
	}

	if (CurrentFocus && CurrentFocus->CanInteract(Cast<AGribbitCharacter>(GetOwner())))
	{
		CurrentFocus->OnInteract(Cast<AGribbitCharacter>(GetOwner()));
	}
}
