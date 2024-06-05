// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AScatterActor;

/**
 * 
 */
class PROCEDURALOBJECTSCATTER_API FAsyncScatterTask : public FNonAbandonableTask
{
public:

	FAsyncScatterTask(AScatterActor* InScatterActor);
	
	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncScatterTask, STATGROUP_ThreadPoolAsyncTasks);
	}

private:

	AScatterActor* ScatterActor;
};
