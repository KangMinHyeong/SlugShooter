// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterStat.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerState.h"

// Sets default values for this component's properties
UShooterCharacterStat::UShooterCharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShooterCharacterStat::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AShooterCharacter>(GetOwner());

}

void UShooterCharacterStat::InitializeComponent()
{	
	Super::InitializeComponent();
	
	CurrentHP = Character->MaxHP;
}

// Called every frame
void UShooterCharacterStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UShooterCharacterStat::SetHP(float HP)
{
	CurrentHP = HP;
	UE_LOG(LogTemp, Display, TEXT("Remain HP : %f"), CurrentHP);

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(ShooterCharacter == nullptr) 
	{
		UE_LOG(LogTemp, Display, TEXT("Your Character"));
		return CurrentHP;
	}
	
    AShooterPlayerState* MyPlayerState = Cast<AShooterPlayerState>(ShooterCharacter->GetController()->PlayerState);
    if(MyPlayerState == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Your wat"));
        return CurrentHP;
    }
	return CurrentHP;
}
