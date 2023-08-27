// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileEnemy.generated.h"

UCLASS()
class FPSGAME_API AProjectileEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectileEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//ApplyDamage 함수를 통해 데미지를 전달받는 함수
private:
	UPROPERTY(VisibleAnywhere, Category = "State")
		float currentHP = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "State")
		float maxHP = 100.f;
private:
	UPROPERTY(VisibleAnywhere, Category = "Rifle")
		class ACRifle* Rifle;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* HitMtg;
public:
	FORCEINLINE class ACRifle* GetRifle() const { return Rifle; } //Getter 같은 경우에 const 붙여주면 좋음

private:
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		class UAnimMontage* FireMontage;
public:
	void PlayAttackMontage();
public:
	void EnemyFire();

};
