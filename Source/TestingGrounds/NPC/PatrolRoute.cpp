// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolRoute.h"
#include "Engine.h"
#include "Components/ActorComponent.h"

TArray<AActor*> UPatrolRoute::GetPatrolPoints() const
{
	return PatrolPoints;
}

void UPatrolRoute::FindNearbyWaypoints(AActor* ControlledPawn)
{
	TArray<AActor*> tempArray;
	float CheckRadius = 2000.f;
	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), tempArray);
	
	//Remove all target points that are outside the CheckRadius
	for (AActor* Actor : tempArray)
	{
		FVector ActorLoc = Actor->GetActorLocation();
		FVector ControllerLoc = ControlledPawn->GetActorLocation();
		float distanceBetweenPoints = FVector::Dist(ActorLoc, ControllerLoc);

		if (distanceBetweenPoints > CheckRadius)
		{
			tempArray.RemoveSwap(Actor);
		}
	}

	//Randomly select qualifying waypoints and add them to the PatrolPoints array
	while (tempArray.Num() > 0)
	{
		//pick random index
		int32 index = FMath::RandRange(0, tempArray.Num() - 1);

		//copy the value from the temparray into the last position in PatrolPoints array
		PatrolPoints.AddUnique(tempArray[index]);

		//remove the value at the index in tempArray
		tempArray.RemoveAtSwap(index);
	}
}
