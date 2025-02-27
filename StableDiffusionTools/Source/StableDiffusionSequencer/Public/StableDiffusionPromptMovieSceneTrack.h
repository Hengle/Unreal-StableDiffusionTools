#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNameableTrack.h"
#include "StableDiffusionPromptMovieSceneTrack.generated.h"

UCLASS()
class STABLEDIFFUSIONSEQUENCER_API UStableDiffusionPromptMovieSceneTrack : public UMovieSceneNameableTrack
{
	GENERATED_BODY()
	UStableDiffusionPromptMovieSceneTrack(const FObjectInitializer& ObjectInitializer);

public:

	/*virtual FText GetDisplayName() const override;
	virtual FName GetTrackName() const override;*/

	virtual bool IsEmpty() const override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;

	//Basic section operations: 
	virtual class UMovieSceneSection* CreateNewSection() override;
	virtual void AddSection(class UMovieSceneSection& Section) override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;


	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;


	//static const FName GetDataTrackName();

	UPROPERTY(BlueprintReadWrite, Category = "StableDiffusion|Sequencer")
	TArray<UMovieSceneSection*> Sections;

private:
	FName TrackName;
};
