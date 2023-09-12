// Fill out your copyright notice in the Description page of Project Settings.


#include "MiddleBoss.h"

AMiddleBoss::AMiddleBoss()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AMiddleBoss::BeginPlay()
{
    Super::BeginPlay();
}

void AMiddleBoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMiddleBoss::StunAttack()
{
	if(!bStun)
	{
		FHitResult HitResult;
		FVector HitRotate;
		bool Line = AttackTrace(HitResult, HitRotate);

		if(Line)
		{
			if(AActor* Actor = HitResult.GetActor())
			{
				FPointDamageEvent DamageEvent(StunDamage, HitResult, HitRotate, nullptr);
				AController* MyController = Cast<AController>(GetController());
				if(MyController == nullptr) return;
				Actor->TakeDamage(StunDamage, DamageEvent, MyController, this);
                AShooterCharacter* Character = Cast<AShooterCharacter>(Actor);
                if(Character != nullptr)
                {
                    Character->PlayerStun(GiveStunTime);
                }
			}
		}
		bStunAttack = true;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StunAttackEffect, End, FRotator::ZeroRotator, StunAttackEffectScale);
		UGameplayStatics::SpawnSoundAttached(StunAttackSound, GetMesh(), TEXT("StunAttackSound"));
		GetWorldTimerManager().SetTimer(StunAttackTimerHandle, this, &AMiddleBoss::StopStunAttackMotion, StunAttackTime, false);
	}
}

void AMiddleBoss::ShieldMode()
{
	bShieldMode = true;
	bGodMode = true;
	UGameplayStatics::SpawnSoundAttached(ShieldModeSound, GetMesh(), TEXT("ShieldModeSound"));
	GetWorldTimerManager().SetTimer(ShieldModeTimerHandle, this, &AMiddleBoss::ResetShieldMode, ShieldModeTime, false);
}

void AMiddleBoss::VelocityUp()
{
	GetCharacterMovement()->MaxWalkSpeed = RushVelocity;
}

void AMiddleBoss::CrushAttack()
{
	bCrushAttack = true;
	GetWorldTimerManager().SetTimer(CrushAttackTimerHandle, this, &AMiddleBoss::ShieldAttack, CrushAttackTime, false);
}

bool AMiddleBoss::IsStunAttack()
{
    return bStunAttack;
}

bool AMiddleBoss::IsShieldMode()
{
    return bShieldMode;
}

bool AMiddleBoss::IsCrushAttack()
{
    return bCrushAttack;
}

void AMiddleBoss::StopStunAttackMotion()
{
    bStunAttack = false;
    GetWorldTimerManager().ClearTimer(StunAttackTimerHandle);
}

void AMiddleBoss::ResetShieldMode()
{
	bShieldMode = false;
	bGodMode = false;
	GetWorldTimerManager().ClearTimer(ShieldModeTimerHandle);
}

void AMiddleBoss::ShieldAttack()
{
	if(!bStun)
	{
		FHitResult HitResult;
		FVector HitRotate;
		bool Line = AttackTrace(HitResult, HitRotate);

		if(Line)
		{
			if(AActor* Actor = HitResult.GetActor())
			{
				FPointDamageEvent DamageEvent(StunDamage, HitResult, HitRotate, nullptr);
				AController* MyController = Cast<AController>(GetController());
				if(MyController == nullptr) return;
				Actor->TakeDamage(StunDamage, DamageEvent, MyController, this);
                AShooterCharacter* Character = Cast<AShooterCharacter>(Actor);
                if(Character != nullptr)
                {
                    Character->PlayerKnockBack(GetActorForwardVector() * KnockBackDist, true, 1.5f);
                }
			}
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldAttackEffect, End, FRotator::ZeroRotator, ShieldAttackEffectScale);
		UGameplayStatics::SpawnSoundAttached(CrushAttackSound, GetMesh(), TEXT("CrushAttackSound"));
		GetWorldTimerManager().SetTimer(CrushAttackTimerHandle, this, &AMiddleBoss::ResetShieldAttack, ShieldAttackTime, false);
	}
}

void AMiddleBoss::ResetShieldAttack()
{
	bCrushAttack= false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultVelocity;
	GetWorldTimerManager().ClearTimer(CrushAttackTimerHandle);
}
