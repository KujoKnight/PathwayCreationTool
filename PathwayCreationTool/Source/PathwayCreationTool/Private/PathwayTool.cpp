// Apache-2.0
// 
// Copyright (c) 2021, John Parmenter
// All rights reserved.
//
// This source code is licensed under the Apache-2.0-style license found in the
// LICENSE file in the root directory of this source tree.

#include "PathwayTool.h"

// Constructor for Path Creation Tool
APathwayTool::APathwayTool()
{
	// Disable Tick()
	PrimaryActorTick.bCanEverTick = true;

	// Set Root Component
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	if (Root)
	{
		SetRootComponent(Root);
	}

	// Add Spline Component
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	if (Spline)
	{
		Spline->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Add Instanced Mesh Component
	Mesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Mesh");
	if (Mesh)
	{
		Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Initialize Spline Point Vectors
	SplinePoints.Empty();
	SplinePoints.Add(FVector::ZeroVector);

	// Set Spline Point Curve Type
	CurveType = ESplinePointType::Linear;

	// Set Instance Mesh Scale and Offset
	MeshScale = FVector::OneVector;
	MeshOffset = FVector2D::ZeroVector;
}

// Override of OnConstruction for Editing in Editor
void APathwayTool::OnConstruction(const FTransform& Transform)
{
	// Call Parent OnConstruction Method
	Super::OnConstruction(Transform);

	// Clear Meshes
	if (Mesh->GetInstanceCount() > 0)
		Mesh->ClearInstances();

	// Refresh Spline Points
	Spline->SetClosedLoop(IsClosedLoop);
	Spline->ClearSplinePoints();
	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Local);
	for (auto i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		Spline->SetSplinePointType(i, CurveType);
	}

	// Mesh Instance Properties
	FTransform MeshTransform = FTransform::Identity;
	FVector MeshLocation = FVector::ZeroVector;
	FVector MeshSize = MeshScale * Mesh->GetStaticMesh()->GetBounds().BoxExtent * 2.0f;
	FVector Offset = FVector::ZeroVector;

	// Distance Check Value
	float dist = 0.0f;

	// Max Instances
	float max = 1.0f;
	if (Spline->GetSplineLength() != 0)
	{
		max = Spline->GetSplineLength() / MeshSize.Size();
	}

	// Add Mesh Instances along Spline
	for (auto i = 0; i < max; i++)
	{
		// Get Current Distance
		dist = i * MeshSize.Size();

		// Set Mesh Location
		MeshLocation = Spline->GetLocationAtDistanceAlongSpline(dist, ESplineCoordinateSpace::Local);

		// Get Offset Values
		Offset.X = FMath::Cos(i) * MeshOffset.X;
		Offset.Y = FMath::Sin(i) * MeshOffset.Y;

		// Create Mesh Instance
		MeshTransform.SetLocation(MeshLocation + Offset);
		MeshTransform.SetRotation(FQuat::Identity);
		MeshTransform.SetScale3D(MeshScale);
		Mesh->AddInstance(MeshTransform);
	}

	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());

	if (ShowDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow
		(
			GetWorld(),
			GetActorLocation() + FVector(0.0f, 0.0f, 200.0f),
			GetActorLocation() + FVector(0.0f, 0.0f, 100.0f),
			10.0f,
			FLinearColor::Red,
			5.0f,
			10.0f
		);

		UKismetSystemLibrary::DrawDebugArrow
		(
			GetWorld(),
			(GetActorLocation() + SplinePoints[SplinePoints.Num() - 1]) + FVector(0.0f, 0.0f, 200.0f),
			(GetActorLocation() + SplinePoints[SplinePoints.Num() - 1]) + FVector(0.0f, 0.0f, 100.0f),
			10.0f,
			FLinearColor::Red,
			5.0f,
			10.0f
		);
	}
}
