// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include "PingPongPlatform.h"

void APingPongPlayerController::MovePlatform(float Value)
{
	APingPongPlatform* Platform = Cast<APingPongPlatform>(this->GetPawn());
	if (Platform)
	{
		Platform->MoveY(Value);
	}
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveY", this, &APingPongPlayerController::MovePlatform);
}
