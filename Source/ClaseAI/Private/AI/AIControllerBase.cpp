// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaseAI/Public/AI/AIControllerBase.h"

#include "AI/NPCBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


AAIControllerBase::AAIControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehavioirTreeComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBaordComponent"));
	
}


void AAIControllerBase::SetMoveCommand(const FVector& InGoal)
{
	if(GetCharacter()->GetVelocity().Length() > 0) return;
		
	if(GetCharacter())
	{
		FAIMoveRequest MoveRequest;
		MoveRequest
			.SetAcceptanceRadius(50.f)
			.SetCanStrafe(true)
			.SetUsePathfinding(true)
			.SetGoalLocation(InGoal);

		FNavPathSharedPtr Path;

		FPathFollowingRequestResult RequestID = MoveTo(MoveRequest, &Path);

		GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ThisClass::MoveFinished);
	}
}

void AAIControllerBase::SetMoveCommand(const FVector& InGoal, bool bAbortMovement)
{

	if(bAbortMovement) GetPathFollowingComponent()->AbortMove(*this,FPathFollowingResultFlags::UserAbort);
	
	SetMoveCommand(InGoal);
}

void AAIControllerBase::ResumeMovement_Implementation()
{
	ensure(GetPathFollowingComponent());
	GetPathFollowingComponent()->ResumeMove();
}

void AAIControllerBase::StopMovement_Implementation()
{
	ensure(GetPathFollowingComponent());
	GetPathFollowingComponent()->PauseMove();
}


void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ANPCBase* NPC = Cast<ANPCBase>(InPawn))
	{
		if(UBlackboardData* BlackboardData = NPC->GetDefaultBlakboard())
		{
			Blackboard->InitializeBlackboard(*BlackboardData);
		}
		
		if(UBehaviorTree* BT = NPC->GetDefaultBehaviour())
		{
			RunBehaviorTree(BT);
		}

		//Blackboard->SetValueAsObject(GET_MEMBER_NAME_CHECKED(ANPCBase, PatrolPointOrigin), NPC->PatrolPointOrigin);
		//Blackboard->SetValueAsObject(GET_MEMBER_NAME_CHECKED(ANPCBase, PatrolPointDestiny), NPC->PatrolPointDestiny);
		
	}

	if(AActor* Ref = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass()))
	{
		Blackboard->SetValueAsVector(TEXT("GoalLocation"), Ref->GetActorLocation());		
	}

	
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if(Result.IsSuccess())
	{
		
	}else if (Result.IsFailure())
	{
		
	}else if(Result.IsInterrupted())
	{
		
	}
}

void AAIControllerBase::MoveFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult) const
{
		GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
}