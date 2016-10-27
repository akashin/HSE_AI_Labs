// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAIController.h"
#include "NavGraph.h"
#include "CleverAIController.generated.h"

/**
 *
 */
UCLASS()
class LAB_3_API ACleverAIController : public ABaseAIController
{
    GENERATED_BODY()

public:
    ACleverAIController();

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = Movement)
    int ChooseDirectionProbeCount;

    UPROPERTY(EditAnywhere, Category = Movement)
    float MinAllowedScale;

    UPROPERTY(EditAnywhere, Category = Movement)
    float InitialScale;

    UPROPERTY(EditAnywhere, Category = Movement)
    float ScaleDecayRate;

    UPROPERTY(EditAnywhere, Category = Movement)
    float AcceptableDistanceToTarget;

private:
    NavGraph Graph;

    int CurrentVertex;

    int NextVertex;
    FVector NextVertexLocation;
    float PreviousDistanceToNextVertex;

    TArray<int> TraversalStack;
    TSet<int> DiscoveredVertices;
    TSet<int> VisitedVertices;
    TMap<int, TArray<FVector>> PossibleDiscoveries;

    bool bIsMoving;
    bool bIsMovingBack;

    void DiscoverNeighborhood();

    bool ChooseDirection();

    void GoToVertex(FVector vertexLocation);

    float GetDirectionScale(FVector direction);

};
