#include "PingPongGameMode.h"

#include "PingPongBall.h"
#include "PingPongPlayerController.h"
#include "PingPongPlatform.h"
#include "Kismet/GameplayStatics.h"


APingPongGameMode::APingPongGameMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	this->SetReplicates(true);
	PlayerControllerClass = APingPongPlayerController::StaticClass();
	DefaultPawnClass = APingPongPlatform::StaticClass();
}

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APingPongPlayerController* PlatformController = Cast<APingPongPlayerController>(NewPlayer);
	if (PlatformController)
	{
		TArray<AActor*> PlatformArray;
		UGameplayStatics::GetAllActorsOfClass(this, APingPongPlatform::StaticClass(), PlatformArray);
		if (PlatformArray.Num() > 1)
		{
			const int Position = GetNumPlayers() - 1;
			APingPongPlatform* Platform = Cast<APingPongPlatform>(PlatformArray[Position]);
			if (Platform)
			{
				PlatformController->Possess(Platform);
				Platform->SetOwner(PlatformController);
			}
		}
		FString Message("Number of players: ");
		Message.AppendInt(GetNumPlayers());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, Message);
		if (GetNumPlayers() > 1)
		{
			this->StartGame();
		}
	}
}

void APingPongGameMode::StartGame() const
{
	// Getting all the balls in the world, assuming there is only one ball.
	TArray<AActor*> BallArray;
	UGameplayStatics::GetAllActorsOfClass(this,
		APingPongBall::StaticClass(), BallArray);
	if (BallArray.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
		TEXT("Two players have joined the game! Start the game!"));
		Cast<APingPongBall>(BallArray[0])->StartMoving();
	}
}


UClass* APingPongGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	APingPongPlayerController* PingPongController = Cast<APingPongPlayerController>(InController);
	if (PingPongController)
	{
		return APingPongPlatform::StaticClass();
	}
	return DefaultPawnClass;
}
