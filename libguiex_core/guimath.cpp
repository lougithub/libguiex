/** 
* @file guimath.cpp
* @brief math utility
* @author ken
* @date 2007-11-23
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guimath.h>
#include <libguiex_core/guimatrix4.h>
#include <libguiex_core/guivector3.h>
#include <libguiex_core/guivector4.h>

namespace guiex
{

	const real CGUIMath::GUI_POS_INFINITY = std::numeric_limits<real>::infinity();
	const real CGUIMath::GUI_NEG_INFINITY = -std::numeric_limits<real>::infinity();
	const real CGUIMath::GUI_PI = real( 4.0f * atan( 1.0f ) );
	const real CGUIMath::GUI_TWO_PI = real( 2.0f * CGUIMath::GUI_PI );
	const real CGUIMath::GUI_HALF_PI = real( 0.5f * CGUIMath::GUI_PI );
	const real CGUIMath::GUI_Deg2Rad = CGUIMath::GUI_PI / real(180.0f);
	const real CGUIMath::GUI_Rad2Deg = real(180.0f) / CGUIMath::GUI_PI;

	int CGUIMath::mTrigTableSize;
	CGUIMath::EAngleUnit CGUIMath::msAngleUnit;

	real  CGUIMath::mTrigTableFactor;
	real *CGUIMath::mSinTable = NULL;
	real *CGUIMath::mTanTable = NULL;

	//-----------------------------------------------------------------------
	CGUIMath::CGUIMath( unsigned int trigTableSize )
	{
		msAngleUnit = AU_DEGREE;

		mTrigTableSize = trigTableSize;
		mTrigTableFactor = mTrigTableSize / CGUIMath::GUI_TWO_PI;

		mSinTable = new real[mTrigTableSize];
		mTanTable = new real[mTrigTableSize];

		buildTrigTables();
	}

	//-----------------------------------------------------------------------
	CGUIMath::~CGUIMath()
	{
		delete [] mSinTable;
		delete [] mTanTable;
	}

	//-----------------------------------------------------------------------
	void CGUIMath::buildTrigTables(void)
	{
		// Build trig lookup tables
		// Could get away with building only GUI_PI sized Sin table but simpler this 
		// way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
		// simplicity.
		real angle;
		for (int i = 0; i < mTrigTableSize; ++i)
		{
			angle = CGUIMath::GUI_TWO_PI * i / mTrigTableSize;
			mSinTable[i] = sin(angle);
			mTanTable[i] = tan(angle);
		}
	}
	//-----------------------------------------------------------------------	
	real CGUIMath::SinTable (real fValue)
	{
		// Convert range to index values, wrap if required
		int idx;
		if (fValue >= 0)
		{
			idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
		}
		else
		{
			idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
		}

		return mSinTable[idx];
	}
	//-----------------------------------------------------------------------
	real CGUIMath::TanTable (real fValue)
	{
		// Convert range to index values, wrap if required
		int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
		return mTanTable[idx];
	}
	//-----------------------------------------------------------------------
	int CGUIMath::ISign (int iValue)
	{
		return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
	}
	//-----------------------------------------------------------------------
	CGUIRadian CGUIMath::ACos (real fValue)
	{
		if ( -1.0 < fValue )
		{
			if ( fValue < 1.0 )
				return CGUIRadian(acos(fValue));
			else
				return CGUIRadian(0.0);
		}
		else
		{
			return CGUIRadian(GUI_PI);
		}
	}
	//-----------------------------------------------------------------------
	CGUIRadian CGUIMath::ASin (real fValue)
	{
		if ( -1.0 < fValue )
		{
			if ( fValue < 1.0 )
				return CGUIRadian(asin(fValue));
			else
				return CGUIRadian(-GUI_HALF_PI);
		}
		else
		{
			return CGUIRadian(GUI_HALF_PI);
		}
	}
	//-----------------------------------------------------------------------
	real CGUIMath::Sign (real fValue)
	{
		if ( fValue > 0.0 )
			return 1.0;

		if ( fValue < 0.0 )
			return -1.0;

		return 0.0;
	}
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
	real CGUIMath::Cos (const CGUIRadian& fValue) 
	{
		return real(cos(fValue.valueRadians()));
	}
	//-----------------------------------------------------------------------
	real CGUIMath::Cos (real fValue)
	{
		return real(cos(fValue));
	}
	//-----------------------------------------------------------------------
	real CGUIMath::InvSqrt(real fValue)
	{
		return real(1 / sqrt( fValue ));
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
	void CGUIMath::setAngleUnit(CGUIMath::EAngleUnit unit)
	{
		msAngleUnit = unit;
	}
	//-----------------------------------------------------------------------
	CGUIMath::EAngleUnit CGUIMath::getAngleUnit(void)
	{
		return msAngleUnit;
	}
	//-----------------------------------------------------------------------
	real CGUIMath::AngleUnitsToRadians(real angleunits)
	{
		if (msAngleUnit == AU_DEGREE)
			return angleunits * GUI_Deg2Rad;
		else
			return angleunits;
	}

	//-----------------------------------------------------------------------
	real CGUIMath::RadiansToAngleUnits(real radians)
	{
		if (msAngleUnit == AU_DEGREE)
			return radians * GUI_Rad2Deg;
		else
			return radians;
	}

	//-----------------------------------------------------------------------
	real CGUIMath::AngleUnitsToDegrees(real angleunits)
	{
		if (msAngleUnit == AU_RADIAN)
			return angleunits * GUI_Rad2Deg;
		else
			return angleunits;
	}

	//-----------------------------------------------------------------------
	real CGUIMath::DegreesToAngleUnits(real degrees)
	{
		if (msAngleUnit == AU_RADIAN)
			return degrees * GUI_Deg2Rad;
		else
			return degrees;
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
}
