// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IRifle.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class FPSGAME_API ACPlayer : public ACharacter, public IIRifle
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class ACRifle* Rifle;
	

private:
	UFUNCTION()
		void MoveForward(float Axis);
	UFUNCTION()
		void MoveRight(float Axis);
	UFUNCTION()
		void Turn(float Axis);
	UFUNCTION()
		void LookUp(float Axis);
	UFUNCTION()
		void OnRunning();
	UFUNCTION()
		void OffRunning();
	UFUNCTION()
		void OnRifle();
	UFUNCTION()
		void OnAim();
	UFUNCTION()
		void OffAim();
	UFUNCTION()
		void OnFire();
	UFUNCTION()
		void OffFire();
private:
	bool isAiming;
public:
	FORCEINLINE class ACRifle* GetRifle() override
	{
		return Rifle;
	}
private:
	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;
	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		class UCUserWidget_CrossHair* CrossHair;
public:
	virtual void OnFocus() override;
	virtual void OffFocus() override;
	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) override;
public:
	void PlayCameraShake();
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	float currentHP = 100.f;
	float maxHP = 100.f;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap", meta = (AllowPrivateAccess = true))
		class USpringArmComponent* MinimapArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap", meta = (AllowPrivateAccess = true))
		class USceneCaptureComponent2D* MinimapCamera;
	//SceneCaptureComponent : 카메라와 같지만 카메라 시점을 이미지화 하는 컴포넌트
};
