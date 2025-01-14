// Please don't steal

#include "HouseLight.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AHouseLight::AHouseLight()
{
	RootComponent->SetMobility(EComponentMobility::Stationary);

	//Setup static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetMobility(EComponentMobility::Stationary);
	StaticMesh->CastShadow = false;
	StaticMesh->SetupAttachment(RootComponent);

	//Network replication
	bReplicates = true;
}

void AHouseLight::UpdateLight()
{
	//Update mesh
	UStaticMesh* Mesh;
	switch (MeshType)
	{
		case ELightMesh::Mesh_Ceiling:
			{
				Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Building/HouseLights/Combined/SM_House_Light_002.SM_House_Light_002'"));
				StaticMesh->SetStaticMesh(Mesh);
				StaticMesh->SetRelativeRotation(FRotator(-180, 0, 0), false);
			}
		break;
		case ELightMesh::Mesh_Ceiling_Large:
			{
				Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Building/HouseLights/Combined/SM_House_Light_001.SM_House_Light_001'"));
				StaticMesh->SetStaticMesh(Mesh);
				StaticMesh->SetRelativeRotation(FRotator(-180, 0, 0), false);
			}
		break;
		case ELightMesh::Mesh_Wall:
			{
				Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Building/HouseLights/Combined/SM_House_Light_003.SM_House_Light_003'"));
				StaticMesh->SetStaticMesh(Mesh);
				StaticMesh->SetRelativeRotation(FRotator(0, 180, 0), false);	
			}
		break;
	}

	//Cleanup old light if any
	if (Light != nullptr)
	{
		Light->DestroyComponent();
		Light = nullptr;
	}
	
	//Update light
	switch (LightType)
	{
		case ELightType::Light_Point:
			{
				Light = NewObject<UPointLightComponent>(this, UPointLightComponent::StaticClass());
				Light->RegisterComponent();
				Light->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Light->SetMobility(EComponentMobility::Movable);
				Light->SetRelativeLocation(UKismetMathLibrary::MakeRelativeTransform(FTransform(StaticMesh->GetSocketLocation(FName("Light"))), StaticMesh->GetComponentTransform()).GetTranslation());
				UPointLightComponent* PointLight = Cast<UPointLightComponent>(Light);
				PointLight->Intensity = 2.0f;
				PointLight->AttenuationRadius = 700.0f;
				PointLight->bUseInverseSquaredFalloff = false;
				PointLight->LightFalloffExponent = 1.0;
				PointLight->ShadowSharpen = 0.1;
			}
		break;
		case ELightType::Light_Rect:
			{
				Light = NewObject<URectLightComponent>(this, URectLightComponent::StaticClass());
				Light->RegisterComponent();
				Light->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Light->SetMobility(EComponentMobility::Movable);
				Light->SetRelativeLocation(UKismetMathLibrary::MakeRelativeTransform(FTransform(StaticMesh->GetSocketLocation(FName("Light"))), StaticMesh->GetComponentTransform()).GetTranslation());
				Light->SetRelativeRotation(FRotator(-90, 0, 0), false);				
				URectLightComponent* RectLight = Cast<URectLightComponent>(Light);
				RectLight->Intensity = 22500.0f;
				RectLight->AttenuationRadius = 750.0f;
				RectLight->SourceWidth = RectLight->SourceHeight = 50.0f;
				RectLight->BarnDoorAngle = RectLight->BarnDoorLength = 0.0f;
				RectLight->ShadowSharpen = 0.1;
			}
		break;
		case ELightType::Light_Spot:
			{
				Light = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
				Light->RegisterComponent();
				Light->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Light->SetMobility(EComponentMobility::Movable);
				Light->SetRelativeLocation(UKismetMathLibrary::MakeRelativeTransform(FTransform(StaticMesh->GetSocketLocation(FName("Light"))), StaticMesh->GetComponentTransform()).GetTranslation());
				if (MeshType != ELightMesh::Mesh_Wall)
					Light->SetRelativeRotation(FRotator(90, 0, 0), false);
				USpotLightComponent* SpotLight = Cast<USpotLightComponent>(Light);
				SpotLight->Intensity = 3.0;
				SpotLight->AttenuationRadius = 1000.0f;
				SpotLight->InnerConeAngle = 60.0f;
				SpotLight->OuterConeAngle = 90.0f;
				SpotLight->bUseInverseSquaredFalloff = false;
				SpotLight->LightFalloffExponent = 1.0;
				SpotLight->ShadowSharpen = 0.1;
			}
		break;
	}

	UpdateSphere();
}

void AHouseLight::TurnOn_Implementation()
{
	Light->SetVisibility(true);
	UMaterial* Material = LoadObject<UMaterial>(NULL, TEXT("Material'/Game/Art/Meshes/Building/HouseLights/Materials/MI_Lights_Emissive.MI_Lights_Emissive'"));
	StaticMesh->SetMaterial(0, Material);
}

void AHouseLight::TurnOff_Implementation()
{
	Light->SetVisibility(false);
	UMaterial* Material = LoadObject<UMaterial>(NULL, TEXT("Material'/Engine/EngineDebugMaterials/BlackUnlitMaterial.BlackUnlitMaterial'"));
	StaticMesh->SetMaterial(0, Material);
}
