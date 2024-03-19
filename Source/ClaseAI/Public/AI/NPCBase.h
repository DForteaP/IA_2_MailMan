// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

class UPatrolComponent;
class UBehaviorTree;
class AAIControllerBase;

UCLASS()
class CLASEAI_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="AIData", meta=(AllowPrivateAccess = true))
	UBehaviorTree* BehaviourTreeAsset;

	UPROPERTY(EditDefaultsOnly,Category="AIData", meta=(AllowPrivateAccess = true))
	UBlackboardData* BlackboardData;

	UPROPERTY(EditDefaultsOnly,Category="AIData", meta=(AllowPrivateAccess = true))
	AActor* PatrolPointOrigin;

	UPROPERTY(EditDefaultsOnly,Category="AIData", meta=(AllowPrivateAccess = true))
	AActor* PatrolPointDestiny;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AIData", meta=(AllowPrivateAccess = true))
	TArray<AActor*> ArrayPointsPatrol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AIData", meta=(AllowPrivateAccess = true))
	UPatrolComponent* PatrolComponent;
public:
	
	ANPCBase();

	UPROPERTY(EditInstanceOnly)
	AActor* ReferenceActor;

	UBehaviorTree* GetDefaultBehaviour() const {return BehaviourTreeAsset;}
	UBlackboardData* GetDefaultBlakboard() const {return BlackboardData;}
	
protected:

	virtual void BeginPlay() override;

};
