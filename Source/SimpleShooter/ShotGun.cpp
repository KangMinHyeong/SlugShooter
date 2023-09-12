// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "Ammo.h"
#include "SkillAmmo.h"

AShotGun::AShotGun()
{
	SkillSpawningPoint  = CreateDefaultSubobject<USceneComponent>(TEXT("Skill Spawning Point"));

	SkillSpawningPoint->SetupAttachment(GunMesh);
}

void AShotGun::BeginPlay()
{
    Super::BeginPlay();
}

void AShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotGun::OperatingSkiils()
{
	FVector SkillLocation = SkillSpawningPoint->GetComponentLocation();
	FRotator SkillRotation = SkillSpawningPoint->GetComponentRotation();

	ASkillAmmo* SkillActor = GetWorld()->SpawnActor<ASkillAmmo>(SkillClass, SkillLocation, SkillRotation);
    SkillActor->SetOwner(GetOwner());

}
