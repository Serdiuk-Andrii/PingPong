// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameMode : public AGameModeBase
{
	
	GENERATED_BODY()

private:
	void StartGame() const;

public:

	APingPongGameMode(const FObjectInitializer& ObjectInitializer);
	
	// Overriding the PostLogin function
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
