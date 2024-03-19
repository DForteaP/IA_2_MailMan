// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NavManager.h"
#include "AI/AIControllerBase.h"
#include "GameFramework/Character.h"


// Sets default values
ANavManager::ANavManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANavManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANavManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANavManager::SendCommand()
{
	if(TargetNavActor)
	{
		if(AAIControllerBase* Controller = Cast<AAIControllerBase>(TargetNavActor->GetController()))
		{
			Controller->SetMoveCommand(Goal + GetActorLocation());
		}
	}
}


