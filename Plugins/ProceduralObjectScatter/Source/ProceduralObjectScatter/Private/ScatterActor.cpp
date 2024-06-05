#include "ScatterActor.h"

#include "Misc/ScopedSlowTask.h"

// Sets default values
AScatterActor::AScatterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingVolume"));
	BoundingVolume->SetBoxExtent(FVector(1000));
	RootComponent = BoundingVolume;
}

// Called when the game starts or when spawned
void AScatterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScatterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(DataAsset)
	{
		UDataAsset* dAsset = DataAsset.Get();
		
	}
}

void AScatterActor::ScatterObjects()
{
	for(auto& Pair : HISMComponents)
	{
		if(UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{
			HISM->ClearInstances();
		}
	}

	if(DataAsset)
	{
		SlowTask = new FScopedSlowTask(NumberOfInstances * DataAsset->StaticMeshes.Num(), FText::FromString("Scattering Objects..." ));
		SlowTask->MakeDialog(true);
	}

	if(AsyncScatterTask && !AsyncScatterTask->IsDone())
	{
		AsyncScatterTask->EnsureCompletion();
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}
	
	AsyncScatterTask = new FAsyncTask<FAsyncScatterTask>(this);
	AsyncScatterTask->StartBackgroundTask();
}

void AScatterActor::AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms)
{
	UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
	if(HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
	{
		AsyncTask(ENamedThreads::GameThread, [this, HISMCPtr, StaticMesh, Transforms]()
		{
		   (*HISMCPtr)->AddInstances(Transforms, false);
			if(SlowTask)
			{
				SlowTask->EnterProgressFrame(Transforms.Num(), FText::FromString("Scattering Mesh : " + StaticMesh->GetName()));
			}
		});
	}
	else
	{
		UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
		NewHISMC->SetStaticMesh(StaticMesh);

		HISMComponents.Add(StaticMesh, NewHISMC);

		AsyncTask(ENamedThreads::GameThread, [this, NewHISMC, StaticMesh, Transforms]()
		{
			NewHISMC->RegisterComponentWithWorld(GetWorld());
		    NewHISMC->AddInstances(Transforms, false);
			if(SlowTask)
			{
				SlowTask->EnterProgressFrame(Transforms.Num(), FText::FromString("Scattering Mesh : " + StaticMesh->GetName()));
			}
		});

		
		
	}
}

void AScatterActor::FinishScatter()
{
	if(SlowTask)
	{
		SlowTask->Destroy();	
	}
	
}

