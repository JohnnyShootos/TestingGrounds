// Fill out your copyright notice in the Description page of Project Settings.

#include "../NPC/ChooseNextWaypoint.h"
#include "TestingGrounds.h"
#include "AIController.h"
#include "../NPC/PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against empty patrol routes
	
	///Get the patrol points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; };
	
	PatrolRoute->FindNearbyWaypoints(ControlledPawn);
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A Guard is missing patrol points"));
		return EBTNodeResult::Failed;
	}

	/// Set the next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	///Cycle through the waypoints
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	
	/// UE_LOG(LogTemp, Warning, TEXT("Waypoint Index: %i"), Index);
	return EBTNodeResult::Succeeded;
}


