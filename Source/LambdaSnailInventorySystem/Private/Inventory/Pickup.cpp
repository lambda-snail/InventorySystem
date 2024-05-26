#include "Inventory/Pickup.h"

#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemBase.h"
#include "Inventory/InventoryActor.h"
#include "Inventory/ItemUser.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetCanEverAffectNavigation(false); 

	Trigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickup::OnPickUp(AActor* Agent)
{
	if(Agent->Implements<UInventoryActor>())
	{
		OnPickup(TScriptInterface<IInventoryActor>(Agent));
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &APickup::OnSphereEndOverlap);

	if(HasAuthority())
	{
		Trigger->SetCollisionResponseToChannel(DefaultCollisionChannel, DefaultCollisionResponse);
		Trigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	if(not Item)
	{
		UClass const* ItemBaseClass = ItemClass.Get();
		UObject* CreatedItem = NewObject<UItemBase>(this, ItemBaseClass);
		Item = Cast<UItemBase>(CreatedItem);
	}

	//PhysicalRepresentation = Item->GetOrCreatePhysicalRepresentation(GetTransform());
	
	Item->CreateComponentForPickup(this);

	Super::BeginPlay();
}

void APickup::OnPickup(TScriptInterface<IInventoryActor> PickupCharacter)
{
	UInventoryComponent* Inventory = PickupCharacter->GetInventoryComponent();
	
	EItemAddResult Result = Inventory->TryAddItem(Item);
	if(Result == EItemAddResult::Success)
	{
		// TODO: Add to item pool?
		Item->OnPickup(Cast<AActor>(PickupCharacter.GetObject()));
		Item = nullptr; // Prevent destruction of item, TODO: Add to actor pool
		this->Destroy();
	}
	else
	{
		// TODO: Some kind of feedback when adding fails?
	}
}

void APickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bInteractiveTrigger)
	{
		if(OtherActor->Implements<UItemUser>())
		{
			TScriptInterface<IItemUser>(OtherActor)->EnterTriggerArea(this);
		}
		
		return;
	}
	
	if(OtherActor->Implements<UInventoryActor>())
	{
		OnPickup(TScriptInterface<IInventoryActor>(OtherActor));
		Trigger->OnComponentBeginOverlap.RemoveAll(this);
		this->Destroy();
	}
}

void APickup::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->Implements<UItemUser>() and bInteractiveTrigger)
	{
		TScriptInterface<IItemUser>(OtherActor)->LeaveTriggerArea();
	}
}
