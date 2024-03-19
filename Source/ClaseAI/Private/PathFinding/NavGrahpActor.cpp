

#include "PathFinding/NavGrahpActor.h"

#include "PathFinding/AlgoPathFinding.h"
#include "PathFinding/GraphNodeComponent.h"

ANavGrahpActor::ANavGrahpActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshPlayerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerComponent"));
	MeshEnemyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyComponent"));
	MeshTargetComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetComponent"));
	
	MeshPlayerComponent->SetStaticMesh(MeshPlayer);
	MeshEnemyComponent->SetStaticMesh(MeshEnemy);
	MeshTargetComponent->SetStaticMesh(MeshTarget);
}

void ANavGrahpActor::SelectNode(int x, int y)
{
	GetPosibleNodesMovement();
	for (auto Node : GraphNodes)
	{
		if (Node->X == x && Node->Y == y && Node->TipoNodo != ETypeNodes::Bloqueado && Node->TipoNodo == ETypeNodes::Posible)
		{
			Node->TipoNodo = ETypeNodes::Seleccionado;
			Node->SetMaterial(0,Cell_SelectedState);
		} else if (Node->TipoNodo == ETypeNodes::Bloqueado)
		{
			Node->TipoNodo = ETypeNodes::Bloqueado;
			Node->SetMaterial(0,Cell_Blocked);
		} else if (Node->TipoNodo == ETypeNodes::Posible)
		{
			Node->TipoNodo = ETypeNodes::Posible;
			Node->SetMaterial(0,Cell_Posible);
		}else
		{
			Node->TipoNodo = ETypeNodes::Libre;
			Node->SetMaterial(0,Cell_DefaultState);
		}
	}
}

void ANavGrahpActor::ShowCharacter(int x, int y)
{
	for (auto Node : GraphNodes)
	{
	   if (Node->X == x && Node->Y == y)
	   {
	   		MeshPlayerComponent->SetWorldLocation(Node->GetComponentLocation()+ FVector3d(0,0,100));
			PlayerLocation = FVector2d(Node->X, Node->Y);
	   }
	}
}

void ANavGrahpActor::ShowEnemy(int x, int y)
{
	for (auto Node : GraphNodes)
	{
		if (Node->X == x && Node->Y == y)
		{
			MeshEnemyComponent->SetWorldLocation(Node->GetComponentLocation() + FVector3d(0,0,100));
			EnemyLocation = FVector2d(Node->X, Node->Y);
		}
	}
}

void ANavGrahpActor::ShowTarget(int x, int y)
{
	for (auto Node : GraphNodes)
	{
		if (Node->X == x && Node->Y == y)
		{
			MeshTargetComponent->SetWorldLocation(Node->GetComponentLocation() + FVector3d(0,0,100));
			TargetLocation = FVector2d(Node->X, Node->Y);
		}
	}
}

void ANavGrahpActor::RefreshNodes()
{
	GetPosibleNodesMovement();
	for (auto Node : GraphNodes)
	{
		if(Node->TipoNodo == ETypeNodes::Bloqueado)
		{
			Node->SetMaterial(0,Cell_Blocked);
		}
		if(Node->TipoNodo == ETypeNodes::Libre)
		{
			Node->SetMaterial(0,Cell_DefaultState);
		}
		if(Node->TipoNodo == ETypeNodes::Posible)
		{
			Node->SetMaterial(0,Cell_Posible);
		}
		if (Node->TipoNodo == ETypeNodes::Seleccionado)
		{
			Node->SetMaterial(0,Cell_SelectedState);
		}
	}
}

void ANavGrahpActor::GenerateTarget()
{
	
	if (GraphNodes.Num() == 0)
	{
		return;
	}

	TArray<UGraphNodeComponent*> NodosForTarget = {};
	for (auto Node : GraphNodes)
	{
		if (Node->TipoNodo != ETypeNodes::Bloqueado && Node->TipoNodo != ETypeNodes::Posible && Node != GetStartNode() && Node != GetEndNode())
		{
			NodosForTarget.Add(Node);
		} 
	}
	
	int32 RandomIndex = FMath::RandRange(0, NodosForTarget.Num() - 1);
	UGraphNodeComponent* RandomNode = NodosForTarget[RandomIndex];
	ShowTarget(RandomNode->X, RandomNode->Y);
	
}

bool ANavGrahpActor::EnemyCatchPlayer()
{
	if (EnemyLocation == PlayerLocation)
	{
		return true;
	}
	return false;
}

bool ANavGrahpActor::PlayerCatchTarget()
{
	if (TargetLocation == PlayerLocation)
	{
		return true;
	}
	return false;
}


void ANavGrahpActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(!bRunConstruction) return;
	
	for (auto GraphNode: GraphNodes)
	{
		if(GraphNode)
		{
			GraphNode->UnregisterComponent();
			GraphNode->MarkAsGarbage();			
		}
	}
	GraphNodes.Reset();

	for (int32 i = 0; i < RowCount; ++i)
	{
		for (int32 j = 0; j < RowCount; ++j)
		{
			const FName NodeName = *FString::Printf(TEXT("Node_%d_%d"), i, j);
			
			UGraphNodeComponent* Node = NewObject<UGraphNodeComponent>(this, NodeName);
			
			Node->SetupAttachment(RootComponent);
			Node->RegisterComponent();			
			Node->SetRelativeLocation({i * DistanceBetweenNodes, j * DistanceBetweenNodes, 0});
			Node->X = i;
			Node->Y = j;

			if(Visualizer) {
				Node->SetStaticMesh(Visualizer);

				if (Cell_SelectedState &&
					SelectedCellIndexes.Contains(FVector2D{static_cast<double>(i),static_cast<double>(j)}))
				{
					Node->TipoNodo = ETypeNodes::Libre;
					Node->SetMaterial(0,Cell_SelectedState);
				}
				if (Cell_Blocked &&
					BlockedCellIndexes.Contains(FVector2D{static_cast<double>(i),static_cast<double>(j)}))
				{
					Node->TipoNodo = ETypeNodes::Bloqueado;
					Node->SetMaterial(0,Cell_Blocked);
				}
			}
		
			GraphNodes.Add(Node);
		}
	}
	
	for (auto Node : GraphNodes)
	{
		TArray<UGraphNodeComponent*> AdjacentNodes = GetAdjacentNodes(Node);
		Node->SetAdjacentNodes(AdjacentNodes);
	}

	ShowCharacter(GetStartNode()->X, GetStartNode()->Y);
	ShowEnemy(GetEndNode()->X, GetEndNode()->Y);
}

void ANavGrahpActor::RunPathFindingQuery()
{
	ResetNodeStates();
	
	TArray<UGraphNodeComponent*> OutPath;
	//UAlgoPathFinding::Djikstra(this, OutPath);
	UAlgoPathFinding::AStar(this, OutPath);

	RefreshNodes();
	
	for (auto Node : OutPath)
	{
		Node->SetMaterial(0,Cell_SelectedState);
		GetEndNode()->SetMaterial(0,Cell_SelectedState);
	}
}

void ANavGrahpActor::EnemyMove()
{
	ResetNodeStates();
	
	TArray<UGraphNodeComponent*> OutPath;
	//UAlgoPathFinding::Djikstra(this, OutPath);
	UAlgoPathFinding::AStar(this, OutPath);

	RefreshNodes();
	
	int numeroarray = OutPath.Num() - 3;
	ShowEnemy(OutPath[numeroarray]->X, OutPath[numeroarray]->Y);
}

void ANavGrahpActor::SetOrigin() 
{
	StartNodeCoordinates = EnemyLocation;
}

void ANavGrahpActor::SetDestiny()
{
	EndNodeCoordinates = PlayerLocation;
}

UGraphNodeComponent* ANavGrahpActor::GetSelectedNode()
{
	for(auto Node : GraphNodes)
	{		
		if (Node->TipoNodo == ETypeNodes::Seleccionado)
		{
			return Node;
		}
	}
	return nullptr;
}

void ANavGrahpActor::ResetNodeStates()
{
	for (auto Node : GraphNodes)
	{
		Node->SetMaterial(0,Cell_DefaultState);
	}
}

TArray<UGraphNodeComponent*> ANavGrahpActor::GetPosibleNodesMovement()
{
	int In_x = PlayerLocation.X;
	int In_y = PlayerLocation.Y;
	TArray<UGraphNodeComponent*> PosiblesNodes;
	for(auto Node : GraphNodes)
	{
		if ( ((Node->X == In_x + 1 && Node->Y == In_y) || (Node->X == In_x + 2 && Node->Y == In_y) || (Node->X == In_x - 1 && Node->Y == In_y) || (Node->X == In_x - 2 && Node->Y == In_y)
			|| (Node->Y == In_y + 1 && Node->X == In_x) || (Node->Y == In_y + 2 && Node->X == In_x) || (Node->Y == In_y - 1 && Node->X == In_x) || (Node->Y == In_y - 2 && Node->X == In_x)
			|| (Node->X == In_x + 1 && Node->Y == In_y + 1) || (Node->X == In_x - 1 && Node->Y == In_y - 1) || (Node->X == In_x + 1 && Node->Y == In_y - 1) || (Node->X == In_x - 1 && Node->Y == In_y + 1)
			) && (Node->TipoNodo != ETypeNodes::Bloqueado) )
		{
			PosiblesNodes.Add(Node);
			Node->SetMaterial(0,Cell_Posible);
			Node->TipoNodo = ETypeNodes::Posible;
		} else if (Node->TipoNodo == ETypeNodes::Bloqueado)
		{
			Node->SetMaterial(0,Cell_Blocked);
			Node->TipoNodo = ETypeNodes::Bloqueado;
		} else
		{
			Node->SetMaterial(0,Cell_DefaultState);
			Node->TipoNodo = ETypeNodes::Libre;
		}
	}
	return PosiblesNodes;
}

void ANavGrahpActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.GetMemberPropertyName().IsEqual(GET_MEMBER_NAME_CHECKED(ANavGrahpActor, StartNodeCoordinates)))
	{
		StartNodeCoordinates.X = FMath::Clamp(StartNodeCoordinates.X, 0, RowCount-1);		
		StartNodeCoordinates.Y = FMath::Clamp(StartNodeCoordinates.Y, 0, RowCount-1);
		
	}else if(PropertyChangedEvent.GetMemberPropertyName().IsEqual(GET_MEMBER_NAME_CHECKED(ANavGrahpActor, EndNodeCoordinates)))
	{
		EndNodeCoordinates.X = FMath::Clamp(EndNodeCoordinates.X, 0, RowCount-1);		
		EndNodeCoordinates.Y = FMath::Clamp(EndNodeCoordinates.Y, 0, RowCount-1);
	}
}