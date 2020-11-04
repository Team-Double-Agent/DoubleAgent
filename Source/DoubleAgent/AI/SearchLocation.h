// Please don't steal

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SearchLocation.generated.h"

class ARoomVolume;

UCLASS()
class DOUBLEAGENT_API ASearchLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	//Constructor
	ASearchLocation();

	//Room reference
	TArray<ARoomVolume*> RoomVolumes;
};
