/** 
* @file guiplane.cpp
* @brief plane
* @author ken
* @date 2007-11-23
*/




//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiplane.h>
#include <libguiex_core\guivector3.h>


namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIPlane::CGUIPlane ()
	{
		normal = CGUIVector3::ZERO;
		d = 0.0;
	}
	//-----------------------------------------------------------------------
	CGUIPlane::CGUIPlane (const CGUIPlane& rhs)
	{
		normal = rhs.normal;
		d = rhs.d;
	}
	//-----------------------------------------------------------------------
	CGUIPlane::CGUIPlane (const CGUIVector3& rkNormal, real fConstant)
	{
		normal = rkNormal;
		d = -fConstant;
	}
	//-----------------------------------------------------------------------
	CGUIPlane::CGUIPlane (const CGUIVector3& rkNormal, const CGUIVector3& rkPoint)
	{
		normal = rkNormal;
		d = -rkNormal.DotProduct(rkPoint);
	}
	//-----------------------------------------------------------------------
	CGUIPlane::CGUIPlane (const CGUIVector3& rkPoint0, const CGUIVector3& rkPoint1,
		const CGUIVector3& rkPoint2)
	{
		redefine(rkPoint0, rkPoint1, rkPoint2);
	}
	//-----------------------------------------------------------------------
	real CGUIPlane::getDistance (const CGUIVector3& rkPoint) const
	{
		return normal.DotProduct(rkPoint) + d;
	}
	//-----------------------------------------------------------------------
	CGUIPlane::Side CGUIPlane::getSide (const CGUIVector3& rkPoint) const
	{
		real fDistance = getDistance(rkPoint);

		if ( fDistance < 0.0 )
			return CGUIPlane::NEGATIVE_SIDE;

		if ( fDistance > 0.0 )
			return CGUIPlane::POSITIVE_SIDE;

		return CGUIPlane::NO_SIDE;
	}
	//-----------------------------------------------------------------------
	void CGUIPlane::redefine(const CGUIVector3& rkPoint0, const CGUIVector3& rkPoint1,
		const CGUIVector3& rkPoint2)
	{
		CGUIVector3 kEdge1 = rkPoint1 - rkPoint0;
		CGUIVector3 kEdge2 = rkPoint2 - rkPoint0;
		normal = kEdge1.CrossProduct(kEdge2);
		normal.Normalise();
		d = -normal.DotProduct(rkPoint0);
	}
	//-----------------------------------------------------------------------
} // namespace guiex
