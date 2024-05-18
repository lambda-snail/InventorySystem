// Fill out your copyright notice in the Description page of Project Settings.


#include "InertItem.h"

#include "Engine/StaticMeshActor.h"

void UInertItem::OnPickup_Implementation(AActor* Owner)
{
	// if(not PhysicalRepresentation)
	// {
	// 	return;
	// }
	//
	// DestroyPhysicalRepresentation();
}

void UInertItem::OnDrop_Implementation(AActor* Owner)
{
	FActorSpawnParameters p;
	FTransform Transform = Owner->GetTransform();
	Transform.SetLocation(Transform.GetLocation() + Transform.GetRotation().GetForwardVector() * 100.0f);

	// TODO: Spawn pickup
	AStaticMeshActor const* MeshActor = GetWorld()->SpawnActorAbsolute<AStaticMeshActor>(Transform.GetLocation(), Transform.Rotator());
	MeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	//AStaticMeshActor const* Mesh = GetOrCreatePhysicalRepresentation<AStaticMeshActor>(Transform); 
	if(UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent())
	{
		MeshComponent->SetMobility(EComponentMobility::Movable);
		MeshComponent->SetSimulatePhysics(true);
		//MeshComponent->SetStaticMesh(ItemMetadata->PhysicalType);
	}
}

void UInertItem::CreateComponentForPickup(AActor* Actor) const
{
	UActorComponent* ActorComponent = Actor->AddComponentByClass(PhysicalRepresentationComponent, false, {}, true);

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
	MeshComponent->SetupAttachment(Actor->GetRootComponent());
	MeshComponent->SetStaticMesh(Mesh);

	MeshComponent->RegisterComponent();
}
