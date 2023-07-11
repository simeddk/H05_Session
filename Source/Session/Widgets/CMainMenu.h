#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/IMenuInterface.h"
#include "CMainMenu.generated.h"

UCLASS()
class SESSION_API UCMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwingGameInstance(IIMenuInterface* InOwingInstance);
	void SetUIMode();
	void SetPlayMode();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

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
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressField;

private:
	IIMenuInterface* OwingGameInstance;
};
