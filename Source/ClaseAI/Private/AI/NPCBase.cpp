// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaseAI/Public/AI/NPCBase.h"
#include "AI/AIControllerBase.h"

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

ANPCBase::ANPCBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIControllerBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PatrolComponent = CreateDefaultSubobject<UPatrolComponent>(TEXT("Patrol Component"));
}

