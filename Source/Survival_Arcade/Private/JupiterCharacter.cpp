#include "JupiterCharacter.h"
#include "JupiterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJupiterCharacter::AJupiterCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	// USpringArmComponent 타입의 기본 컴포넌트 객체를 하나 생성한다.
	// 그 객체 이름은 SpringArmComp이고, 이걸 SpringArmComp에 저장하라.
	SpringArmComp->SetupAttachment(RootComponent);
	// 그리고 이걸 캡슐 컴포넌트(루트 컴포넌트)에 붙인다. 
	SpringArmComp->TargetArmLength = 300.0f;
	// 스프링암의 길이를 300으로 설정한다.
	SpringArmComp->bUsePawnControlRotation = true;
	// 스프링암이 플레이어 컨트롤러의 회전을 따라가도록 설정한다.

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 위와 같이 카메라 객체 생성
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// 그리고 카메라를 스프링암에 붙인다. 
	// USpringArmComponent::SocketName은 스프링암의 끝부분에 카메라를 붙이겠다는 의미이다.
	CameraComp->bUsePawnControlRotation = false;
	// 카메라까지 같이 움직이면 회전값 2배 이벤트

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	// 초기 체력 설정
	MaxHealth = 100.0f;
	Health = MaxHealth;
}


void AJupiterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	// 캐스팅(Casting)은 쉽게 말해 
	// 이 객체가 내가 원하는 클래스의 객체인지 확인하고, 맞다면 그 타입으로 다루겠다는 의미.
	// 여기서는 PlayerInputComponent를 UEnhancedInputComponent로 캐스팅 가능한지 확인하고, 
	// 성공하면 그 결과를 EnhancedInput이라는 지역 포인터 변수에 담는다. 
	// 그 포인터가 유효하면 if문 안에서 EnhancedInput을 통해 Enhanced Input 기능을 사용한다.
	{
		if (AJupiterPlayerController* PlayerController = Cast<AJupiterPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					// IA를 가져옴.
					ETriggerEvent::Triggered,
					// 키가 눌려서 이벤트가 발생했을 때
					this,
					&AJupiterCharacter::Move
				);
				// 실제로 이벤트와 함수를 연결하는 게 핵심 포인트.
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AJupiterCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AJupiterCharacter::StopJump
				);
			}
			//Jump는 2가지가 있으므로 Triggered와 Completed를 모두 연결해준다.

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AJupiterCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AJupiterCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AJupiterCharacter::StopSprint
				);
			}
		}
	}
}

void AJupiterCharacter::Move(const FInputActionValue& value)
{
	if(!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	//MoveInput에다가 FVector2D 형태로 저장

	if (!FMath::IsNearlyZero(MoveInput.X))	//0이 아닐 때 처리해주는 안전 코드
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
		//정면 방향으로 X만큼 이동시켜 주세요.
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))	//0이 아닐 때 처리해주는 안전 코드
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
		//좌우 방향으로 Y만큼 이동시켜 주세요.
	}
}
//GetActorForwardVector나 GetActorRightVector를 쓰려면 Controller 체크가 필수.

void AJupiterCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
//점프 함수가 실제로 캐릭터 무브먼트에서 제공하는 함수
}

void AJupiterCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
	// StopJumping 함수도 Character가 기본 제공
}

void AJupiterCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴.
	AddControllerYawInput(LookInput.X);
	// 좌우 회전
	AddControllerPitchInput(LookInput.Y);
	// 상하 회전
}

void AJupiterCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AJupiterCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
// 스프린트는 기존 동작과 조금 다르므로 추가 변수 필요.

// 체력 회복 함수
void AJupiterCharacter::AddHealth(float Amount)
{
	// 체력을 회복시킴. 최대 체력을 초과하지 않도록 제한함
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), Health);
}

int AJupiterCharacter::GetHealth() const
{
	return Health;
}

// 데미지 처리 함수
float AJupiterCharacter::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator, 
	AActor* DamageCauser)
{
	// 기본 데미지 처리 로직 호출 (필수는 아님)
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 체력을 데미지만큼 감소시키고, 0 이하로 떨어지지 않도록 Clamp
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	// 체력이 0 이하가 되면 사망 처리
	if (Health <= 0.0f)
	{
		OnDeath();
	}

	// 실제 적용된 데미지를 반환
	return ActualDamage;
}

// 사망 처리 함수
void AJupiterCharacter::OnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));

	// 사망 후 로직
}