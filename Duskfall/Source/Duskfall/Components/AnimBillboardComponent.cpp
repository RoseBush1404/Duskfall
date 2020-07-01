// Duskfall - Michael Rose - r015676g


#include "AnimBillboardComponent.h"
#include "Components/SpriteAnimConfig.h"
#include "Engine/World.h"
#include "Components/AnimEventNotifier.h"

UAnimBillboardComponent::UAnimBillboardComponent()
{
	SetHiddenInGame(false);
	
}

void UAnimBillboardComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StartingAnimation != nullptr)
	{
		CurrentAnimation = StartingAnimation;
		if (CurrentAnimation != nullptr)
		{
			PlayAnimation(CurrentAnimation);
		}
	}
}

void UAnimBillboardComponent::PlayAnimation(USpriteAnimConfig* AnimToPlay)
{
	if (AnimToPlay == nullptr) { return; }

	if (AnimToPlay->Loops)
	{
		LoopAnimation(AnimToPlay);
	}
	else
	{
		PlayAnimationOnce(AnimToPlay);
	}
}

#pragma region Play Once Functions

void UAnimBillboardComponent::PlayAnimationOnce(USpriteAnimConfig* AnimToPlay)
{
	if (AnimToPlay == nullptr) { return; }
	if (AnimToPlay->AnimationFrames.Num() <= 0) { return; }

	CurrentAnimation = AnimToPlay;
	CurrentFrame = 0;
	float TimerDelay = (1 / CurrentAnimation->GetFPS());
	SetSprite(CurrentAnimation->AnimationFrames[0].Frame);
	GetWorld()->GetTimerManager().ClearTimer(AnimationFrameTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AnimationFrameTimerHandle, this, &UAnimBillboardComponent::ChangeToNextFrame, TimerDelay, true);
}

void UAnimBillboardComponent::ChangeToNextFrame()
{
	// if is at the last frame of the animation, stop the timer
	if ((CurrentFrame + 1 >= CurrentAnimation->AnimationFrames.Num())) { GetWorld()->GetTimerManager().ClearTimer(AnimationFrameTimerHandle); return; }

	CurrentFrame++;
	SetSprite(CurrentAnimation->AnimationFrames[CurrentFrame].Frame);

	if (CurrentAnimation->AnimationFrames[CurrentFrame].HasEvent)
	{
		//pass the event string to owner of this billboard
		if (User != nullptr)
		{
			IAnimEventNotifier* UserAnimEventNotifier = Cast<IAnimEventNotifier>(User);
			if (UserAnimEventNotifier != nullptr) //c++ layer
			{
				UserAnimEventNotifier->Execute_AnimEventHit(User, CurrentAnimation->AnimationFrames[CurrentFrame].EventName);
			}
			else if (User->GetClass() != nullptr && User->GetClass()->ImplementsInterface(UAnimEventNotifier::StaticClass())) //Blueprint layer
			{
				IAnimEventNotifier::Execute_AnimEventHit(User, CurrentAnimation->AnimationFrames[CurrentFrame].EventName);
			}
		}
	}
}

#pragma endregion contains all the fucntions fro the play once system

#pragma region Looping Animation Functions

void UAnimBillboardComponent::LoopAnimation(USpriteAnimConfig* AnimToPlay)
{
	if (AnimToPlay == nullptr) { return; }
	if (AnimToPlay->AnimationFrames.Num() <= 0) { return; }

	CurrentAnimation = AnimToPlay;
	CurrentFrame = 0;
	float TimerDelay = (1 / CurrentAnimation->GetFPS());
	SetSprite(CurrentAnimation->AnimationFrames[0].Frame);
	GetWorld()->GetTimerManager().ClearTimer(AnimationFrameTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AnimationFrameTimerHandle, this, &UAnimBillboardComponent::LoopToNextFrame, TimerDelay, true);
}

void UAnimBillboardComponent::LoopToNextFrame()
{
	// if is at the last frame of the animation, loop back around to first frame
	if ((CurrentFrame + 1 >= CurrentAnimation->AnimationFrames.Num())) { CurrentFrame = -1; }

	CurrentFrame++;
	SetSprite(CurrentAnimation->AnimationFrames[CurrentFrame].Frame);

	if (CurrentAnimation->AnimationFrames[CurrentFrame].HasEvent)
	{
		//pass the event string to owner of this billboard
		if (User != nullptr)
		{
			IAnimEventNotifier* UserAnimEventNotifier = Cast<IAnimEventNotifier>(User);
			if (UserAnimEventNotifier != nullptr) //c++ layer
			{
				UserAnimEventNotifier->Execute_AnimEventHit(User, CurrentAnimation->AnimationFrames[CurrentFrame].EventName);
			}
			else if (User->GetClass() != nullptr && User->GetClass()->ImplementsInterface(UAnimEventNotifier::StaticClass())) //Blueprint layer
			{
				IAnimEventNotifier::Execute_AnimEventHit(User, CurrentAnimation->AnimationFrames[CurrentFrame].EventName);
			}
		}
	}
}

#pragma endregion contains all the fucntions for the looping system

void UAnimBillboardComponent::StopAnimation()
{
	CurrentAnimation = nullptr;
	CurrentFrame = 0;
	GetWorld()->GetTimerManager().ClearTimer(AnimationFrameTimerHandle);
	SetSprite(nullptr);
}