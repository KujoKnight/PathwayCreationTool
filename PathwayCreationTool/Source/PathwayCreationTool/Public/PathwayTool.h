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
#include "Kismet/KismetSystemLibrary.h"
#include "PathwayTool.generated.h"

UCLASS()
class PATHWAYCREATIONTOOL_API APathwayTool : public AActor
{
	GENERATED_BODY()
	
public:
	// Tool Constructor
	APathwayTool();

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

	// Spline Point Vectors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		TArray<FVector> SplinePoints;

	// Instanced Mesh Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		FVector MeshScale;

	// Instanced Mesh Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		FVector2D MeshOffset;

	// Spline Points Curve Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		TEnumAsByte<ESplinePointType::Type> CurveType;

	// Spline Closed Loop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		bool IsClosedLoop = false;

	// Show Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
		bool ShowDebug = false;

};
