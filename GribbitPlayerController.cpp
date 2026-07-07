#include "GribbitPlayerController.h"
#include "GribbitCharacter.h"
#include "GribbitInteractionComponent.h"
#include "GribbitNeedsWidget.h"
#include "Blueprint/UserWidget.h"

void AGribbitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreateNeedsHUD();
	}
}

void AGribbitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &AGribbitPlayerController::HandleInteractInput);
}

void AGribbitPlayerController::CreateNeedsHUD()
{
	if (NeedsWidgetClass)
	{
		NeedsWidget = CreateWidget<UGribbitNeedsWidget>(this, NeedsWidgetClass);
		if (NeedsWidget)
		{
			NeedsWidget->AddToViewport();
		}
	}
}

void AGribbitPlayerController::HandleInteractInput()
{
	AGribbitCharacter* Char = GetPawn<AGribbitCharacter>();
	if (!Char) return;

	if (UGribbitInteractionComponent* IC = Char->FindComponentByClass<UGribbitInteractionComponent>())
	{
		IC->TryInteract();
	}
}
