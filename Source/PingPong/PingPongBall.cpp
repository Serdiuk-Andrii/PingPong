#include "PingPongBall.h"

#include "PingPongPlatform.h"
#include "Components/BoxComponent.h"

APingPongBall::APingPongBall()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->SetConstraintMode(EDOFMode::XYPlane);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComponent->SetupAttachment(StaticMeshComponent);
	BoxComponent->SetCollisionProfileName("Trigger");
	BoxComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
}

void APingPongBall::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	if (BallMesh)
	{
		StaticMeshComponent->SetStaticMesh(BallMesh);
	}
	OnActorBeginOverlap.AddDynamic(this, &APingPongBall::OnPlatformHit);
}

void APingPongBall::OnPlatformHit(AActor* OverlappedActor, AActor* OtherActor)
{
	APingPongPlatform* Platform = Cast<APingPongPlatform>(OtherActor);
	if (Platform)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
		TEXT("Collision with a platform!"));
		const FVector BallLinearVelocity = StaticMeshComponent->GetPhysicsLinearVelocity();
		const float Signum = BallLinearVelocity.Y > 0 ? 1.0f : -1.0f;
		const FVector NewBallLinearVelocity = FVector(3000.0f * -1.0f,
													  3000.0f * Signum,
													  BallLinearVelocity.Z);
		
		StaticMeshComponent->SetPhysicsLinearVelocity(NewBallLinearVelocity);
	}
	else
	{
		const FVector BallLinearVelocity = StaticMeshComponent->GetPhysicsLinearVelocity();

		FBox ActorBoundingBox = OtherActor->GetComponentsBoundingBox();
		// Extract the X values from the bounding box
		float MinX = ActorBoundingBox.Min.X;
		float MaxX = ActorBoundingBox.Max.X;
		
		float BallX = this->GetActorLocation().X;
		FVector NewBallLinearVelocity;
		if (BallX >= MinX && BallX <= MaxX)
		{
			// Hitting a wall perpendicular to platform
			const float Signum = BallLinearVelocity.X > 0 ? 1.0f : -1.0f;
			NewBallLinearVelocity = FVector(3000.0f * Signum,
														  3000.0f * -1.0f,
														  BallLinearVelocity.Z);
		}
		else
		{
			// Hitting a wall parallel to platform
			const float Signum = BallLinearVelocity.Y > 0 ? 1.0f : -1.0f;
			NewBallLinearVelocity = FVector(3000.0f * -1.0f,3000.0f * Signum,BallLinearVelocity.Z);
		}
		StaticMeshComponent->SetPhysicsLinearVelocity(NewBallLinearVelocity);
	}
}

void APingPongBall::StartMoving() const
{
	StaticMeshComponent->SetPhysicsLinearVelocity(FVector(3000.0f,
		(FMath::RandBool() ? 1.f : 1.f) * 500.f, 0.f
	));
}
