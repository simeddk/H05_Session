#include "CMainMenu.h"
#include "Global.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "CSessionRow.h"

UCMainMenu::UCMainMenu()
{
	CHelpers::GetClass(&SessionRowWidgetClass, "/Game/Widgets/WB_SessionRow");
}

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

	CheckNullResult(QuitButton, false);
	QuitButton->OnClicked.AddDynamic(this, &UCMainMenu::QuitGame);

	return true;
}

void UCMainMenu::HostServer()
{
	CheckNull(OwingGameInstance);

	OwingGameInstance->Host();
}

void UCMainMenu::JoinServer()
{
	if (!!OwingGameInstance && SessionRowIndex.IsSet())
	{
		CLog::Log("Session Row Index : " + SessionRowIndex.GetValue());

		OwingGameInstance->Join(SessionRowIndex.GetValue());
	}
	else
	{
		CLog::Log("Session Row Index is not set!!");
	}
	
}


void UCMainMenu::SetSessionList(TArray<FString> InSessionNames)
{
	UWorld* world = GetWorld();
	CheckNull(world);

	SessionList->ClearChildren();

	uint32 i = 0;
	for (const FString& sessionName : InSessionNames)
	{
		UCSessionRow* row = CreateWidget<UCSessionRow>(world, SessionRowWidgetClass);
		CheckNull(row);

		row->SessionName->SetText(FText::FromString(sessionName));
		row->Setup(this, i++);

		SessionList->AddChild(row);
	}

}

void UCMainMenu::SetSessionRowIndex(uint32 InIndex)
{
	SessionRowIndex = InIndex;
}

void UCMainMenu::OpenJoinMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(JoinMenu);

	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (!!OwingGameInstance)
		OwingGameInstance->RefreshSessionList();
}

void UCMainMenu::OpenMainMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(MainMenu);

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UCMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	CheckNull(world);

	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	controller->ConsoleCommand("quit");
}
