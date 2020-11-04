// Please don't steal

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISightTargetInterface.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class DOUBLEAGENT_API ACamera : public AActor, public IAISightTargetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera();

	//Setter for blueprints
	UFUNCTION(BlueprintNativeEvent)
	void GetPerceptionLocationRotation(FVector& OutLocation, FRotator& OutRotation) const;

	//Overriding base function for perception
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	//Blueprint function to set center location
	UFUNCTION(BlueprintNativeEvent)
    FVector GetCenterLocation() const;
	
	// Overide base function to add socket locations to raycast
	UFUNCTION(BlueprintCallable)
    virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;
};
