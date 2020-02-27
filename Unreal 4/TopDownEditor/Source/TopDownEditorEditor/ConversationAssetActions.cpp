#include "ConversationAssetActions.h"

FText FConversationAssetActions::GetName() const
{
	return FText::FromString(TEXT("A conversation asset"));
}
UClass* FConversationAssetActions::GetSupportedClass() const
{
	return UConversationAsset::StaticClass();
}
FColor FConversationAssetActions::GetTypeColor() const
{
	return FColor::Emerald;
}
uint32 FConversationAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc | EAssetTypeCategories::UI;
}
FText FConversationAssetActions::GetAssetDescription(const FAssetData& assetData) const
{
	return FText::FromString(assetData.AssetName.ToString() +  TEXT("Es un tipo de asset de conversacion") );
}