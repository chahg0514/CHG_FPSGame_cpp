// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRifle.generated.h"

UCLASS()
class FPSGAME_API ACRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HandSocket = TEXT("Hand_Rifle");
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HolsterSocket = TEXT("Holster_Rifle");
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class ACharacter* OwnerCharacter;
	
	//IIRifle* Rifle;
public:
	static ACRifle* Spawn(class UWorld* InWorld, class ACharacter* InOwner);
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* GrabMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* UnGrabMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* FireMontage;
private:
	bool isEquipped; //장착여부
	bool isEquipping; //장착하고있는 중인지 여부
	bool isAiming;
	
public:
	void Equip();
	void BeginEquip();
	void EndEquip();

	void UnEquip();
	void BeginUnEquip();
	void EndUnEquip();
	
public:
	FORCEINLINE bool GetIsEquipped() { return isEquipped; }
	FORCEINLINE bool GetIsEquipping() { return isEquipping; }
	FORCEINLINE bool GetIsAiming() { return isAiming; } //유니티 프로퍼티처럼 사용하는거인듯
	FORCEINLINE void SetIsAiming(bool bAiming)
	{
		isAiming = bAiming;
	}
private:
	bool isFiring;
public:
	void BeginFire();
	void Firing();
	void EndFire();
	
private:
	bool isTarget;
	UPROPERTY(VisibleAnywhere, Category = "Target")
		class AActor* Target;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* MuzzleParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* ImpactParticle;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class USoundCue* FireSoundCue;
	UPROPERTY(VIsibleDefaultsOnly, Category = "BulletMark")
		class UMaterialInstanceConstant* DecalMaterial;
public:
	FORCEINLINE FVector GetMuzzleLocation() const
	{
		return Mesh->GetSocketLocation("Muzzle");
	}

};
