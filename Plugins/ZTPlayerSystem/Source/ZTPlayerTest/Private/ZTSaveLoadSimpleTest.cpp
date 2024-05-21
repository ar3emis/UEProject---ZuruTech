#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "ZTPlayerController.h"
#include "SubSystems/ZTGameInstanceSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ZTSaveLoadSimpleTest, "ZTProject.ZTSaveLoadSimpleTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ZTSaveLoadSimpleTest::RunTest(const FString& Parameters)
{
     const FString MapName = "/Game/Maps/TestLevel.TestLevel";
	 AutomationOpenMap(MapName);

	UWorld* TestWorld = AutomationCommon::GetAnyGameWorld();
	if(TestWorld)
	{
		if(UGameInstance* GameInstance = TestWorld->GetGameInstance())
		{
			FString SaveSlotName = "ZTSaveSlot";
			UZTGameInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<UZTGameInstanceSubsystem>();
			Subsystem->CreateSaveGameSlot(SaveSlotName);
			
			if(AZTPlayerController* PlayerController = Cast<AZTPlayerController>(TestWorld->GetFirstPlayerController()))
			{
				PlayerController->LastHitLocation = FMath::RandRange(-1.0f, 1.0f) * FVector(1000, 1000, 0);
				UStaticMesh* Mesh =  LoadObject<UStaticMesh>(nullptr,  TEXT("/Game/StarterContent/Props/SM_Chair"));
				if(Mesh)
				{
					FMeshData MeshData;
					MeshData.Mesh = Mesh;
					PlayerController->SaveSlotName = SaveSlotName;
					PlayerController->BaseMaterial  = Mesh->GetMaterial(0);
					PlayerController->SpawnMeshFromMeshData(MeshData);
					PlayerController->SaveSlot();

					AddInfo("Test Result Is Successful!!!", 1);
					return true;

					
				}

				AddError("Mesh does not exist", 1);
				return true;
			}

			Subsystem->LoadGame(SaveSlotName);

			UZTPlayerSaveGame* CurrentSaveGame = Subsystem->GetCurrentSaveGame();
			TestTrue("Save Slot should Exist", CurrentSaveGame->SaveSlots.Contains(SaveSlotName));
			
		}

		
		AddError("Game Instance Should Exist", 1);
		return true;
	
	}

	
	AddError("World Should Exist", 1);
	return true;
}
