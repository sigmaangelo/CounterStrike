// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/TracerRound.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATracerRound::ATracerRound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(4.0f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TracerRound = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TracerRound"));
	TracerRound->SetupAttachment(SphereCollision);
	TracerRound->SetRelativeScale3D(FVector(2.0f, 0.05f, 0.05f));
	TracerRound->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshTracer(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/Effect/TracerRound.TracerRound'"));

	if (MeshTracer.Succeeded())
	{
		TracerRound->SetStaticMesh(MeshTracer.Object);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	MovementComponent->InitialSpeed = 30000;
	MovementComponent->MaxSpeed = 30000;
	MovementComponent->Bounciness = 0;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Friction = 0;
	MovementComponent->InterpLocationTime = 0.05f;
	MovementComponent->ProjectileGravityScale = 0;


}

// Called when the game starts or when spawned
void ATracerRound::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			this->Destroy();
		}), 1, false);
	
}

// Called every frame
void ATracerRound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATracerRound::OnSphereComponentBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	this->Destroy();
}

