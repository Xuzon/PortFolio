#include "ConversationAsset.h"
#include "Public/AssetTypeActions_Base.h"

class TOPDOWNEDITOREDITOR_API FConversationAssetActions : public FAssetTypeActions_Base
{
	FText GetName() const override;
	UClass* GetSupportedClass() const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;
	FText GetAssetDescription(const FAssetData& assetData) const;
};