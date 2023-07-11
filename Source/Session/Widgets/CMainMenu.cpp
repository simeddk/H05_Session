#include "CMainMenu.h"
#include "Global.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UCMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);

	CheckNullResult(HostButton, false);
	HostButton->OnClicked.AddDynamic(this, &UCMainMenu::HostServer);

	CheckNullResult(JoinButton, false);
	JoinButton->OnClicked.AddDynamic(this, &UCMainMenu::OpenJoinMenu);

	CheckNullResult(CancelJoinMenuButton, false);
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UCMainMenu::OpenMainMenu);

	CheckNullResult(ConfirmJoinMenuButton, false);
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UCMainMenu::JoinServer);

	return true;
}

void UCMainMenu::SetOwingGameInstance(IIMenuInterface* InOwingInstance)
{
	OwingGameInstance = InOwingInstance;
}

void UCMainMenu::SetUIMode()
{
	AddToViewport();
	bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	CheckNull(world);

	APlayerController* playerController = world->GetFirstPlayerController();
	CheckNull(playerController);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}

void UCMainMenu::SetPlayMode()
{
	RemoveFromViewport();
	bIsFocusable = false;

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	CheckNull(world);

	APlayerController* playerController = world->GetFirstPlayerController();
	CheckNull(playerController);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;
}

void UCMainMenu::HostServer()
{
	CheckNull(OwingGameInstance);

	OwingGameInstance->Host();
}

void UCMainMenu::JoinServer()
{
	CheckNull(OwingGameInstance);
	CheckNull(IPAddressField);

	const FString& address = IPAddressField->GetText().ToString();
	OwingGameInstance->Join(address);
}

void UCMainMenu::OpenJoinMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(JoinMenu);

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UCMainMenu::OpenMainMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(MainMenu);

	MenuSwitcher->SetActiveWidget(MainMenu);
}
