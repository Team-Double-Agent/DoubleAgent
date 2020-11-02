// Please don't steal

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "DoubleAgent/AI/RoomVolume.h"
#include "StaffAIController.generated.h"

UENUM(BlueprintType, meta = (DisplayName="ActionStatus", ToolTip="Special actions staff can preform, most actions have a limit on the amount of staff that can be performing said action"))
enum class EActionStatus : uint8
{
	//No actions are being performed
	Action_Idle		UMETA(DisplayName = "Idle"),
	//Going to turn back on an lightswitch
	Action_LightSwitch		UMETA(DisplayName = "LightSwitch"),
	//Going to man the camera hub
    Action_CameraHub		UMETA(DisplayName = "CameraHub"),
	//Going to turn back on the power box
    Action_PowerBox		UMETA(DisplayName = "PowerBox"),
	//Going to revive an npc
    Action_Revive		UMETA(DisplayName = "Revive"),
	//Calling for backup on landline
    Action_Backup		UMETA(DisplayName = "Backup"),
	//Patting down player due to suspicious clothing
    Action_BodySearch		UMETA(DisplayName = "BodySearch"),
	//Investigating source of noise that was heard
    Action_NoiseInvestigation		UMETA(DisplayName = "NoiseInvestigation"),
	//Searching building for player
    Action_Searching		UMETA(DisplayName = "Searching"),
	//Searching the closest unreached search location
    Action_SearchInvestigation		UMETA(DisplayName = "SearchInvestigation"),
};

//A tracked player is any player that has been considered a threat
USTRUCT()
struct FTrackedPlayer
{
	GENERATED_BODY()

	FTrackedPlayer(){}
	FTrackedPlayer(AActor* Actor_, FVector Location_, float Detection_);
	UPROPERTY(EditAnywhere)
	AActor* Actor;
	UPROPERTY(EditAnywhere)
	FVector Location;
	UPROPERTY(EditAnywhere)
	float Detection;
};

//Memory structure used to hold all important actors
USTRUCT()
struct FStaffMemory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FTrackedPlayer> Players;
};

UCLASS()
class DOUBLEAGENT_API AStaffAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	//NPC spotted tick
	virtual void NPCVisionTick(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;

	//Process hearing perception
	virtual bool HandleHearing(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;

	//Process sight perception
	virtual void HandleSightTick(AActor* CurrentActor, FAIStimulus& CurrentStimulus, float DeltaTime) override;
	virtual void HandleSight(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;
	
	//Player spotted 
	virtual void PlayerVisionTick(AActor* CurrentPlayer, FAIStimulus& CurrentStimulus, float DeltaTime);
	virtual void PlayerVisionUpdate(AActor* CurrentPlayer, FAIStimulus& CurrentStimulus);

	//Percent change per second
	UPROPERTY(EditAnywhere)
	float DetectionRate;

	//Searched locations
	UPROPERTY(EditAnywhere)
	TArray<ASearchLocation*> SearchedLocations;

	//Helper functions
	void MarkSearchLocationSearched(ASearchLocation* SearchLocation);
	
private:
	//Staff memory of specific actors
	UPROPERTY(EditAnywhere)
	FStaffMemory Memory;

	//Override
	virtual void Tick(float DeltaTime) override;

	//Detection fall off
	void DetectionDecay(float DeltaTime);
};
