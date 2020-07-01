// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpriteAnimConfig.generated.h"

/**
 * 
 */

class UTexture2D;

USTRUCT(BlueprintType)
struct FAnimFrameInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* Frame;

	UPROPERTY(EditDefaultsOnly)
		bool HasEvent = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "HasEvent"))
		FString EventName;
};

UCLASS(ClassGroup = Animation, abstract, EditInlineNew, Blueprintable)
class DUSKFALL_API USpriteAnimConfig : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Aniamtion Info")
		int FramesPerSecond = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Animation Info")
		bool Loops = false;

	UPROPERTY(EditDefaultsOnly, Category = "Aniamtion Info")
		TArray<FAnimFrameInfo> AnimationFrames;

public:

	float GetFPS() { return FramesPerSecond; }
};
