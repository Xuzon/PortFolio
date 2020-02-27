// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConversationAsset.generated.h"

USTRUCT(BlueprintType)
struct TOPDOWNEDITOR_API FConversationLine
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	FString text;
	UPROPERTY(EditAnywhere)
	FColor color {FColor::Black};
	UPROPERTY(EditAnywhere)
	float time = 3;
	UPROPERTY(EditAnywhere)
	int nextLineOffset = 1;
};

/**
 * 
 */
UCLASS(BlueprintType)
class TOPDOWNEDITOR_API UConversationAsset : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString name;
	UPROPERTY(EditAnywhere)
	TArray<FConversationLine> lines;
};
