


#include "Building/GeneticAlgorithmSave.h"

void UGeneticAlgorithmSave::GetTraitsByRoomType(TArray<FSavedRoomTraits>& OutTraits, ERoomTypeEnum RoomType)
{
	for(const FSavedRoomTraits& Traits : SavedRoomTraits)
	{
		if(Traits.RoomType == RoomType)
		{
			OutTraits.Add(Traits);
		}
	}
}

void UGeneticAlgorithmSave::AddRoomTraitsToSave(URoomTraits* RoomTraits)
{
	FSavedRoomTraits NewSavedRoomTraits;
	NewSavedRoomTraits.RoomTraits = RoomTraits->Traits;
	NewSavedRoomTraits.RoomType = RoomTraits->RoomType;

	SavedRoomTraits.Add(NewSavedRoomTraits);
}
