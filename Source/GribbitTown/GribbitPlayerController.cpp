#include "GribbitPlayerController.h"
#include "GribbitCharacter.h"
#include "GribbitInteractionComponent.h"
#include "GribbitNeedsWidget.h"
#include "GribbitVehicle.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AGribbitPlayerController::AGribbitPlayerController()
{
	static ConstructorHelpers::FClassFinder<UGribbitNeedsWidget> NeedsWidgetBP(TEXT("/Game/UI/WBP_NeedsBars"));
	if (NeedsWidgetBP.Succeeded())
	{
		NeedsWidgetClass = NeedsWidgetBP.Class;
	}
}

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
	InputComponent->BindAction("EnterVehicle", IE_Pressed, this, &AGribbitPlayerController::HandleEnterVehicleInput);
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

void AGribbitPlayerController::HandleEnterVehicleInput()
{
	AGribbitCharacter* Char = GetPawn<AGribbitCharacter>();
	if (!Char)
	{
		// Already driving: the car itself handles getting out.
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector From = Char->GetActorLocation();
	AGribbitVehicle* Nearest = nullptr;
	float NearestDistSq = VehicleBoardingRange * VehicleBoardingRange;

	for (TActorIterator<AGribbitVehicle> It(World); It; ++It)
	{
		AGribbitVehicle* Car = *It;
		if (!Car || Car->Driver)
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(From, Car->GetActorLocation());
		if (DistSq <= NearestDistSq)
		{
			NearestDistSq = DistSq;
			Nearest = Car;
		}
	}

	if (Nearest)
	{
		Nearest->EnterVehicle(Char);
	}
}
