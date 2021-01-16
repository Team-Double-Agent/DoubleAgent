// Please don't steal

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "DoubleAgent/Door.h"
#include "DoubleAgent/Radio.h"
#include "DoubleAgent/AI/RoomVolume.h"
#include "StaffAIController.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "ActionStatus", ToolTip = "Special actions staff can preform, most actions have a limit on the amount of staff that can be performing said action"))
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
	//Investigating source of noise that was heard (at or above 40)
    Action_NoiseInvestigation		UMETA(DisplayName = "NoiseInvestigation"),
	//Searching building for player (above or at 90)
    Action_Searching		UMETA(DisplayName = "Searching"),
	//Searching the closest unreached search location (below 40)
    Action_SearchInvestigation		UMETA(DisplayName = "SearchInvestigation"),
	//Generic cleaning task
	Action_Cleaning UMETA(DisplayName = "Cleaning"),
	//Getting ammo from armory
	Action_Restock UMETA(DisplayName = "Restocking"),
	//Reloading
	Action_Reloading UMETA(DisplayName = "Reloading"),
	//Using the washroom
	Action_Washroom UMETA(DisplayName = "Washroom")
};

USTRUCT(BlueprintType)
struct FTrackedActor
{
	GENERATED_BODY()

	FTrackedActor(){};
	FTrackedActor(AActor* Actor_, FVector Location_, float Detection_);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Detection;
};

USTRUCT(BlueprintType)
struct FTrackedCamera
{
	GENERATED_BODY()

	FTrackedCamera(){};
	FTrackedCamera(ACamera* Camera_);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACamera* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoRotate;	
};

USTRUCT(BlueprintType)
struct FTrackedDoor
{
	GENERATED_BODY()
	
	FTrackedDoor(){};
	FTrackedDoor(ADoor* Door_);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADoor* Door;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDoorOpen;
};

//Memory structure used to hold all important actors
USTRUCT(BlueprintType)
struct FStaffMemory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTrackedActor> Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTrackedCamera> Cameras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTrackedDoor> Doors;
};

UCLASS()
class DOUBLEAGENT_API AStaffAIController : public AAIControllerBase
{
	GENERATED_BODY()

	//Override
	virtual void Tick(float DeltaTime) override;
	void OnPossess(APawn* InPawn) override;
	
	//Detection fall off
	void DetectionDecay(float DeltaTime);
	
public:
	//Staff memory of specific actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStaffMemory Memory;
	
	//NPC spotted tick
	virtual void NPCVisionTick(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;
	void StaffVisionTick(AActor* CurrentActor, FAIStimulus& CurrentStimulus);
	void StaffVisionUpdate(AActor* CurrentActor, FAIStimulus& CurrentStimulus);

	//Process hearing perception
	virtual bool HandleHearing(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;

	//Process sight perception
	virtual void HandleSightTick(AActor* CurrentActor, FAIStimulus& CurrentStimulus, float DeltaTime) override;
	virtual void HandleSight(AActor* CurrentActor, FAIStimulus& CurrentStimulus) override;
	
	//Player spotted 
	virtual void PlayerVisionTick(AActor* CurrentPlayer, FAIStimulus& CurrentStimulus, float DeltaTime);
	virtual void PlayerVisionUpdate(AActor* CurrentPlayer, FAIStimulus& CurrentStimulus);

	//Camera
	virtual void CameraVisionUpdate(AActor* CurrentActor, FAIStimulus& CurrentStimulus);

	//Door
	virtual void DoorVisionUpdate(AActor* CurrentActor, FAIStimulus& CurrentStimulus);
	
	//Radio event
	void HandleRadioEvent(FRadioEvent RadioEvent);
	
	//Percent change per second
	UPROPERTY(EditAnywhere)
	float DetectionRate;

	//Searched locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASearchLocation*> SearchedLocations;

	//Helper functions
	UFUNCTION(BlueprintCallable)
	void MarkSearchLocationSearched(ASearchLocation* SearchLocation);

	//Base overrides
	virtual void BeginPlay() override;
};
