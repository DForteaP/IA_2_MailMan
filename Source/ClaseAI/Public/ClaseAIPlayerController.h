// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "ClaseAIPlayerController.generated.h"

class ANPCBase;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AClaseAIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AClaseAIPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputRTS, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* RTSMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputRTS, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SelectAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=InputRTS, meta=(AllowPrivateAccess = "true"))
	class UInputAction* OrderAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=InputRTS, meta=(AllowPrivateAccess = "true"))
	TArray<ACharacter*> NPCSArray;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	void SelectNPC();
	void MoveNPC();

private:
	FVector CachedDestination;

	bool bIsTouch; 
	float FollowTime;
};


