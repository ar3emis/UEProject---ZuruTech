// Fill out your copyright notice in the Description page of Project Settings.


#include "DelegateExamples.h"

UDelegateExamples::UDelegateExamples()
{
	
	SimpleZTDelegate.BindUObject(this, &UDelegateExamples::OnSimpleZTDelegateTriggered);

	MulticastZTDelegate.AddLambda([](int32 Value)
	{
		UE_LOG(LogTemp, Warning, TEXT("MulticastZTDelegate says Value is %s"), *FString::FromInt(Value));
	});

	
	ZTDelegateTriggerHandle = MulticastZTDelegate.AddUObject(this, &UDelegateExamples::OnMulticastZTDelegateTriggered);

	if(!DynamicSimpleZTDelegate.IsBound())
	{
		DynamicSimpleZTDelegate.BindDynamic(this, &UDelegateExamples::HandleZTDynamicDelegate);
	}
}

void UDelegateExamples::TriggerSimpleZTDelegate(FString Msg, FVector Value)
{
	if(SimpleZTDelegate.IsBound())
	{
		SimpleZTDelegate.Execute(Msg, Value);
	}
	
}

void UDelegateExamples::TriggerMulticastZTDelegate(int32 Value)
{
	if(MulticastZTDelegate.IsBound())
	{
		MulticastZTDelegate.Broadcast(Value);
	}
	
	MulticastZTDelegate.Remove(ZTDelegateTriggerHandle);
}

void UDelegateExamples::OnSimpleZTDelegateTriggered(FString Msg, FVector Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SimpleZTDelegate says %s. Value is %s"), *Msg, *Value.ToString());
}

void UDelegateExamples::HandleZTDynamicDelegate()
{
	UE_LOG(LogTemp, Warning, TEXT("DynamicDelegate is Executred"));
}

void UDelegateExamples::OnMulticastZTDelegateTriggered(int32 Value)
{
}
