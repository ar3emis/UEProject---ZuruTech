// Fill out your copyright notice in the Description page of Project Settings.


#include "FlycamPawn.h"


#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFlycamPawn::AFlycamPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(90);
	CapsuleComponent->SetCapsuleRadius(40);
	CapsuleComponent->SetEnableGravity(false);
	RootComponent = CapsuleComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

// Called when the game starts or when spawned
void AFlycamPawn::BeginPlay()
{
	Super::BeginPlay();

	DelegateExamples = NewObject<UDelegateExamples>();

	PawnMovementDelegate.AddDynamic(this, &AFlycamPawn::HandleZTDynamicDelegate);
}

// Called every frame
void AFlycamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFlycamPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis3D;

		InputMappingContext->MapKey(MoveAction, EKeys::W);

		UInputModifierNegate* MoveActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;
		
		FEnhancedActionKeyMapping& MoveActionBackwardKeyMapping =  InputMappingContext->MapKey(MoveAction, EKeys::S);
		MoveActionBackwardKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		UInputModifierSwizzleAxis* YXZSiwzzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
		YXZSiwzzleAxisModifier->Order = EInputAxisSwizzle::YXZ;

		FEnhancedActionKeyMapping& MoveActionRightKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::D);
		MoveActionRightKeyMapping.Modifiers.Add(YXZSiwzzleAxisModifier);
		
		FEnhancedActionKeyMapping& MoveActionLeftKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		MoveActionLeftKeyMapping.Modifiers.Add(YXZSiwzzleAxisModifier);
		MoveActionLeftKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		UInputModifierSwizzleAxis* ZYXSiwzzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
		ZYXSiwzzleAxisModifier->Order = EInputAxisSwizzle::ZYX;
		
		FEnhancedActionKeyMapping& MoveActionUpKeyMapping =  InputMappingContext->MapKey(MoveAction, EKeys::E);
		MoveActionUpKeyMapping.Modifiers.Add(ZYXSiwzzleAxisModifier);
		
		FEnhancedActionKeyMapping& MoveActionDownKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::Q);
		MoveActionDownKeyMapping.Modifiers.Add(ZYXSiwzzleAxisModifier);
		MoveActionDownKeyMapping.Modifiers.Add(MoveActionNegateModifier);
		
		UInputAction* LookAction = NewObject<UInputAction>();
		LookAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>();
		LookActionMouseNegateModifier->bX = false;
		LookActionMouseNegateModifier->bY = true;
		LookActionMouseNegateModifier->bZ = false;
		
		FEnhancedActionKeyMapping& LookActionMouseMapping =  InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
		LookActionMouseMapping.Modifiers.Add(LookActionMouseNegateModifier);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFlycamPawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlycamPawn::Look);
		
		if(const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if(UEnhancedInputLocalPlayerSubsystem* SubSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
		 
	}
}

void AFlycamPawn::Move(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();
	FRotator Rotation = Controller->GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

	UKismetMathLibrary::GetForwardVector(Rotation);
	
	AddMovementInput(ForwardDirection, MovementInput.X);
	AddMovementInput(RightDirection, MovementInput.Y);
	AddMovementInput(UpDirection, MovementInput.Z);

	//OnPawnMoved(MovementInput);

	if(DelegateExamples)
	{
		DelegateExamples->TriggerSimpleZTDelegate("Movement In Progress", MovementInput);
		DelegateExamples->DynamicSimpleZTDelegate.ExecuteIfBound();
	}

	PawnMovementDelegate.Broadcast(MovementInput);

}


void AFlycamPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}



