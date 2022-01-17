// Apache-2.0
// 
// Copyright (c) 2021, John Parmenter
// All rights reserved.
//
// This source code is licensed under the Apache-2.0-style license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PathwayCreationTool.generated.h"

UCLASS()
class SUPERHAMSTERHAVOC_API APathwayCreationTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Tool Constructor
	APathwayCreationTool();

protected:
	// Override OnConstruction
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Root Component
	UPROPERTY(VisibleAnywhere, Category = "Path")
		USceneComponent* Root;

	// Spline Component
	UPROPERTY(VisibleAnywhere, Category = "Path")
		USplineComponent* Spline;

	// Instanced Mesh Component
	UPROPERTY(VisibleAnywhere, Category = "Path")
		UInstancedStaticMeshComponent* Mesh;

	// Path Start Mesh
	UPROPERTY(VisibleAnywhere, Category = "Path")
		UStaticMeshComponent* StartMesh;

	// Path End Mesh
	UPROPERTY(VisibleAnywhere, Category = "Path")
		UStaticMeshComponent* EndMesh;

	// Spline Point Vectors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Spline")
		TArray<FVector> SplinePoints;

	// Instanced Mesh Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		FVector MeshScale;

	// Instanced Mesh Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		FVector2D MeshOffset;

	// Spline Points Curve Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Spline")
		TEnumAsByte<ESplinePointType::Type> CurveType;

	// Instanced Mesh Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh", meta = (ClampMin = "0.01", ClampMax = "3.0"))
		float MeshSpacing = 2.0f;

	// Spline Closed Loop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Spline")
		bool IsClosedLoop = false;

	// Enable Start Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool EnableStartMesh = false;

	// Enable End Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool EnableEndMesh = false;

	// Enable Start Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool SetInstanceLookAt = false;

	// Use Random Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool EnableRandomRotation = false;

	// Use Random Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool EnableRandomScale = false;
	
	// Use Mass Spread
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Mesh")
		bool UseMassSpread = false;

	// Show Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path | Debug")
		bool ShowDebug = false;
};
