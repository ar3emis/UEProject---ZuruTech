#include "ZTPlayerController.h"
#include "Misc/AutomationTest.h"
#include "SubSystems/ZTGameInstanceSubsystem.h"
#include "Tests/AutomationCommon.h"


UWorld* GetTestWorld()
{
	const FString MapName = "/Game/Maps/TestLevel.TestLevel";
	AutomationOpenMap(MapName);

	UWorld* TestWorld = AutomationCommon::GetAnyGameWorld();
	if(!TestWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get Test World."));
		
	}

	return TestWorld;
}

UGameInstance* GetGameInstance()
{
	if(UWorld* TestWorld = GetTestWorld())
	{
		if(UGameInstance* GameInstance = TestWorld->GetGameInstance())
		{
			return  GameInstance;
		}

		UE_LOG(LogTemp, Warning, TEXT("Failed to get Game Instance."));
	}

	return nullptr;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(ZTSaveLoadComplexTest, "ZTProject.ZTSaveLoadComplexTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FZTCreateSaveSlotCommand, FString, SaveSlotName);
bool FZTCreateSaveSlotCommand::Update()
{
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		UZTGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<UZTGameInstanceSubsystem>();
		Subsystem->CreateSaveGameSlot(SaveSlotName);
		UE_LOG(LogTemp, Warning, TEXT("Successfully Created Save Slot."));
		return true;
	}

	return false;
}


DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FZTSaveTestDataCommand, FString, SaveSlotName, FString, MeshPath);
bool FZTSaveTestDataCommand::Update()
{
	if(UWorld* TestWorld = GetTestWorld())
	{
		if(AZTPlayerController* PlayerController = Cast<AZTPlayerController>(TestWorld->GetFirstPlayerController()))
		{
			PlayerController->SaveSlotName = SaveSlotName;
			PlayerController->LastHitLocation = FMath::RandRange(-1.0f, 1.0f) * FVector(1000, 1000, 0);
			UStaticMesh* Mesh =  LoadObject<UStaticMesh>(nullptr,  *MeshPath);
			if(Mesh)
			{
				FMeshData MeshData;
				MeshData.Mesh = Mesh;
				
				PlayerController->BaseMaterial  = Mesh->GetMaterial(0);
				PlayerController->SpawnMeshFromMeshData(MeshData);
				PlayerController->SaveSlot();
				return true;
			}

			UE_LOG(LogTemp, Error, TEXT("Mesh Does not Exist."));

		}

		UE_LOG(LogTemp, Error, TEXT("PlayerController Does not Exist."));
	}

	return false;
}



void ZTSaveLoadComplexTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("Save and Load Complex Test - Chair");
	OutTestCommands.Add("SaveSlotName=TestSlot1,MeshPath=/Game/StarterContent/Props/SM_Chair");

	OutBeautifiedNames.Add("Save and Load Complex Test - Table");
	OutTestCommands.Add("SaveSlotName=TestSlot2,MeshPath=/Game/StarterContent/Props/SM_TableRound");

	OutBeautifiedNames.Add("Save and Load Complex Test - Couch");
	OutTestCommands.Add("SaveSlotName=TestSlot3,MeshPath=/Game/StarterContent/Props/SM_Couch");
}

bool ZTSaveLoadComplexTest::RunTest(const FString& Parameters)
{

	FString SaveSlotName = "DefaultSlotName";
	FString  MeshPath = "/Game/StarterContent/Props/SM_Chair";

	FParse::Value(*Parameters, TEXT("SaveSlotName="), SaveSlotName);
	FParse::Value(*Parameters, TEXT("MeshPath="), MeshPath);
	
	ADD_LATENT_AUTOMATION_COMMAND(FZTCreateSaveSlotCommand(SaveSlotName));
	ADD_LATENT_AUTOMATION_COMMAND(FZTSaveTestDataCommand(SaveSlotName, MeshPath));
	
	return true;
}
