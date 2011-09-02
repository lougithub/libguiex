/** 
* @file guirotator.h
* @brief rotator
* @author ken
* @date 2011-09-01
*/

#ifndef __GUI_ROTATOR_H_20110901__
#define __GUI_ROTATOR_H_20110901__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"
#include "guiquaternion.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIRotator
	{
	public:
		CGUIRotator();
		CGUIRotator( real InPitch, real InYaw, real InRoll );
		explicit CGUIRotator( const class CGUIQuaternion& rQuat );

		// Binary arithmetic operators.
		CGUIRotator operator+( const CGUIRotator &R ) const
		{
			return CGUIRotator( Pitch+R.Pitch, Yaw+R.Yaw, Roll+R.Roll );
		}
		CGUIRotator operator-( const CGUIRotator &R ) const
		{
			return CGUIRotator( Pitch-R.Pitch, Yaw-R.Yaw, Roll-R.Roll );
		}
		CGUIRotator operator*( real Scale ) const
		{
			return CGUIRotator( Pitch*Scale, Yaw*Scale, Roll*Scale );
		}
		CGUIRotator operator*= (real Scale)
		{
			Pitch = Pitch*Scale; Yaw = Yaw*Scale; Roll = Roll*Scale;
			return *this;
		}
		// Binary comparison operators.
		bool operator==( const CGUIRotator &R ) const
		{
			return Pitch==R.Pitch && Yaw==R.Yaw && Roll==R.Roll;
		}
		bool operator!=( const CGUIRotator &V ) const
		{
			return Pitch!=V.Pitch || Yaw!=V.Yaw || Roll!=V.Roll;
		}
		// Assignment operators.
		CGUIRotator operator+=( const CGUIRotator &R )
		{
			Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
			return *this;
		}
		CGUIRotator operator-=( const CGUIRotator &R )
		{
			Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
			return *this;
		}
		int IsZero() const
		{
			return (
				GUI_REAL_EQUAL(Pitch, 0 ) &&
				GUI_REAL_EQUAL(Yaw, 0 ) &&
				GUI_REAL_EQUAL(Roll, 0 ));
		}

		CGUIVector3 Vector() const;
		CGUIQuaternion Quaternion() const;
		CGUIVector3 Euler() const;

		static CGUIRotator MakeFromEuler(const CGUIVector3& Euler);

		// Resets the rotation values so they fall within the range [0,65535]
		CGUIRotator Clamp() const
		{
			real outPitch = fmod( Pitch, 360.f);
			if( outPitch < 0.f )
			{
				outPitch += 360.f;
			}
			real outYaw = fmod( Yaw, 360.f);
			if( outYaw < 0.f )
			{
				outYaw += 360.f;
			}
			real outRoll = fmod( Roll, 360.f);
			if( outRoll < 0.f )
			{
				outRoll += 360.f;
			}
			return CGUIRotator( outPitch, outYaw, outRoll);
		}

		// within the range [-180, 180]
		static real NormalizeAxis(real Angle)
		{
			Angle = fmod( Angle, 180.f);
			return Angle;
		}

		CGUIRotator GetNormalized() const
		{
			CGUIRotator Rot = *this;
			Rot.Pitch = NormalizeAxis(Rot.Pitch);
			Rot.Roll = NormalizeAxis(Rot.Roll);
			Rot.Yaw = NormalizeAxis(Rot.Yaw);
			return Rot;
		}

		static CGUIRotator ZeroRotator;

	public:
		// Variables.
		real Pitch; // Looking up and down, in degree
		real Yaw;   // Rotating around (running in circles), 0=East, +North, -South. in degree
		real Roll;  // Rotation about axis of screen, 0=Straight, +Clockwise, -CCW. in degree
	};


}//namespace guiex
#endif //__GUI_ROTATOR_H_20110901__

