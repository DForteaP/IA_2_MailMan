
#include "PathFinding/GraphNodeComponent.h"

void UGraphNodeComponent::SetAdjacentNodes(const TArray<UGraphNodeComponent*>& InAdjacentNodes)
{
	AdjacentNodes = InAdjacentNodes;
}

const TArray<UGraphNodeComponent*>& UGraphNodeComponent::GetAdjacentNodes() const
{
	return AdjacentNodes;
}
