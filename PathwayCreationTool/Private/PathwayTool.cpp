// Apache-2.0
// 
// Copyright (c) 2021, John Parmenter
// All rights reserved.
//
// This source code is licensed under the Apache-2.0-style license found in the
// LICENSE file in the root directory of this source tree.

#include "PathwayTool.h"

// Constructor for Path Creation Tool
APathwayCreationTool::APathwayCreationTool()
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

	// Add Start Mesh Component
	StartMesh = CreateDefaultSubobject<UStaticMeshComponent>("StartMesh");
	if (StartMesh)
	{
		StartMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Add End Mesh Component
	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>("EndMesh");
	if (EndMesh)
	{
		EndMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
void APathwayCreationTool::OnConstruction(const FTransform& Transform)
{
	// Call Parent OnConstruction Method
	Super::OnConstruction(Transform);

	// Clear Meshes
	if (Mesh->GetInstanceCount() > 0)
		Mesh->ClearInstances();

	// Refresh Spline Points
	Spline->ClearSplinePoints();
	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Local);
	for (auto i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		Spline->SetSplinePointType(i, CurveType);
	}
	Spline->SetClosedLoop(IsClosedLoop);

	// Mesh Instance Properties
	FTransform MeshTransform = FTransform::Identity;
	FVector MeshLocation = FVector::ZeroVector;
	FQuat MeshRotation = FQuat::Identity;
	FVector MeshSize = FVector::OneVector;
	if (Mesh->GetStaticMesh())
	{
		MeshSize = MeshScale * (Mesh->GetStaticMesh()->GetBounds().BoxExtent * MeshSpacing);
	}
	else 
	{
		MeshSize = MeshScale * FVector::OneVector;
	}
	FVector Offset = FVector::ZeroVector;

	// Distance Check Value
	float dist = 0.0f;

	// Enable Start Mesh
	StartMesh->SetVisibility(EnableStartMesh);
	StartMesh->SetHiddenInGame(EnableStartMesh);
	StartMesh->SetRelativeLocation(FVector::ZeroVector);

	if (!UseMassSpread)
	{
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
			FTransform prevTransform = FTransform::Identity;
			MeshTransform.SetLocation(MeshLocation + Offset);

			if (EnableRandomRotation)
			{
				FRotator randRot = FRotator::ZeroRotator;
				randRot = FRotator(0.0f, FMath::RandRange(0, 360), 0.0f);
				MeshRotation = randRot.Quaternion();
			}
			else
			{
				if (SetInstanceLookAt)
				{
					if (i == 0)
					{
						FRotator lookAt = FRotator::ZeroRotator;
						float nextDist = i + 1 * MeshSize.Size();
						FVector nextPoint = Spline->GetLocationAtDistanceAlongSpline(nextDist, ESplineCoordinateSpace::Local);
						lookAt = UKismetMathLibrary::FindLookAtRotation(MeshTransform.GetLocation(), nextPoint);
						MeshRotation = lookAt.Quaternion();
					}
					else
					{
						FRotator lookAt = FRotator::ZeroRotator;
						lookAt = UKismetMathLibrary::FindLookAtRotation(MeshTransform.GetLocation(), prevTransform.GetLocation());
						MeshRotation = lookAt.Quaternion();
					}
				}
				else
				{
					MeshRotation = FQuat::Identity;
				}
			}
			MeshTransform.SetRotation(MeshRotation);

			if (EnableRandomScale)
			{
				FVector randScale = FVector::OneVector;
				randScale = FVector(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
				MeshTransform.SetScale3D(randScale);
			}
			else
			{
				MeshTransform.SetScale3D(MeshScale);
			}

			Mesh->AddInstance(MeshTransform);

			prevTransform = MeshTransform;
		}
	}
	else
	{
		// Add Mesh Instances along Spline
		for (auto i = 0; i < Spline->GetSplineLength(); i += MeshSize.Size())
		{
			// Set Mesh Location
			MeshLocation = Spline->GetLocationAtDistanceAlongSpline(i, ESplineCoordinateSpace::Local);

			// Get Offset Values
			Offset.X = FMath::Cos(i) * MeshOffset.X;
			Offset.Y = FMath::Sin(i) * MeshOffset.Y;

			// Create Mesh Instance
			FTransform prevTransform = FTransform::Identity;
			MeshTransform.SetLocation(MeshLocation + Offset);

			if (EnableRandomRotation)
			{
				FRotator randRot = FRotator::ZeroRotator;
				randRot = FRotator(0.0f, FMath::RandRange(0, 360), 0.0f);
				MeshRotation = randRot.Quaternion();
			}
			else
			{
				if (SetInstanceLookAt)
				{
					if (i == 0)
					{
						FRotator lookAt = FRotator::ZeroRotator;
						float nextDist = i + 1 * MeshSize.Size();
						FVector nextPoint = Spline->GetLocationAtDistanceAlongSpline(nextDist, ESplineCoordinateSpace::Local);
						lookAt = UKismetMathLibrary::FindLookAtRotation(MeshTransform.GetLocation(), nextPoint);
						MeshRotation = lookAt.Quaternion();
					}
					else
					{
						FRotator lookAt = FRotator::ZeroRotator;
						lookAt = UKismetMathLibrary::FindLookAtRotation(MeshTransform.GetLocation(), prevTransform.GetLocation());
						MeshRotation = lookAt.Quaternion();
					}
				}
				else
				{
					MeshRotation = FQuat::Identity;
				}
			}
			MeshTransform.SetRotation(MeshRotation);

			if (EnableRandomScale)
			{
				FVector randScale = FVector::OneVector;
				randScale = FVector(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
				MeshTransform.SetScale3D(randScale);
			}
			else
			{
				MeshTransform.SetScale3D(MeshScale);
			}

			Mesh->AddInstance(MeshTransform);

			prevTransform = MeshTransform;
		}
	}

	// Enable End Mesh
	EndMesh->SetVisibility(EnableEndMesh);
	EndMesh->SetHiddenInGame(EnableEndMesh);
	EndMesh->SetRelativeLocation(Spline->GetLocationAtDistanceAlongSpline(dist, ESplineCoordinateSpace::Local));

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
