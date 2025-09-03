// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Controller/CounterStrikePlayerController.h"
#include "Animation/PlayerAnimInstance.h"
#include "Component/PlayerStatComponent.h"
#include "Component/PlayerMoveComponent.h"
#include "Component/PlayerShootingComponent.h"
#include "Component/PlayerUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/ChatMain.h"
#include "Guns/Knife.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/FPSPawn/Meshes/AK_mesh.AK_mesh'"));
	if (WeaponMesh.Succeeded())
	{
		WeaponMeshComponent->SetStaticMesh(WeaponMesh.Object);
	}
	WeaponMeshComponent->SetWorldScale3D(FVector(0.01, 0.01, 0.01));
	WeaponMeshComponent->SetOwnerNoSee(true);
	WeaponMeshComponent->SetVisibility(true);
	WeaponMeshComponent->SetupAttachment(GetMesh(), FName("arm-right-wristSocket"));

	StatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));
	MoveComponent = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("MoveComponent"));
	ShootingComponent = CreateDefaultSubobject<UPlayerShootingComponent>(TEXT("ShootingComponent"));
	UIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("UIComponent"));
	GunComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Guncomponent"));

	GetCameraBoom()->TargetArmLength = 0;
	GetCameraBoom()->SetRelativeLocation(FVector(0, 0, 90));

	HandComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandComponent"));
	HandComponent->SetupAttachment(GetFollowCamera());
	HandComponent->SetRelativeLocation(FVector(9, 8.8, -23.6));
	HandComponent->SetRelativeRotation(FRotator(0, 6.4, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshHand(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPSPawn/Meshes/FPSArms_rigged.FPSArms_rigged'"));

	if(MeshHand.Succeeded())
	{
		HandComponent->SetSkeletalMesh(MeshHand.Object);
	}

	GunComponent->SetupAttachment(HandComponent, TEXT("Palm_R"));

	GetMesh()->SetHiddenInGame(true);

	// GDH CODE 
	UITargeting = CreateDefaultSubobject<UWidgetComponent>(TEXT("UITargeting"));
	UITargeting->SetupAttachment(FollowCamera);
	// GDH CODE

	UE_LOG(LogTemp, Warning, TEXT("Constructor"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (UIComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents"));

		UIComponent->Player = this;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		/*EnhancedInputComponent->BindAction(IA_Walk, ETriggerEvent::Triggered, this, &ACharacter::Walk);
		EnhancedInputComponent->BindAction(IA_Walk, ETriggerEvent::Completed, this, &ACharacter::Walk);

		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &ACharacter::Crouch);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &ACharacter::Crouch);*/

		// Shooting
		ShootingComponent->SetupPlayerInputComponent(PlayerInputComponent);

		// Moving
		MoveComponent->SetupPlayerInputComponent(PlayerInputComponent);

		//UI
		UIComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController"));

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	//서버 플레이어는 PossessedBy에서 InitWidget() 호출할 예정, 서버 빼고 실행시키도록 설계
	if (IsLocallyControlled() && !HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsLocallyControlled() && !HasAuthority()"));

		UIComponent->InitWidget();
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy Begin, Has controller : %s"), Controller ? TEXT("Yes") : TEXT("No"));
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Warning, TEXT("PossessedBy ing , Has controller : %s"), Controller ? TEXT("Yes") : TEXT("No"));
	//PossessedBy는 서버에서만 실행되므로 자신 컨트롤러가 있는지만 확인하고, InitWidget() 실행
	if (IsLocallyControlled())
	{
		UIComponent->InitWidget();
	}
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy End , Has controller : %s"), Controller ? TEXT("Yes") : TEXT("No"));
}

void APlayerCharacter::PostNetInit()
{
	Super::PostNetInit();

	UE_LOG(LogTemp, Warning, TEXT("PostNetInit"));
}
