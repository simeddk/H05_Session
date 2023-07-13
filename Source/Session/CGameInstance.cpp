#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Widgets/CMainMenu.h"
#include "Widgets/CMenuBase.h"

const static FName SESSION_NAME = TEXT("MySession");

UCGameInstance::UCGameInstance(const FObjectInitializer& InObject)
{
	CLog::Log("CGameInstance Constructor Called");

	CHelpers::GetClass<UUserWidget>(&MainMenuWidgetClass, "/Game/Widgets/WB_MainMenu");
	CHelpers::GetClass<UUserWidget>(&GameMenuWidgetClass, "/Game/Widgets/WB_GameMenu");
}

void UCGameInstance::Init()
{
	CLog::Log("CGameInstance Init Called");

	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (!!oss)
	{
		CLog::Log("OSS Name : " + oss->GetSubsystemName().ToString());

		//Session Event Binding
		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);

			//Find Session
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				CLog::Log("Starting Find Sessions");

				SessionSearch->bIsLanQuery = true;
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
	else
	{
		CLog::Log("OSS Not Found");
	}
}

void UCGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		//SessionName이 중복인 경우, 세션 삭제 -> 세션 재생성
		auto exsistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (!!exsistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		//세션 생성
		else
		{
			CreateSession();
		}
	}
}

void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = true;
		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
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

void UCGameInstance::LoadGameMenu()
{
	CheckNull(GameMenuWidgetClass);

	UCMenuBase* gameMenu = CreateWidget<UCMenuBase>(this, GameMenuWidgetClass);
	CheckNull(gameMenu);

	gameMenu->SetOwingGameInstance(this);
	gameMenu->SetUIMode();
}

void UCGameInstance::ReturnToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);

	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::OnCreateSessionComplete(FName InSessionName, bool InSuccess)
{
	//세션이 정상적으로 생성되었는지 체크
	if (InSuccess == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Cound Not Create Session"));
		return;
	}

	//Play 맵으로 ServerTravel
	if (!!MainMenu)
		MainMenu->SetPlayMode();

	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	if (InSuccess == true)
		CreateSession();
}

void UCGameInstance::OnFindSessionComplete(bool InSuccess)
{
	if (InSuccess == true && SessionSearch.IsValid())
	{
		CLog::Log("Finished Find Sessions");

		for (const auto& searchResult : SessionSearch->SearchResults)
		{
			CLog::Log("==============================");
			CLog::Log(searchResult.GetSessionIdStr());
			CLog::Log(searchResult.PingInMs);
			CLog::Log("==============================");
		}
	}

	//TODO.
	//1. SessionSearch 얻은 정보(세션 검색 결과)를 -> SessionRow에게 전달
	//2. Join 메뉴로 들어올 때마다 SessionList(ScrollBox)를 새로고침
	
}
