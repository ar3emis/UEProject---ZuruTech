// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncScatterTask.h"
#include "ScatterActor.h"
#include "Async/ParallelFor.h"

FAsyncScatterTask::FAsyncScatterTask(AScatterActor* InScatterActor)
{
	ScatterActor = InScatterActor;
}

void FAsyncScatterTask::DoWork()
{
	if(ScatterActor)
	{
		if(UScatterDataAsset* DataAsset = ScatterActor->DataAsset.Get())
		{
			TArray<UStaticMesh*> StaticMeshes = DataAsset->StaticMeshes;

			// ParallelFor(100, [](int32 ThreadIndex)
			// {
			// 	
			// });
			//

			
			
			for(int jIndex = 0; jIndex < StaticMeshes.Num(); jIndex++)
			{
				//int32 RandomMeshIndex = FMath::RandRange(0, StaticMeshes.Num() - 1);
				UStaticMesh* CurrentMesh = StaticMeshes[jIndex];

				
				
				for(int iIndex = 0; iIndex < ScatterActor->NumberOfInstances; iIndex++)
				{
				
					FVector BoundingExtent = ScatterActor->BoundingVolume->GetScaledBoxExtent();
					FVector Origin = ScatterActor->BoundingVolume->GetComponentLocation();
					FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

					FVector Position = FMath::RandPointInBox(BoundingBox);
					//InstanceTransforms.Add(FTransform(Position));

					TArray<FTransform> InstanceTransforms;
					InstanceTransforms.Add(FTransform(Position));
					ScatterActor->AddInstances(CurrentMesh, InstanceTransforms);
				}

				
			}
			
		}

		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			ScatterActor->FinishScatter();
		});
	}
}
