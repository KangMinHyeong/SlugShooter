// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBox.h"
#include "Components/ActorComponent.h"


// Sets default values
AGunBox::AGunBox()
{

	Box = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BOX"));

	Box->SetCollisionProfileName(TEXT("NoCollision"));


}

// Called when the game starts or when spawned
void AGunBox::BeginPlay()
{
	Super::BeginPlay();
	
}

