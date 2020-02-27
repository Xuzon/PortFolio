// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ConversationAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNEDITOREDITOR_API UConversationAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UConversationAssetFactory();

	UObject* FactoryCreateNew
	(
		UClass* inClass,
		UObject* inParent,
		FName inName,
		EObjectFlags flags,
		UObject* Context,
		FFeedbackContext* warn
	) override;
};
