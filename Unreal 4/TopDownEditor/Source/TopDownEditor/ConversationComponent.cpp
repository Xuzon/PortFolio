// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UConversationComponent::UConversationComponent() : currentLine(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConversationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UConversationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UConversationComponent::ShowCurrentLine()
{
	if(conversation == nullptr)
	{
		return;
	}
	//C++ reference, evita copia
	FConversationLine& line = conversation->lines[currentLine];
	GEngine->AddOnScreenDebugMessage(-1,line.time, line.color, line.text);
}

void UConversationComponent::AdvanceConversation()
{
	if(conversation == nullptr)
	{
		return;
	}
	int nextLine = currentLine + conversation->lines[currentLine].nextLineOffset;
	if(nextLine < 0 || nextLine >= conversation->lines.Num())
	{
		return;
	}
	currentLine = nextLine;
}
