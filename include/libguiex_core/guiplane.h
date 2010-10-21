/** 
* @file guiplane.h
* @brief plane
* @author ken
* @date 2007-11-23
*/

#ifndef __GUI_PLANE_20071123_H__
#define __GUI_PLANE_20071123_H__




//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiplane.h"
#include "guivector3.h"

#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex 
{
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
	/** Defines a plane in 3D space.
	@remarks
	A plane is defined in 3D space by the equation
	Ax + By + Cz + D = 0
	@par
	This equates to a vector (the normal of the plane, whose x, y
	and z components equate to the coefficients A, B and C
	respectively), and a constant (D) which is the distance along
	the normal you have to go to move the plane back to the origin.
	*/
	class GUIEXPORT CGUIPlane
	{
	public:
		/** Default constructor - sets everything to 0.
		*/
		CGUIPlane ();
		CGUIPlane (const CGUIPlane& rhs);
		/** Construct a plane through a normal, and a distance to move the plane along the normal.*/
		CGUIPlane (const CGUIVector3& rkNormal, real fConstant);
		CGUIPlane (const CGUIVector3& rkNormal, const CGUIVector3& rkPoint);
		CGUIPlane (const CGUIVector3& rkPoint0, const CGUIVector3& rkPoint1,
			const CGUIVector3& rkPoint2);

		/** The "positive side" of the plane is the half space to which the
		plane normal points. The "negative side" is the other half
		space. The flag "no side" indicates the plane itself.
		*/
		enum Side
		{
			NO_SIDE,
			POSITIVE_SIDE,
			NEGATIVE_SIDE
		};

		Side getSide (const CGUIVector3& rkPoint) const;

		/** This is a pseudodistance. The sign of the return value is
		positive if the point is on the positive side of the plane,
		negative if the point is on the negative side, and zero if the
		point is on the plane.
		@par
		The absolute value of the return value is the true distance only
		when the plane normal is a unit length vector.
		*/
		real getDistance (const CGUIVector3& rkPoint) const;

		/** Redefine this plane based on 3 points. */
		void redefine(const CGUIVector3& rkPoint0, const CGUIVector3& rkPoint1,
			const CGUIVector3& rkPoint2);

		CGUIVector3 normal;
		real d;
		/// Comparison operator
		bool operator==(const CGUIPlane& rhs) const
		{
			return (rhs.d == d && rhs.normal == normal);
		}
	};

	typedef std::vector<CGUIPlane> PlaneList;

} // namespace guiex

#endif
