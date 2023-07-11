#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMainMenu.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& InObject)
{
	CLog::Log("CGameInstance Constructor Called");

	CHelpers::GetClass<UUserWidget>(&MainMenuWidgetClass, "/Game/Widgets/WB_MainMenu");
}

void UCGameInstance::Init()
{
	CLog::Log("CGameInstance Init Called");
}

void UCGameInstance::Host()
{
	if (!!MainMenu)
		MainMenu->SetPlayMode();

	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::Join(const FString& InAddress)
{
	CLog::Print("Join to " + InAddress);

	if (!!MainMenu)
		MainMenu->SetPlayMode();

	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);

	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::LoadMainMenu()
{
	CheckNull(MainMenuWidgetClass);

	MainMenu = CreateWidget<UCMainMenu>(this, MainMenuWidgetClass);
	CheckNull(MainMenu);

	MainMenu->SetOwingGameInstance(this);
	MainMenu->SetUIMode();
}
