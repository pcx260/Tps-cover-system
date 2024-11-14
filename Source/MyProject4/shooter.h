// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "shooter.generated.h"



class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
//class UCapsuleComponent;


UCLASS()
class MYPROJECT4_API Ashooter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Ashooter();
	//UPROPERTY(EditAnywhere, Category = "Leftcapsule", meta = (AllowPrivateAcces = "true"));
	//UCapsuleComponent*leftcapsule;

//	UPROPERTY(EditAnywhere, Category = "Rightcapsule");
	//UCapsuleComponent*rightcapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAcces ="true"));
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAcces = "true"));
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta =(AllowPrivateAccess="true"));
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* MoveActionleft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess = "true"));
	UInputAction* CoverAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* MoverightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* MoveforwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"));
	UInputAction* ProneAction;

	UPROPERTY(BlueprintReadWrite)
	bool bstop;

	UPROPERTY(EditAnywhere)
	float tracedistance;

	UPROPERTY(BlueprintReadWrite)
	bool bcover;

	UPROPERTY(BlueprintReadWrite)
	bool bstopwalk;

	UPROPERTY(BlueprintReadWrite)
	bool bstartwalk;


	UPROPERTY(BlueprintReadWrite)
	bool battack;
	FTimerHandle timer;

	UPROPERTY(BlueprintReadWrite)
	bool bcanleft;

	UPROPERTY(BlueprintReadWrite)
	bool bcanright;

	UPROPERTY(BlueprintReadWrite)
	float fmoveright;

	UPROPERTY(BlueprintReadWrite)
	bool bturn;

	UPROPERTY(BlueprintReadWrite)
	bool baim;

	UPROPERTY(BlueprintReadWrite)
	float armlenght;

	UPROPERTY(BlueprintReadWrite)
	bool bfire;

	UPROPERTY(BlueprintReadWrite)
	bool bcrouch;

	UPROPERTY(BlueprintReadWrite)
	bool bprone;

	UPROPERTY(BlueprintReadWrite)
	bool bbusy;

	UPROPERTY(BlueprintReadWrite)
	bool bdowncam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAcces = "true"));
	UMaterialInterface*bullethole;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Look(const FInputActionValue& Value);

	void move(const FInputActionValue& Value);

	void moveleft();

	void moveright();

	void sprint();

	void unsprint();

	void cover();

	void stopwalk();

	void startwalk();

	void shoot();

	void dontshoot();

	void shootdelay();

	void cleardelay();

	void moveforward();

	void unturn();

	void aim();

	void unaim();

	void zoom();

	void fcrouch();

	void prone();

	void unprone();

	void covertrace();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

//	FORCEINLINE class UCapsuleComponent* Getleftcapsule() const { return leftcapsule; }
	
};
