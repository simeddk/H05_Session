#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

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
	CLog::Print("Host");

	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::Join(const FString& InAddress)
{
	CLog::Print("Join to " + InAddress);

	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);

	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::LoadMainMenu()
{
	CheckNull(MainMenuWidgetClass);

	UUserWidget* mainMenu = CreateWidget(this, MainMenuWidgetClass);
	CheckNull(mainMenu);

	mainMenu->AddToViewport();

	mainMenu->bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(mainMenu->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* playerController = GetFirstLocalPlayerController();
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}
