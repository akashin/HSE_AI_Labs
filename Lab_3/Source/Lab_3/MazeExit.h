// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MazeExit.generated.h"

UCLASS()
class LAB_3_API AMazeExit : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMazeExit();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    void SetExitIndex(int index);

    bool TryEscape(FVector characterLocation);

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Level)
    UStaticMeshComponent* ExitMesh;

    static constexpr float EscapeRadius = 300.0f;

private:
    int ExitIndex;

};
