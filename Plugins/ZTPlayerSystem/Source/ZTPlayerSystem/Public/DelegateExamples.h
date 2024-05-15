#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DelegateExamples.generated.h"

DECLARE_DELEGATE_TwoParams(FSimpleZTDelegate, FString, FVector)
DECLARE_MULTICAST_DELEGATE_OneParam(FMulticastZTDelegate, int32)
DECLARE_DYNAMIC_DELEGATE(FDynamicSimpleZTDelegate);


UCLASS()
class ZTPLAYERSYSTEM_API UDelegateExamples : public UObject
{
	GENERATED_BODY()

public:

	UDelegateExamples();
	
	FSimpleZTDelegate SimpleZTDelegate;
	FMulticastZTDelegate MulticastZTDelegate;

	FDynamicSimpleZTDelegate DynamicSimpleZTDelegate;
	
	void TriggerSimpleZTDelegate(FString Msg, FVector Value);
	void TriggerMulticastZTDelegate(int32 Value);

	void OnMulticastZTDelegateTriggered(int32 Value);

private:

	void OnSimpleZTDelegateTriggered(FString Msg, FVector Value);

	FDelegateHandle ZTDelegateTriggerHandle;

	UFUNCTION()
   	void HandleZTDynamicDelegate();
};
