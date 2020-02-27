// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationAssetXmlFactory.h"
#include "FeedbackContext.h"

UConversationAssetXmlFactory::UConversationAssetXmlFactory()
{
	SupportedClass = UConversationAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
	bEditAfterNew = true;
	Formats.Add(TEXT("xml;xml conversation file"));
}

UObject* UConversationAssetXmlFactory::FactoryCreateFile
(
	UClass* inClass, UObject* inParent, FName inName, EObjectFlags flags, const FString& fileName,
	const TCHAR* parms, FFeedbackContext* warn, bool& bOutOperationCanceled
)
{
	UConversationAsset* toRet = nullptr;
	toRet = NewObject<UConversationAsset>(inParent, inClass, inName, flags);
	FXmlFile xml(fileName, EConstructMethod::ConstructFromFile);
	FXmlNode* root = xml.GetRootNode();
	if (root == nullptr)
	{
		return toRet;
	}

	FString assetName = root->GetAttribute(FString("name"));
	toRet->name = assetName;
	const FXmlNode* linesNode = root->FindChildNode(FString("Lines"));
	for (auto node : linesNode->GetChildrenNodes())
	{
		FConversationLine line;
		FString durationString = node->GetAttribute("duration");
		line.time = FCString::Atof(*durationString);

		FString colorString = node->GetAttribute("color");
		line.color = FColor::FromHex(*colorString);

		FString timeString = node->GetAttribute("offset");
		line.nextLineOffset = FCString::Atoi(*timeString);

		line.text = node->GetContent();

		toRet->lines.Add(line);
	}
	return toRet;
}