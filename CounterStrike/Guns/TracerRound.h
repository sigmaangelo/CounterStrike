// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TracerRound.generated.h"

UCLASS()
class COUNTERSTRIKE_API ATracerRound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracerRound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnSphereComponentBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
		class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* TracerRound;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* MovementComponent;
};
