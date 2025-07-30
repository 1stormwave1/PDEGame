


#include "Building/BehaviourSaveGame.h"

#include "JsonObjectConverter.h"

void UBehaviourSaveGame::WriteJson(FString JsonFilePath)
{
	FString JsonString;

	JsonString.Append("[\n");
	for(int32 i = 0; i < BehaviourData.Num(); ++i)
	{
		if(const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(BehaviourData[i]))
		{
			FString StructInstanceString;
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&StructInstanceString, 0));

			JsonString.Append(StructInstanceString);
			if(i < BehaviourData.Num() - 1)
			{
				JsonString.Append(",\n");
			}
			else
			{
				JsonString.Append("\n]");
			}
		}
	}

	const FString FilePath = FPaths::ProjectPersistentDownloadDir() + TEXT("/") + JsonFilePath;
	FFileHelper::SaveStringToFile(JsonString, *FilePath);
}
