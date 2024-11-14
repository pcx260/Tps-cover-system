// Fill out your copyright notice in the Description page of Project Settings.


#include "shooter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/DecalComponent.h"
#include"Camera/CameraComponent.h"
#include"DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"

// Sets default values
Ashooter::Ashooter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 350.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	//UCapsuleComponent*leftcapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("leftcapsule"));
	

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	bstop = false;
	tracedistance = 100.0f;
	bcover = false;
	bstopwalk = false;
	bstartwalk = false;
	battack = false;
	bcanleft = false;
	bcanright = false;
	fmoveright = 0.f;
	bturn = false;
	baim = false;
	armlenght = 400.f;
	bfire = false;
	bcrouch = false;
	bprone = false;
	bbusy = false;
	bdowncam = false;
}

// Called when the game starts or when spawned
void Ashooter::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

void Ashooter::SetupPlayerInputComponent(UInputComponent*PlayerInputComponent) 
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		
		if (UEnhancedInputLocalPlayerSubsystem*Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	//UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	//(UEnhancedInputComponent * EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	if (UEnhancedInputComponent*EnhancedInputComponent=Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,&Ashooter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Ashooter::move);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &Ashooter::sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &Ashooter::unsprint);
		EnhancedInputComponent->BindAction(CoverAction, ETriggerEvent::Started, this, &Ashooter::cover);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &Ashooter::stopwalk);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &Ashooter::startwalk);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &Ashooter::shootdelay);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &Ashooter::cleardelay);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &Ashooter::shoot);
		EnhancedInputComponent->BindAction(MoveActionleft, ETriggerEvent::Started, this, &Ashooter::moveleft);
		EnhancedInputComponent->BindAction(MoverightAction, ETriggerEvent::Started, this, &Ashooter::moveright);
		EnhancedInputComponent->BindAction(MoveforwardAction, ETriggerEvent::Started, this, &Ashooter::moveforward);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &Ashooter::unturn);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &Ashooter::aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &Ashooter::unaim);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &Ashooter::dontshoot);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &Ashooter::fcrouch);
		EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &Ashooter::prone);
		
	}
}



void Ashooter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller!=nullptr)
	{
		
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
	}
}

void Ashooter::move(const FInputActionValue& Value)
{
	FVector2D movement = Value.Get<FVector2D>();
	//bstartwalk = false;
	if (Controller!=nullptr)
	{
		
		const FRotator Rotation = GetControlRotation();
		const FRotator yaw(0, Rotation.Yaw, 0);
		//move forward diye fonksiyon oluþturup içine at cover için
		//fmoveright = 0.f;
		//Find the Directions
		const FVector ForwardDirection = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
		 FVector cright = (GetCharacterMovement()->GetPlaneConstraintNormal()*(-1.0f));

		 FVector rightaa = cright.RightVector;
		
		if (bcover==false&&bbusy==false)
		{
			AddMovementInput(ForwardDirection, movement.Y);
		}
		
			
		
		if (bcover==false&&bbusy==false)
		{
			AddMovementInput(RightDirection, movement.X);
		}
		
		
			
		
	 
		
		//
	
		
		
		if (bcover==true && bcanleft == true && movement.X == -1.f)
		{
			AddMovementInput(RightDirection,-1.f);
		}
	
		//
		
		if (bcover==true&&bcanright==true && movement.X == 1.f)
		{
			AddMovementInput(RightDirection,1.f);
		}
		
		

	}
}

void Ashooter::moveleft()
{
	
	
	fmoveright = -1.f;

	
	
}

void Ashooter::moveright()
{
	fmoveright = 1.f;
	
}

void Ashooter::sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void Ashooter::unsprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	
}

void Ashooter::cover()
{
	
	if (bcover==false)
	{
		bturn = false;
		FHitResult hitresult;
		FCollisionQueryParams querryparam;
		querryparam.AddIgnoredActor(this);
		FVector start = GetActorLocation();
		FVector End = start + GetActorForwardVector()*tracedistance;
		
		GetWorld()->LineTraceSingleByChannel(hitresult, start, End, ECollisionChannel::ECC_GameTraceChannel1,querryparam);
		DrawDebugLine(GetWorld(),start,End,FColor::Red,false,5.0f);
		

		if (hitresult.GetActor())
		{
			bcover = true;
			
			GetCharacterMovement()->SetPlaneConstraintEnabled(true);
			GetCharacterMovement()->SetPlaneConstraintNormal(hitresult.Normal);
			float characteryaw = GetActorRotation().Yaw;
			

			
			
			
			
			
			FVector forwardvector = GetActorForwardVector();
				
				
			FVector distance = GetActorLocation() - hitresult.GetActor()->GetActorLocation();
				FVector location = GetActorLocation() + forwardvector * (distance.Length());
				SetActorLocation(location, true, nullptr, ETeleportType::TeleportPhysics);
			
				
				
				
					SetActorRotation(FRotator(0, GetCharacterMovement()->GetPlaneConstraintNormal().Rotation().Yaw, 0));
				
			
			
		}
		

	}
	else
	{
		bcover = false;
		float characteryaw = GetActorRotation().Yaw;
		GetCharacterMovement()->SetPlaneConstraintEnabled(false);
		SetActorRotation(FRotator(0, characteryaw + 180.f, 0));
		
	}
}

void Ashooter::stopwalk()
{
	bstopwalk = true;
}

void Ashooter::startwalk()
{
	bstartwalk = true;
}

void Ashooter::shoot()
{
	bfire = true;
	
	bturn = true;
	battack = true;
	FHitResult Hitresult;
	float distance = 5000.0f;
	FCollisionQueryParams querryparams;
	querryparams.AddIgnoredActor(this);
	FVector start = FollowCamera->GetComponentLocation();
	FVector end = ((FollowCamera->GetForwardVector() * distance) + start);
	GetWorld()->LineTraceSingleByChannel(Hitresult,start,end,ECollisionChannel::ECC_Camera,querryparams);
	DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 5.0f);
	
	FVector size;
	size.X = 7;
	size.Y = 7;
	size.Z = 7;
	
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), bullethole, size, Hitresult.Location, Hitresult.ImpactNormal.Rotation(), 2.f);
}

void Ashooter::dontshoot()
{

	bfire = false;
	
	battack = false;
	if (CameraBoom->TargetArmLength <= 400.f)
	{
		//baim = false;
	}
	
		
	
	
}

void Ashooter::shootdelay()
{

	

	GetWorld()->GetTimerManager().SetTimer(timer, this, &Ashooter::shoot, 0.2f,true);
}

void Ashooter::cleardelay()
{
	GetWorld()->GetTimerManager().ClearTimer(timer);
	battack = false;
	bturn = false;
}

void Ashooter::moveforward()
{
	fmoveright = 0;
}

void Ashooter::unturn()
{
	bturn = false;
}

void Ashooter::aim()
{
	
	
		baim = true;
		bUseControllerRotationYaw = true;

	
	
	

}

void Ashooter::unaim()
{
	
	
		baim = false;
		bUseControllerRotationYaw = false;
	
	
	
}

void Ashooter::zoom()
{
	if (baim == true && CameraBoom->TargetArmLength >= 200.f&&CameraBoom->TargetArmLength!=200 )
	{
		CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - 30.f;

		UKismetMathLibrary::Clamp(CameraBoom->TargetArmLength, 200.f, 400.f);
	}


	
	if (baim == false && CameraBoom->TargetArmLength <= 400.f&&CameraBoom->TargetArmLength!=400.f )
	{
		CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + 30.f;
		//FMath::Clamp(200, 200, 400);
		UKismetMathLibrary::Clamp(CameraBoom->TargetArmLength, 200.f, 400.f);
		
	}


	if (bdowncam==true&&FollowCamera->GetRelativeLocation().Z>0)
	{
		FVector camcrouch = FollowCamera->GetRelativeLocation();
		
		FollowCamera->SetRelativeLocation(FVector(camcrouch.X, camcrouch.Y, camcrouch.Z - 5));

		
	}


	if (bdowncam == false && FollowCamera->GetRelativeLocation().Z < 70)
	{
		FVector camcrouch = FollowCamera->GetRelativeLocation();

		FollowCamera->SetRelativeLocation(FVector(camcrouch.X, camcrouch.Y, camcrouch.Z + 5));
	}
}

void Ashooter::fcrouch()
{
	bbusy = true;
	if (bcrouch==false)
	{
		bdowncam = true;
		bcrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	
	}
	else
	{
		bcrouch = false;
		bdowncam = false;
	}
	
}

void Ashooter::prone()
{
	bbusy = true;
	if (bprone==false)
	{
		bprone = true;
		bdowncam = true;
		GetCharacterMovement()->MaxWalkSpeed = 75.0f;
	}
	else if(bprone==true)
	{
		bdowncam = false;
		bprone = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	
	
}

void Ashooter::unprone()
{
	
	bprone = false;
	
}

void Ashooter::covertrace()
{//left trace
	
	FVector leftplane = GetCharacterMovement()->GetPlaneConstraintNormal();
	FVector rightplane = GetCharacterMovement()->GetPlaneConstraintNormal();

FRotator aa=UKismetMathLibrary::MakeRotFromX(leftplane);
	
FVector rr = UKismetMathLibrary::GetRightVector(aa);

//leftplane.RightVector

	FVector left = rr *-50.f;
	FVector leftstart = GetActorLocation() + left;
	FVector leftend = leftstart + leftplane * -200;
	
	FHitResult lefthit;
	FCollisionQueryParams leftquerry;
	leftquerry.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), leftstart, leftend, FColor::Blue, false, 5.0f);
	GetWorld()->LineTraceSingleByChannel(lefthit, leftstart, leftend, ECollisionChannel::ECC_Camera, leftquerry);
	
	if (lefthit.GetActor())
	{
		//bcanleft = true;
		bcanright = true;
	}
	else {
		//bcanleft = false;
		bcanright = false;
	}
	
	
	FVector right = rr * 50.f;
	FVector rightstart = GetActorLocation() + right;
	FVector rightend = rightstart + leftplane * -200;
	FHitResult righthit;
	FCollisionQueryParams rightquerry;
	rightquerry.AddIgnoredActor(this);

	
	
	DrawDebugLine(GetWorld(), rightstart, rightend, FColor::Blue, false, 5.0f);
	GetWorld()->LineTraceSingleByChannel(righthit, rightstart, rightend, ECollisionChannel::ECC_Camera, rightquerry);

	if (righthit.GetActor())
	{
		
		//bcanright = true;
		
		bcanleft = true;
	}
	else
	{
		//bcanright = false;

		bcanleft = false;
	
	}
	
	//sweeptrace
	
	DrawDebugCapsule(GetWorld(), GetActorLocation(), 30.f, 100.f, GetActorQuat(), FColor::Red, false, 0.1f);
	
	FHitResult sweephit;
	FCollisionShape shape;
	FCollisionQueryParams querrycapsule;
	querrycapsule.AddIgnoredActor(this);
	FVector end = GetActorLocation() + (GetActorForwardVector() *-15);
	GetWorld()->SweepSingleByChannel(sweephit, GetActorLocation(), end, GetActorQuat(), ECollisionChannel::ECC_Camera, shape.MakeCapsule(50.0f,30.0f), querrycapsule);
	//UKismetMathLibrary::MakeRotFromX()
	if (sweephit.GetActor()&&lefthit.GetActor()&&righthit.GetActor())
	{
		GetCharacterMovement()->SetPlaneConstraintNormal(sweephit.Normal);
		SetActorLocation(FVector(sweephit.Location), true, nullptr);
		SetActorRotation(FRotator(0, righthit.Normal.Rotation().Yaw, 0));

		
	}
	

}



// Called every frame
void Ashooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GetVelocity().Size() > 0 && bcover == false || battack == true

	if (bturn==true&&bcover==false || GetVelocity().Size() > 0 && bcover == false)
	{
		SetActorRotation(FRotator(0, GetControlRotation().Yaw, 0));
	}
	
	zoom();

	if (bcover==true)
	{
		covertrace();
	}
	
}




