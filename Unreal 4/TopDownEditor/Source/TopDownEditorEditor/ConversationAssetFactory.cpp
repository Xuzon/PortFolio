// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationAssetFactory.h"
#include "TopDownEditor/ConversationAsset.h"
#include "FeedbackContext.h"

UConversationAssetFactory::UConversationAssetFactory() 
{
    SupportedClass = UConversationAsset::StaticClass(); 
    bCreateNew = true; 
    bEditAfterNew = true;
}

UObject* UConversationAssetFactory::FactoryCreateNew
	(
		UClass* inClass,
		UObject* inParent,
		FName inName,
		EObjectFlags flags,
		UObject* Context,
		FFeedbackContext* warn
	)
{

    UObject* toRet = nullptr;
    if( warn->YesNof(FText::FromString(TEXT("Are you sure?"))))
    {
        toRet = NewObject<UConversationAsset>(inParent, inClass, inName,flags);
    }
    return toRet;
}