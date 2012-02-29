/** 
* @file guimath.cpp
* @brief math utility
* @author ken
* @date 2007-11-23
*/

//============================================================================//
// include
//============================================================================// 
#include "guimath.h"
#include "guimatrix4.h"
#include "guivector3.h"
#include "guivector4.h"

namespace guiex
{

	const real CGUIMath::GUI_POS_INFINITY = std::numeric_limits<real>::infinity();
	const real CGUIMath::GUI_NEG_INFINITY = -std::numeric_limits<real>::infinity();
	const real CGUIMath::GUI_PI = real( 4.0f * atan( 1.0f ) );
	const real CGUIMath::GUI_TWO_PI = real( 2.0f * CGUIMath::GUI_PI );
	const real CGUIMath::GUI_HALF_PI = real( 0.5f * CGUIMath::GUI_PI );
	const real CGUIMath::GUI_Deg2Rad = CGUIMath::GUI_PI / real(180.0f);
	const real CGUIMath::GUI_Rad2Deg = real(180.0f) / CGUIMath::GUI_PI;

	//-----------------------------------------------------------------------
	real CGUIMath::DegreesToRadians(real degrees) 
	{ 
		return degrees * GUI_Deg2Rad;
	}
	//-----------------------------------------------------------------------
	real CGUIMath::RadiansToDegrees(real radians)
	{
		return radians * GUI_Rad2Deg; 
	}
	//-----------------------------------------------------------------------
	real CGUIMath::UnitRandom ()
	{
		return real( rand() ) / real( RAND_MAX );
	}

	//-----------------------------------------------------------------------
	real CGUIMath::RangeRandom (real fLow, real fHigh)
	{
		return (fHigh-fLow)*UnitRandom() + fLow;
	}

	//-----------------------------------------------------------------------
	real CGUIMath::SymmetricRandom ()
	{
		return 2.0f * UnitRandom() - 1.0f;
	}
	//-----------------------------------------------------------------------
	bool CGUIMath::pointInTri2D( real px, real py, real ax, real ay, real bx, real by, real cx, real cy )
	{
		real v1x, v2x, v1y, v2y;
		bool bClockwise;

		v1x = bx - ax;
		v1y = by - ay;

		v2x = px - bx;
		v2y = py - by;

		// For the sake of readability
#define Clockwise ( v1x * v2y - v1y * v2x >= 0.0 )

		bClockwise = Clockwise;

		v1x = cx - bx;
		v1y = cy - by;

		v2x = px - cx;
		v2y = py - cy;

		if( Clockwise != bClockwise )
			return false;

		v1x = ax - cx;
		v1y = ay - cy;

		v2x = px - ax;
		v2y = py - ay;

		if( Clockwise != bClockwise )
			return false;

		return true;

		// Clean up the #defines
#undef Clockwise
	}

	//-----------------------------------------------------------------------
	bool CGUIMath::RealEqual( real a, real b, real tolerance )
	{
		if (fabs(b-a) <= tolerance)
			return true;
		else
			return false;
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIMath::calculateTangentSpaceVector(
		const CGUIVector3& position1, const CGUIVector3& position2, const CGUIVector3& position3,
		real u1, real v1, real u2, real v2, real u3, real v3)
	{
		//side0 is the vector along one side of the triangle of vertices passed in, 
		//and side1 is the vector along another side. Taking the cross product of these returns the normal.
		CGUIVector3 side0 = position1 - position2;
		CGUIVector3 side1 = position3 - position1;
		//Calculate face normal
		CGUIVector3 normal = side1.CrossProduct(side0);
		normal.Normalise();
		//Now we use a formula to calculate the tangent. 
		real deltaV0 = v1 - v2;
		real deltaV1 = v3 - v1;
		CGUIVector3 tangent = deltaV1 * side0 - deltaV0 * side1;
		tangent.Normalise();
		//Calculate binormal
		real deltaU0 = u1 - u2;
		real deltaU1 = u3 - u1;
		CGUIVector3 binormal = deltaU1 * side0 - deltaU0 * side1;
		binormal.Normalise();
		//Now, we take the cross product of the tangents to get a vector which 
		//should point in the same direction as our normal calculated above. 
		//If it points in the opposite direction (the dot product between the normals is less than zero), 
		//then we need to reverse the s and t tangents. 
		//This is because the triangle has been mirrored when going from tangent space to object space.
		//reverse tangents if necessary
		CGUIVector3 tangentCross = tangent.CrossProduct(binormal);
		if (tangentCross.DotProduct(normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		return tangent;

	}
	//-----------------------------------------------------------------------
	CGUIMatrix4 CGUIMath::buildReflectionMatrix(const CGUIPlane& p)
	{
		return CGUIMatrix4(
			-2 * p.normal.x * p.normal.x + 1,   -2 * p.normal.x * p.normal.y,       -2 * p.normal.x * p.normal.z,       -2 * p.normal.x * p.d, 
			-2 * p.normal.y * p.normal.x,       -2 * p.normal.y * p.normal.y + 1,   -2 * p.normal.y * p.normal.z,       -2 * p.normal.y * p.d, 
			-2 * p.normal.z * p.normal.x,       -2 * p.normal.z * p.normal.y,       -2 * p.normal.z * p.normal.z + 1,   -2 * p.normal.z * p.d, 
			0,                                  0,                                  0,                                  1);
	}
	//-----------------------------------------------------------------------
	CGUIVector4 CGUIMath::calculateFaceNormal(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3)
	{
		CGUIVector3 normal = calculateBasicFaceNormal(v1, v2, v3);
		// Now set up the w (distance of tri from origin
		return CGUIVector4(normal.x, normal.y, normal.z, -(normal.DotProduct(v1)));
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIMath::calculateBasicFaceNormal(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3)
	{
		CGUIVector3 normal = (v2 - v1).CrossProduct(v3 - v1);
		normal.Normalise();
		return normal;
	}
	//-----------------------------------------------------------------------
	CGUIVector4 CGUIMath::calculateFaceNormalWithoutNormalize(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3)
	{
		CGUIVector3 normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
		// Now set up the w (distance of tri from origin)
		return CGUIVector4(normal.x, normal.y, normal.z, -(normal.DotProduct(v1)));
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIMath::calculateBasicFaceNormalWithoutNormalize(const CGUIVector3& v1, const CGUIVector3& v2, const CGUIVector3& v3)
	{
		CGUIVector3 normal = (v2 - v1).CrossProduct(v3 - v1);
		return normal;
	}
	//------------------------------------------------------------------------------
	uint16 CGUIMath::GetNextPOT( uint16 value )
	{
		int r=1;
		while( value > r )
		{
			r<<=1;
		}
		return r;
	}
}
