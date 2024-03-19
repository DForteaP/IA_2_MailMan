
#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GraphNodeComponent.generated.h"

UENUM(BlueprintType)
enum class ETypeNodes : uint8	
{
	Libre,
	Bloqueado,
	Seleccionado,
	Posible
};

UCLASS()
class CLASEAI_API UGraphNodeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	TArray<UGraphNodeComponent*> AdjacentNodes;

public:

	UPROPERTY(EditAnywhere, Category="Graph")
	int X;

	UPROPERTY(EditAnywhere, Category="Graph")
	int Y;
	
	void SetAdjacentNodes(const TArray<UGraphNodeComponent*>& InAdjacentNodes);

	const TArray<UGraphNodeComponent*>& GetAdjacentNodes() const;
	
	UFUNCTION(BlueprintCallable)
	int GetX() const {return X;}

	UFUNCTION(BlueprintCallable)
	int GetY() const {return Y;}

	UPROPERTY(EditAnywhere, Category = "Node")
	ETypeNodes TipoNodo;
	
};