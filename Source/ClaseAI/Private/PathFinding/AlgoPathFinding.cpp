
#include "PathFinding/AlgoPathFinding.h"

void UAlgoPathFinding::Djikstra(const ANavGrahpActor* NavGraph,  TArray<UGraphNodeComponent*>& Path)
{
	TMap<UGraphNodeComponent*, float> LowestDistanceFromStartNode;
	TArray<UGraphNodeComponent*> PendingNodes;
	NavGraph->GetGraphNodes(PendingNodes);

	for(auto Node: PendingNodes)
	{
		if(Node==NavGraph->GetStartNode())
		{
			LowestDistanceFromStartNode.Add(Node, 0.f);
		}
		else
		{
			LowestDistanceFromStartNode.Add(Node, 1E8);
		}
	}
	
	while (!PendingNodes.IsEmpty())
	{
		UGraphNodeComponent* ShortestDistanceNode = UAlgoPathFinding::GetShortestNode(PendingNodes, LowestDistanceFromStartNode);
		PendingNodes.Remove(ShortestDistanceNode);
		
			for(const auto AdjacentNode : ShortestDistanceNode->GetAdjacentNodes())
			{
				const float DistanceToNode = FVector::Distance(ShortestDistanceNode->GetRelativeLocation(), AdjacentNode->GetRelativeLocation()) + LowestDistanceFromStartNode[ShortestDistanceNode];
				if(LowestDistanceFromStartNode[AdjacentNode] > DistanceToNode)
				{
					LowestDistanceFromStartNode[AdjacentNode] = DistanceToNode;
				}
			}
	}
	UAlgoPathFinding::BackTrack(NavGraph, LowestDistanceFromStartNode, Path);
}

void UAlgoPathFinding::BackTrack(const ANavGrahpActor* NavGraph, TMap<UGraphNodeComponent*, float> LowestDistanceMap, TArray<UGraphNodeComponent*>& Path)
{
	UGraphNodeComponent* CurrentBestNode = NavGraph->GetEndNode();
	
	while (CurrentBestNode != NavGraph->GetStartNode())
	{
		CurrentBestNode = GetShortestNode(CurrentBestNode->GetAdjacentNodes(), LowestDistanceMap);
		Path.Add(CurrentBestNode);
	}

	Path.Add(CurrentBestNode);
}

double UAlgoPathFinding::Heuristic_EuclideanDistance(const UGraphNodeComponent* OriginNode, const UGraphNodeComponent* DestNode)
{
	return FVector::Dist(OriginNode->GetComponentLocation(), DestNode->GetComponentLocation());
}

void UAlgoPathFinding::AStar(const ANavGrahpActor* InNavGraph, TArray<UGraphNodeComponent*>& InPath)
{
	TMap<UGraphNodeComponent*, float> G;
	TMap<UGraphNodeComponent*, float> F;

	TArray<UGraphNodeComponent*> Visitables;
	InNavGraph->GetGraphNodes(Visitables);

	for (const auto Node: Visitables)
	{
		G.Add(Node, UE_MAX_FLT);
		F.Add(Node, UE_MAX_FLT);
	}
	
	G[InNavGraph->GetStartNode()] = 0.f;
	F[InNavGraph->GetStartNode()] = Heuristic_EuclideanDistance(InNavGraph->GetStartNode(), InNavGraph->GetEndNode());

	TArray<UGraphNodeComponent*> VisitedNodes;
	UGraphNodeComponent* CurrentNode = nullptr;

	while(!Visitables.IsEmpty())
	{
		CurrentNode = GetShortestNode(Visitables, F);
		Visitables.Remove(CurrentNode);
		VisitedNodes.Add(CurrentNode);

		//
			if (CurrentNode == InNavGraph->GetEndNode())
			{
				BackTrack(InNavGraph, F, InPath);
				return;
			}
		//

		for(const auto AdjacentNode : CurrentNode->GetAdjacentNodes())
		{
			const float G_Adyacent = G[CurrentNode] + FVector::Dist(CurrentNode->GetComponentLocation(), AdjacentNode->GetComponentLocation());
			const float H_Adyacent = Heuristic_EuclideanDistance(AdjacentNode, InNavGraph->GetEndNode());
			const float F_Adyacent = G_Adyacent + H_Adyacent;

			if (AdjacentNode->TipoNodo != ETypeNodes::Bloqueado)
			{
				if(F[AdjacentNode] > F_Adyacent)
				{
					G[AdjacentNode] = G_Adyacent;
					F[AdjacentNode] = F_Adyacent;
				}
			}
		}
	}
}

UGraphNodeComponent* UAlgoPathFinding::GetShortestNode(TArray<UGraphNodeComponent*> PendingNodes, TMap<UGraphNodeComponent*, float> LowestDistance)
{
	float BestDistance = 1e8;
	UGraphNodeComponent* BestNode = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Nodos pendientes: %d"), PendingNodes.Num());
	for(const auto Node: PendingNodes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nodo %d y %d"), Node->X, Node->Y);
		if(LowestDistance[Node] < BestDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Elegido el Nodo %d y %d"), Node->X, Node->Y);
			BestDistance =  LowestDistance[Node];
			BestNode = Node;
		}
	}
	
	return BestNode;
}
