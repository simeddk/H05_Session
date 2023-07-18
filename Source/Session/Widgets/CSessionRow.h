#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSessionRow.generated.h"

UCLASS()
class SESSION_API UCSessionRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SessionName;
};
