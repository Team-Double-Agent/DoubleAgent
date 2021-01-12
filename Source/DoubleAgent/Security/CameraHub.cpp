// Please don't steal


#include "CameraHub.h"
#include "DoubleAgent/Player_Character.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraHub::ACameraHub(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create the root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->SetRootComponent(Root);
	//Create the box collision and attach it to the root
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Root);
	//Resize the collision box
	BoxCollision->SetRelativeLocation(FVector(0,150,100));
	BoxCollision->SetRelativeScale3D(FVector(5, 8, 3.5));
	//Enable the collision on the object
	BoxCollision->SetGenerateOverlapEvents(true);
	SetActorEnableCollision(true);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACameraHub::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACameraHub::OnComponentOverlapEnd);

	//Set up table and stand static mesh and attach to the root component
	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableAndStand"));
	TableMesh->SetupAttachment(Root);

	UStaticMesh* Mesh;
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub.SM_CameraHub'"));
	TableMesh->SetStaticMesh(Mesh);
	
	//Set up all the screen static meshes and attach to the table component
	//Screen 001
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen1")));
	ScreenMeshes[0]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_001.SM_CameraHub_001'"));
	ScreenMeshes[0]->SetStaticMesh(Mesh);
	//Screen 002
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen2")));
	ScreenMeshes[1]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_002.SM_CameraHub_002'"));
	ScreenMeshes[1]->SetStaticMesh(Mesh);
	//Screen 003
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen3")));
	ScreenMeshes[2]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_003.SM_CameraHub_003'"));
	ScreenMeshes[2]->SetStaticMesh(Mesh);
	//Screen 004
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen4")));
	ScreenMeshes[3]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_004.SM_CameraHub_004'"));
	ScreenMeshes[3]->SetStaticMesh(Mesh);
	//Screen 005
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen5")));
	ScreenMeshes[4]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_005.SM_CameraHub_005'"));
	ScreenMeshes[4]->SetStaticMesh(Mesh);
	//Screen 006
	ScreenMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen6")));
	ScreenMeshes[5]->SetupAttachment(TableMesh);
	Mesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Art/Meshes/Security/CameraHub/SM_CameraHub_006.SM_CameraHub_006'"));
	ScreenMeshes[5]->SetStaticMesh(Mesh);
	//Increase scale so that the table is a more appropriate size
	//Rotate the CameraHub mesh so that it'll match with the collision box
	TableMesh->SetRelativeScale3D(FVector(1.3f,1.3f,1.3f));
	TableMesh->SetRelativeRotation(FRotator(0.0f, 90.0f,0.0f));
}

// Called when the game starts or when spawned
void ACameraHub::BeginPlay(){
	Super::BeginPlay();
	//Setup collision functions
	TArray<AActor*> tempCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACamera::StaticClass(), tempCameras); // Get all camera actors
	for(auto c : tempCameras){
		Cameras.Add(dynamic_cast<ACamera*>(c)); //Cast all camera actors into camera pointers and store them in an array
	}
	for(int i = 0; i < 6; i++){
		CameraAutoDefault.Add(Cameras[i]->bAutoRotate);
		Cameras[i]->CaptureComponent->TextureTarget = TextureTargets[i]; //Setting each camera to send their texture target to the associated target texture
		Cameras[i]->CaptureComponent->CaptureScene();
	}
}

// Called every frame
void ACameraHub::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ACameraHub::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(APlayer_Character::StaticClass())){
		if(bPowerOn){
			EnableCapture();
		}
	}
}

void ACameraHub::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(APlayer_Character::StaticClass())){
		DisableCapture();
	} else{
		//Chris AI stuff
		//Operator AI leaves CameraHub
	}
}

void ACameraHub::PowerEnabled(){
	for(auto c: Cameras){
		c->bPowerOn = true;
	}
	bPowerOn = true;
}

void ACameraHub::PowerDisabled(){
	for(auto c: Cameras){
		c->bPowerOn = false;
	}
	bPowerOn = false;
}

void ACameraHub::EnableCapture(){
	for(auto c: Cameras){
		c->SetCaptureEnabled(true);
	}
}

void ACameraHub::DisableCapture(){
	for(auto c: Cameras){
		c->SetCaptureEnabled(false);
	}
}
