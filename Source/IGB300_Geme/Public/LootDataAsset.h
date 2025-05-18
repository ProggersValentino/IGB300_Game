#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "LootDataAsset.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
    Rare        UMETA(DisplayName = "Rare")
};
USTRUCT(BlueprintType)
struct FLootAssetInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AssetName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* AssetThumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    ERarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
    TSubclassOf<UGameplayEffect> GameplayEffect;


};

UCLASS(BlueprintType)
class IGB300_GEME_API ULootDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FLootAssetInfo> AssetItems;
};
