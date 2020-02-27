// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlFile.h"
#include "Factories/Factory.h"
#include "ConversationAsset.h"
#include "ConversationAssetXmlFactory.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNEDITOREDITOR_API UConversationAssetXmlFactory : public UFactory
{
	GENERATED_BODY()
public:
	UConversationAssetXmlFactory();
	UObject* FactoryCreateFile
	(
		UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& FileName,
		const TCHAR* Parms, FFeedbackContext* warn, bool& bOutOperationCanceled
	)	override;
};
