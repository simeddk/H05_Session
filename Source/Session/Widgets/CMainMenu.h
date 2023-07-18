#pragma once

#include "CoreMinimal.h"
#include "CMenuBase.h"
#include "CMainMenu.generated.h"

UCLASS()
class SESSION_API UCMainMenu : public UCMenuBase
{
	GENERATED_BODY()

public:
	UCMainMenu();

protected:
	virtual bool Initialize() override;

public:
	void SetSessionList(TArray<FString> InSessionNames);
	void SetSessionRowIndex(uint32 InIndex);

private:
	UFUNCTION()	void HostServer();
	UFUNCTION()	void JoinServer();
	UFUNCTION()	void OpenJoinMenu();
	UFUNCTION()	void OpenMainMenu();
	UFUNCTION()	void QuitGame();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* SessionList;

private:
	TSubclassOf<class UCSessionRow> SessionRowWidgetClass;

	TOptional<uint32> SessionRowIndex;
};
