#include "Inventory/Network/ReplicatedObject.h"

int32 UReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));

	AActor* Owner = GetOwner();
	if (UNetDriver* NetDriver = Owner->GetNetDriver())
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		return true;
	}
	
	return false;
}

UWorld* UReplicatedObject::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

AActor* UReplicatedObject::GetOwner() const
{
	return GetTypedOuter<AActor>();
}

void UReplicatedObject::DestroyObject()
{
	if(this->IsGarbageEliminationEnabled() and GetOwner() and GetOwner()->HasAuthority())
	{
		MarkAsGarbage();
	}
}
