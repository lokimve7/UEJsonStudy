// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonTest.h"


// Sets default values
AJsonTest::AJsonTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonTest::BeginPlay()
{
	Super::BeginPlay();

	LoadData();
}

void AJsonTest::SaveData()
{
	/*FString userName = TEXT("김현진");
	int32 userAge = 27;*/
	TArray<FString> interests = { TEXT("게임"), TEXT("키보드"), TEXT("주식") };
	//bool isWoman = false;

	FUserInfo info;
	info.name = TEXT("김현진");
	info.age = 27;
	for (int32 i = 0; i < 3; i++)
	{
		info.interests.Add(interests[i]);
	}
	info.isWoman = false;


	// 변수에 담겨있는 데이터를 Json Object 로 변경	
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
	jsonObject->SetStringField("userName", info.name);
	jsonObject->SetNumberField("userAge", info.age);
	jsonObject->SetBoolField("isWoman", info.isWoman);

	//FString -> FJsonValue 로 변환해서 jsonArray 셋팅
	TArray<TSharedPtr<FJsonValue>> arrayInterests;
	for (int32 i = 0; i < info.interests.Num(); i++)
	{
		arrayInterests.Add(MakeShareable(new FJsonValueString(info.interests[i])));
	}
	jsonObject->SetArrayField("interests", arrayInterests);

	// Json Object 를 문자열(string) 로 변경
	FString jsonString;
	TSharedRef<TJsonWriter<>> jsonWrite = TJsonWriterFactory<>::Create(&jsonString);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWrite);

	UE_LOG(LogTemp, Warning, TEXT("save : %s"), *jsonString);

	// Json 형태로 변경된 문자열(jsonString) 을 파일로 저장
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
	FFileHelper::SaveStringToFile(jsonString, *filePath);
}

void AJsonTest::LoadData()
{
	FString jsonString;
	//파일을 읽어오자
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
	FFileHelper::LoadFileToString(jsonString, *filePath);
	UE_LOG(LogTemp, Warning, TEXT("load : %s"), *jsonString)

	//string -> json object 로 변환
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		FString userName = jsonObject->GetStringField("userName");
		int32 userAge = jsonObject->GetNumberField("userAge");

		TArray<FString> interests;
		TArray<TSharedPtr<FJsonValue>> arrayInterests = jsonObject->GetArrayField("interests");
		for (int32 i = 0; i < arrayInterests.Num(); i++)
		{
			interests.Add(arrayInterests[i]->AsString());
		}
		
		bool isWoman = jsonObject->GetBoolField("isWoman");

		UE_LOG(LogTemp, Warning, TEXT("userName : %s"), *userName);
		UE_LOG(LogTemp, Warning, TEXT("userAge : %d"), userAge);
		UE_LOG(LogTemp, Warning, TEXT("userName : %d"), isWoman);
		for (int32 i = 0; i < interests.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("interests : %s"), *interests[i]);
		}
	}
}

// Called every frame
void AJsonTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

