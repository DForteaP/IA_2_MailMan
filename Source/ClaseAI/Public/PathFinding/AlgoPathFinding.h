
#pragma once

#include "CoreMinimal.h"
#include "NavGrahpActor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AlgoPathFinding.generated.h"


UCLASS()
class CLASEAI_API UAlgoPathFinding : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void Djikstra(const ANavGrahpActor* NavGraph,  TArray<UGraphNodeComponent*>& Path);

	UFUNCTION(BlueprintCallable)
	static void BackTrack(const ANavGrahpActor* InNavGraph, TMap<UGraphNodeComponent*, float> InLowestDistances, TArray<UGraphNodeComponent*>& InPath);
	
	UFUNCTION(BlueprintCallable)
	static void AStar(const ANavGrahpActor* InNavGraph, TArray<UGraphNodeComponent*>& InPath);


public:
	
	static double Heuristic_EuclideanDistance(const UGraphNodeComponent* OriginNode, const UGraphNodeComponent* DestNode);

	UFUNCTION(CallInEditor, BlueprintCallable, Category="Graph | Pathfinding")
	static UGraphNodeComponent* GetShortestNode(TArray<UGraphNodeComponent*> Array, TMap<UGraphNodeComponent*, float> Map);
	
};
