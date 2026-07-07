#include "GribbitNeedsWidget.h"
#include "GribbitNeedsComponent.h"
#include "GribbitCharacter.h"
#include "GribbitPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UGribbitNeedsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BuildNativeLayout();
}

void UGribbitNeedsWidget::BuildNativeLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("NeedsRoot"));
	WidgetTree->RootWidget = Root;

	AddNeedRow(Root, FText::FromString(TEXT("Hunger")), BarHunger);
	AddNeedRow(Root, FText::FromString(TEXT("Energy")), BarEnergy);
	AddNeedRow(Root, FText::FromString(TEXT("Fun")), BarFun);
	AddNeedRow(Root, FText::FromString(TEXT("Social")), BarSocial);
	AddNeedRow(Root, FText::FromString(TEXT("Hygiene")), BarHygiene);
	AddNeedRow(Root, FText::FromString(TEXT("Bladder")), BarBladder);
}

void UGribbitNeedsWidget::AddNeedRow(UVerticalBox* Root, const FText& Label, TObjectPtr<UProgressBar>& OutBar)
{
	if (!WidgetTree || !Root)
	{
		return;
	}

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	OutBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());

	Text->SetText(Label);
	Text->SetMinDesiredWidth(90.f);
	OutBar->SetPercent(1.f);

	UHorizontalBoxSlot* TextSlot = Row->AddChildToHorizontalBox(Text);
	TextSlot->SetPadding(FMargin(4.f, 2.f));

	UHorizontalBoxSlot* BarSlot = Row->AddChildToHorizontalBox(OutBar);
	BarSlot->SetPadding(FMargin(4.f, 2.f));
	BarSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	UVerticalBoxSlot* RowSlot = Root->AddChildToVerticalBox(Row);
	RowSlot->SetPadding(FMargin(12.f, 3.f));
}

bool UGribbitNeedsWidget::TryBindLocalNeeds()
{
	AGribbitPlayerController* PC = GetOwningPlayer<AGribbitPlayerController>();
	if (!PC) return false;

	AGribbitCharacter* Char = Cast<AGribbitCharacter>(PC->GetPawn());
	if (!Char) return false;

	BoundNeeds = Char->FindComponentByClass<UGribbitNeedsComponent>();
	return BoundNeeds != nullptr;
}

void UGribbitNeedsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!BoundNeeds && !TryBindLocalNeeds())
	{
		return;
	}

	OnNeedsUpdated(
		BoundNeeds->Hunger,
		BoundNeeds->Energy,
		BoundNeeds->Fun,
		BoundNeeds->Social,
		BoundNeeds->Hygiene,
		BoundNeeds->Bladder);
	UpdateNativeBars();
}

void UGribbitNeedsWidget::UpdateNativeBars() const
{
	if (!BoundNeeds)
	{
		return;
	}

	if (BarHunger) BarHunger->SetPercent(BoundNeeds->Hunger / 100.f);
	if (BarEnergy) BarEnergy->SetPercent(BoundNeeds->Energy / 100.f);
	if (BarFun) BarFun->SetPercent(BoundNeeds->Fun / 100.f);
	if (BarSocial) BarSocial->SetPercent(BoundNeeds->Social / 100.f);
	if (BarHygiene) BarHygiene->SetPercent(BoundNeeds->Hygiene / 100.f);
	if (BarBladder) BarBladder->SetPercent(BoundNeeds->Bladder / 100.f);
}
