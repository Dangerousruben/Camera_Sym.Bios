// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraBase.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "UnrealClient.h"
#include "WidgetLayoutLibrary.h"

//Called at the start of the game
void UCameraBase::BeginPlay()
{
	//Adds function SetViewportSize to delegate ViewportResizedEvent so SetViewportSize gets called when the viewport resizes
	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &UCameraBase::SetViewportSize);
	SetViewportSize(NULL, 0);
	Super::BeginPlay();
}

//Sets the viewport size and scales it accordingly
//Param[in] a_Viewport* : Reference to the viewport, it is necessary for the delegate ViewportResizedEvent, is unnamed because I am not using it
//Param[in] unint32 : An index that is always zero as of 4.10, I don't know what it is used for, it is necessary for the delegate ViewportResizedEvent, is unnamed because I am not using it
inline void UCameraBase::SetViewportSize(FViewport*, uint32)
{
//In the editor the viewport can be invalid
#if WITH_EDITOR

	if (!GEngine->GameViewport)
	{
		return;
	}

#endif
	m_ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport);

	//Doing these calculations now saves 2 calculations per tick
	m_ViewportSize.X -= EdgePanMargin;
	m_ViewportSize.Y -= EdgePanMargin;
}

//Returns the cameras pan direction in accordance of the mouse position
FVector UCameraBase::GetCameraEdgePanDirection()
{

//No edge pan necessary on PS4
#if !PLATFORM_PS4

	//Only necessary in the editor
	#if WITH_EDITOR

		//Looking at IsSimulateInEditorViewport() you would expect that if the viewport is being simulated it will return true
		//However the opposite is true so if the viewport is being simulated it will return false
		if (GEngine->GameViewport->IsSimulateInEditorViewport())
		{
			return FVector::ZeroVector;
		}

		//Checks if mouse position is negative, if so then return zero
		//This way edge panning is disabled if you press shift F1
		//But not when the UI is clicked
		if (!GEngine->GameViewport->Viewport->HasFocus())
		{
			FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			if (MousePosition.X < -EdgePanMargin)
			{
				return FVector::ZeroVector;
			}
			else if (MousePosition.X > m_ViewportSize.X + EdgePanMargin)
			{
				return FVector::ZeroVector;
			}
			if (MousePosition.Y < -EdgePanMargin)
			{
				return FVector::ZeroVector;
			}

			else if (MousePosition.Y > m_ViewportSize.Y + EdgePanMargin)
			{
				return FVector::ZeroVector;
			}
		}
	#endif

	//Disables edge pan if alt tabbed
	if (!GEngine->GameViewport->Viewport->IsForegroundWindow())
	{
		return FVector::ZeroVector;
	}

	float CameraDirectionX = 0;
	float CameraDirectionY = 0;

	//Gets the mouseposition on the world viewport
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	if (MousePosition.X <= EdgePanMargin)
	{
		CameraDirectionY = -EdgePanSpeed;
	}

	else if (MousePosition.X >= m_ViewportSize.X)
	{
		CameraDirectionY = EdgePanSpeed;
	}

	if (MousePosition.Y <= EdgePanMargin)
	{
		CameraDirectionX = EdgePanSpeed;
	}

	else if (MousePosition.Y >= m_ViewportSize.Y)
	{
		CameraDirectionX = -EdgePanSpeed;
	}

	return FVector(CameraDirectionX, CameraDirectionY, 0);

#else

	return FVector::ZeroVector;

#endif
}
