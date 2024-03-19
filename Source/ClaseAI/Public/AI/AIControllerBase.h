// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UAIMovementInterface : public UInterface
{
	GENERATED_BODY()
};

class IAIMovementInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movement")
	void ResumeMovement();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Movement")
	void StopMovement();
};


UCLASS()
class AAIControllerBase : public AAIController, public IAIMovementInterface
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	void SetMoveCommand(const FVector& InGoal);
	void SetMoveCommand(const FVector& InGoal, bool bAbortMovement);

	virtual void ResumeMovement_Implementation() override;
	virtual void StopMovement_Implementation() override;
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
	void MoveFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult) const;
};