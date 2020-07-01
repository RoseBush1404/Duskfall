// Duskfall - Michael Rose - r015676g

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "TimerManager.h"
#include "AnimBillboardComponent.generated.h"

/**
 * 
 */

class USpriteAnimConfig;

UCLASS(ClassGroup = Rendering, hidecategories = (Object, Activation, "Components|Activation", Physics, Collision, Lighting, Mesh, PhysicsVolume), meta = (BlueprintSpawnableComponent))
class DUSKFALL_API UAnimBillboardComponent : public UBillboardComponent
{
	GENERATED_BODY()

		UAnimBillboardComponent();

//valus
public:

protected:

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Animations")
		USpriteAnimConfig* StartingAnimation;
		
	USpriteAnimConfig* CurrentAnimation = nullptr;

	int CurrentFrame = -1;

	FTimerHandle AnimationFrameTimerHandle;

	AActor* User;

private:


//functions
public:

	UFUNCTION(BlueprintCallable, Category = "Animation Controls")
		void PlayAnimation(USpriteAnimConfig* AnimToPlay);

	UFUNCTION(BlueprintCallable, Category = "Animation Controls")
		void PlayAnimationOnce(USpriteAnimConfig* AnimToPlay);

	UFUNCTION(BlueprintCallable, Category = "Animation Controls")
		void LoopAnimation(USpriteAnimConfig* AnimToPlay);

	UFUNCTION(BlueprintCallable, Category = "Animation Controls")
		void StopAnimation();

	void SetUser(AActor* NewUser) { User = NewUser; }

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void ChangeToNextFrame();

	UFUNCTION()
		void LoopToNextFrame();

private:

};
