// Duskfall - Michael Rose - r015676g


#include "BaseWeapon.h"
#include "PaperFlipbookComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::AttackingCharging()
{
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::AttackPressed()
{
}

void ABaseWeapon::AttackReleased()
{
}

