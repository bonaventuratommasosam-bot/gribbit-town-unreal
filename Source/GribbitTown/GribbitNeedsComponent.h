#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GribbitNeedsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitNeedsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitNeedsComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Needs (replicated)
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Hunger = 80.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Energy = 80.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Fun = 70.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Social = 60.f;

	UFUNCTION(BlueprintCallable, Category = "Needs")
	void ModifyNeed(FName NeedName, float Amount);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};