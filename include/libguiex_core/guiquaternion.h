/** 
* @file guiquaternion.h
* @brief quaternion
* @author ken
* @date 2007-11-23
*/


#ifndef __GUI_QUATERNION_20071123_H__
#define __GUI_QUATERNION_20071123_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guimath.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex 
{
	class CGUIMatrix3;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{

	/** Implementation of a CGUIQuaternion, i.e. a rotation around an axis.
	*/
	class GUIEXPORT CGUIQuaternion
	{
	public:
		inline CGUIQuaternion (
			real fW = 1.0,
			real fX = 0.0, real fY = 0.0, real fZ = 0.0)
		{
			w = fW;
			x = fX;
			y = fY;
			z = fZ;
		}
		inline CGUIQuaternion (const CGUIQuaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
		}
		/// Construct a quaternion from a rotation matrix
		inline CGUIQuaternion(const CGUIMatrix3& rot)
		{
			this->FromRotationMatrix(rot);
		}
		/// Construct a quaternion from an angle/axis
		inline CGUIQuaternion(const CGUIRadian& rfAngle, const CGUIVector3& rkAxis)
		{
			this->FromAngleAxis(rfAngle, rkAxis);
		}

		inline CGUIQuaternion(const real& rfAngle, const CGUIVector3& rkAxis)
		{
			this->FromAngleAxis(rfAngle, rkAxis);
		}

		/// Construct a quaternion from 3 orthonormal local axes
		inline CGUIQuaternion(const CGUIVector3& xaxis, const CGUIVector3& yaxis, const CGUIVector3& zaxis)
		{
			this->FromAxes(xaxis, yaxis, zaxis);
		}
		/// Construct a quaternion from 3 orthonormal local axes
		inline CGUIQuaternion(const CGUIVector3* akAxis)
		{
			this->FromAxes(akAxis);
		}

		void FromRotationMatrix (const CGUIMatrix3& kRot);
		void ToRotationMatrix (CGUIMatrix3& kRot) const;
		void FromAngleAxis (const CGUIRadian& rfAngle, const CGUIVector3& rkAxis);
		void ToAngleAxis (CGUIRadian& rfAngle, CGUIVector3& rkAxis) const;
		inline void ToAngleAxis (CGUIDegree& dAngle, CGUIVector3& rkAxis) const 
		{
			CGUIRadian rAngle;
			ToAngleAxis ( rAngle, rkAxis );
			dAngle = rAngle;
		}

		inline void FromAngleAxis (const real& rfAngle, const CGUIVector3& rkAxis) {
			FromAngleAxis ( CGUIAngle(rfAngle), rkAxis );
		}
		inline void ToAngleAxis (real& rfAngle, CGUIVector3& rkAxis) const {
			CGUIRadian r;
			ToAngleAxis ( r, rkAxis );
			rfAngle = r.valueAngleUnits();
		}

		void FromAxes (const CGUIVector3* akAxis);
		void FromAxes (const CGUIVector3& xAxis, const CGUIVector3& yAxis, const CGUIVector3& zAxis);
		void ToAxes (CGUIVector3* akAxis) const;
		void ToAxes (CGUIVector3& xAxis, CGUIVector3& yAxis, CGUIVector3& zAxis) const;
		/// Get the local x-axis
		CGUIVector3 xAxis(void) const;
		/// Get the local y-axis
		CGUIVector3 yAxis(void) const;
		/// Get the local z-axis
		CGUIVector3 zAxis(void) const;

		inline CGUIQuaternion& operator= (const CGUIQuaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
		CGUIQuaternion operator+ (const CGUIQuaternion& rkQ) const;
		CGUIQuaternion operator- (const CGUIQuaternion& rkQ) const;
		CGUIQuaternion operator* (const CGUIQuaternion& rkQ) const;
		CGUIQuaternion operator* (real fScalar) const;
		GUIEXPORT friend CGUIQuaternion operator* (real fScalar,const CGUIQuaternion& rkQ);
		CGUIQuaternion operator- () const;
		inline bool operator== (const CGUIQuaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) &&
				(rhs.z == z) && (rhs.w == w);
		}
		inline bool operator!= (const CGUIQuaternion& rhs) const
		{
			return !operator==(rhs);
		}
		// functions of a quaternion
		real Dot (const CGUIQuaternion& rkQ) const;  // dot product
		real Norm () const;  // squared-length
		/// Normalises this quaternion, and returns the previous length
		real Normalise(void); 
		CGUIQuaternion Inverse () const;  // apply to non-zero quaternion
		CGUIQuaternion UnitInverse () const;  // apply to unit-length quaternion
		CGUIQuaternion Exp () const;
		CGUIQuaternion Log () const;

		// rotation of a vector by a quaternion
		CGUIVector3 operator* (const CGUIVector3& rkVector) const;

		/// Calculate the local roll element of this quaternion
		CGUIRadian getRoll(void) const;
		/// Calculate the local pitch element of this quaternion
		CGUIRadian getPitch(void) const;
		/// Calculate the local yaw element of this quaternion
		CGUIRadian getYaw(void) const;		
		/// Equality with tolerance (tolerance is max angle difference)
		bool equals(const CGUIQuaternion& rhs, const CGUIRadian& tolerance) const;

		// spherical linear interpolation
		static CGUIQuaternion Slerp (real fT, const CGUIQuaternion& rkP,const CGUIQuaternion& rkQ, bool shortestPath = false);

		static CGUIQuaternion SlerpExtraSpins (real fT,
			const CGUIQuaternion& rkP, const CGUIQuaternion& rkQ,
			int iExtraSpins);

		// setup for spherical quadratic interpolation
		static void Intermediate (const CGUIQuaternion& rkQ0,
			const CGUIQuaternion& rkQ1, const CGUIQuaternion& rkQ2,
			CGUIQuaternion& rka, CGUIQuaternion& rkB);

		// spherical quadratic interpolation
		static CGUIQuaternion Squad (real fT, const CGUIQuaternion& rkP,
			const CGUIQuaternion& rkA, const CGUIQuaternion& rkB,
			const CGUIQuaternion& rkQ, bool shortestPath = false);

		// normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
		static CGUIQuaternion nlerp(real fT, const CGUIQuaternion& rkP, 
			const CGUIQuaternion& rkQ, bool shortestPath = false);

		// cutoff for sine near zero
		static const real ms_fEpsilon;

		// special values
		static const CGUIQuaternion ZERO;
		static const CGUIQuaternion IDENTITY;

		real w, x, y, z;

	};

}




#endif	//__GUI_QUATERNION_20071123_H__
