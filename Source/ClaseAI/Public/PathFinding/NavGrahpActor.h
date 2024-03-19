#pragma once

#include "CoreMinimal.h"
#include "GraphNodeComponent.h"
#include "VectorUtil.h"
#include "GameFramework/Actor.h"
#include "NavGrahpActor.generated.h"

class AClaseAICharacter;
class UGraphNodeComponent;

UCLASS()
class CLASEAI_API ANavGrahpActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Graph")
	TArray<UGraphNodeComponent*> GraphNodes;

public:
	ANavGrahpActor();

	UPROPERTY(EditAnywhere, Category="Graph")
	int32 RowCount = 10;

	UPROPERTY(EditAnywhere, Category="Graph")
	float DistanceBetweenNodes = 100.f;

	UPROPERTY(EditAnywhere, Category="Graph")
	UStaticMesh* Visualizer;

	UPROPERTY(EditAnywhere, Category="Graph")
	UMaterialInterface* Cell_DefaultState;

	UPROPERTY(EditAnywhere, Category="Graph")
	UMaterialInterface* Cell_SelectedState;

	UPROPERTY(EditAnywhere, Category="Graph")
	UMaterialInterface* Cell_Blocked;

	UPROPERTY(EditAnywhere, Category="Graph")
	UMaterialInterface* Cell_Posible;
	
	UPROPERTY(EditAnywhere, Category="Graph")
	TArray<FVector2D> SelectedCellIndexes;

	UPROPERTY(EditAnywhere, Category="Graph")
	TArray<FVector2D> BlockedCellIndexes;
	
	UPROPERTY(EditAnywhere, Category="Graph")
	bool bRunConstruction = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* MeshPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* MeshEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* MeshTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshPlayerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshEnemyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshTargetComponent;
	
	// Navigations settings
	UPROPERTY(EditAnywhere, Category="Graph | Pathfinding") FVector2D StartNodeCoordinates;
	UPROPERTY(EditAnywhere, Category="Graph | Pathfinding") FVector2D EndNodeCoordinates;
	
	FORCEINLINE void GetGraphNodes(TArray<UGraphNodeComponent*>& Nodes) const { Nodes = GraphNodes; }

	FORCEINLINE UGraphNodeComponent* GetStartNode() const
	{
		return GraphNodes[LinealizeCoords(FCoords{static_cast<int32>(StartNodeCoordinates.X), static_cast<int32>(StartNodeCoordinates.Y)})];
	}

    FORCEINLINE UGraphNodeComponent* GetEndNode()	const
    {
    	return GraphNodes[LinealizeCoords(FCoords{static_cast<int32>(EndNodeCoordinates.X), static_cast<int32>(EndNodeCoordinates.Y)})];;
    }
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Graph | Pathfinding")
	void RunPathFindingQuery();

	UFUNCTION(CallInEditor, BlueprintCallable, Category="Graph | Pathfinding")
	void EnemyMove();

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void SetOrigin();
	
	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void SetDestiny();

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	UGraphNodeComponent*  GetSelectedNode();
	
	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void SelectNode(int x, int y);

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void ShowCharacter(int x, int y);

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void ShowEnemy(int x, int y);
	
	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void ShowTarget(int x, int y);

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void RefreshNodes();

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	void GenerateTarget();

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	bool EnemyCatchPlayer();

	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	bool PlayerCatchTarget();
	
	UFUNCTION(BlueprintCallable, Category="Graph | Pathfinding")
	TArray<UGraphNodeComponent*> GetPosibleNodesMovement();

protected:
#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif

private:

	void ResetNodeStates();

	FVector2d EnemyLocation = FVector2d(9,9);
	FVector2d PlayerLocation = FVector2d(0,0);
	FVector2d TargetLocation = FVector2d(0,0);
	
	struct FCoords
	{
		int32 i, j;

		FCoords operator+(const FCoords& Other)
		{
			return {i + Other.i, j + Other.j};
		}
		FCoords operator+(const FCoords& Other) const
		{
			return {i + Other.i, j + Other.j};
		}
	};
	
	inline static const FCoords Up		{-1,0};
	inline static const FCoords Down	{1,0};
	inline static const FCoords Left	{0,-1};
	inline static const FCoords Right	{0,1};
	
	FORCEINLINE uint32 LinealizeCoords(const FCoords& InCoords) const
	{
		return InCoords.i * RowCount + InCoords.j;
	}

	FORCEINLINE void SetAdjacentNodesToNodes(UGraphNodeComponent* Node, const TArray<UGraphNodeComponent*>& InAdjacentNodes) const
	{
		Node->SetAdjacentNodes(InAdjacentNodes);
	}

	FORCEINLINE FCoords CoordinatesFromLinearIndex(const SIZE_T Index) const
	{
		return FCoords{static_cast<int32>(Index / RowCount), static_cast<int32>(Index % RowCount)};
	}

	FORCEINLINE bool CheckCoordsWithinGraph(const FCoords& InCoords) const
	{
		return (InCoords.i >= 0 && InCoords.i < RowCount && InCoords.j >= 0 && InCoords.j < RowCount);
	}

	
	TArray<UGraphNodeComponent*> GetAdjacentNodes(UGraphNodeComponent* Node) const
	{
		TArray<UGraphNodeComponent*> OutNodes;
	
		const SIZE_T Index = GraphNodes.Find(Node);
		const FCoords NodeCoord = CoordinatesFromLinearIndex(Index);
		
		const FCoords Node_Up		= ANavGrahpActor::Up		+ NodeCoord;
		const FCoords Node_Down		= ANavGrahpActor::Down		+ NodeCoord;
		const FCoords Node_Left		= ANavGrahpActor::Left		+ NodeCoord;
		const FCoords Node_Right	= ANavGrahpActor::Right		+ NodeCoord;

		if (CheckCoordsWithinGraph(Node_Up))	OutNodes.Add(GraphNodes[LinealizeCoords(Node_Up)]);
		if (CheckCoordsWithinGraph(Node_Down))	OutNodes.Add(GraphNodes[LinealizeCoords(Node_Down)]);
		if (CheckCoordsWithinGraph(Node_Left))	OutNodes.Add(GraphNodes[LinealizeCoords(Node_Left)]);
		if (CheckCoordsWithinGraph(Node_Right))	OutNodes.Add(GraphNodes[LinealizeCoords(Node_Right)]);
		
		for(auto OutNode : OutNodes)
		{
			UE_LOG(LogTemp, Display, TEXT("CurrentNode: %s, AdjacentNode: %s"), *Node->GetName(), *OutNode->GetName())			
		}

		return OutNodes;
	}
	
};
