// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolComponent.h"


AActor* UPatrolBehavior::GetNextTargetPatrolPoint(const TArray<AActor*>& PatrolPoints)
{
	if(CurrentPatrolIndex < PatrolPoints.Num())
	{
		return PatrolPoints[CurrentPatrolIndex++];	
	}
	else
	{
		CurrentPatrolIndex = 0;
		return PatrolPoints[CurrentPatrolIndex];
	}
}

AActor* URandomPatrolBehavior::GetNextTargetPatrolPoint(const TArray<AActor*>& PatrolPoints)
{
	return PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)];
}

UPatrolComponent::UPatrolComponent()
{
	PatrolBehaviorClass = UPatrolBehavior::StaticClass();
}

AActor* UPatrolComponent::GetNextTargetPatrolPoint()
{
	if(PatrolBehaviorClass)
	{
		if(UPatrolBehavior* PatrolBehaviorCDO = PatrolBehaviorClass->GetDefaultObject<UPatrolBehavior>())
		{
			return PatrolBehaviorCDO->GetNextTargetPatrolPoint(PatrolPoints);
		}
	}

	return nullptr;
}