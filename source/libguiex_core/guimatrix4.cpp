/** 
* @file guimatrix4.cpp
* @brief 4x4 matrix
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include
//============================================================================// 
#include "guimatrix4.h"



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
//-----------------------------------------------------------------------
	const CGUIMatrix4 CGUIMatrix4::ZERO(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 );
//-----------------------------------------------------------------------
	const CGUIMatrix4 CGUIMatrix4::IDENTITY(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );
//-----------------------------------------------------------------------
	const CGUIMatrix4 CGUIMatrix4::CLIPSPACE2DTOIMAGESPACE(
		0.5,    0,  0, 0.5, 
		0,	-0.5,  0, 0.5, 
		0,    0,  1,   0,
		0,    0,  0,   1);
//-----------------------------------------------------------------------
	real MINOR(const CGUIMatrix4& m, const size_t r0, const size_t r1, const size_t r2, 
		const size_t c0, const size_t c1, const size_t c2)
	{
		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
			m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
	}
	//-----------------------------------------------------------------------
	/** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling 3x3 matrix.
	*/
	CGUIMatrix4::CGUIMatrix4(const CGUIMatrix3& m3x3)
	{
		operator=(CGUIMatrix4::IDENTITY);
		operator=(m3x3);
	}
	//-----------------------------------------------------------------------
	/** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling CGUIQuaternion.
	*/
	CGUIMatrix4::CGUIMatrix4(const CGUIQuaternion& Q)
	{
#if 0
		CGUIMatrix3 m3x3;
		rot.ToRotationMatrix(m3x3);
		operator=(CGUIMatrix4::IDENTITY);
		operator=(m3x3);
#else
		const real x2 = Q.x + Q.x;  const real y2 = Q.y + Q.y;  const real z2 = Q.z + Q.z;
		const real xx = Q.x * x2;   const real xy = Q.x * y2;   const real xz = Q.x * z2;
		const real yy = Q.y * y2;   const real yz = Q.y * z2;   const real zz = Q.z * z2;
		const real wx = Q.w * x2;   const real wy = Q.w * y2;   const real wz = Q.w * z2;

		m[0][0] = 1.0f - (yy + zz);	m[1][0] = xy - wz;				m[2][0] = xz + wy;			m[3][0] = 0;
		m[0][1] = xy + wz;			m[1][1] = 1.0f - (xx + zz);		m[2][1] = yz - wx;			m[3][1] = 0;
		m[0][2] = xz - wy;			m[1][2] = yz + wx;				m[2][2] = 1.0f - (xx + yy);	m[3][2] = 0;
		m[0][3] = 0.0f;				m[1][3] = 0.0f;					m[2][3] = 0.0f;				m[3][3] = 1.0f;
#endif
	}
	//-----------------------------------------------------------------------
	CGUIMatrix4::CGUIMatrix4(const CGUIRotator& Rot )
	{
		const real	SR	= sin(Rot.Roll / 180.f * CGUIMath::GUI_PI);
		const real	SP	=sin(Rot.Pitch / 180.f * CGUIMath::GUI_PI);
		const real	SY	=sin(Rot.Yaw / 180.f * CGUIMath::GUI_PI);
		const real	CR	= cos(Rot.Roll / 180.f * CGUIMath::GUI_PI);
		const real	CP	= cos(Rot.Pitch / 180.f * CGUIMath::GUI_PI);
		const real	CY	= cos(Rot.Yaw / 180.f * CGUIMath::GUI_PI);

		m[0][0]	= CP * CY;
		m[0][1]	= CP * SY;
		m[0][2]	= SP;
		m[0][3]	= 0.f;

		m[1][0]	= SR * SP * CY - CR * SY;
		m[1][1]	= SR * SP * SY + CR * CY;
		m[1][2]	= - SR * CP;
		m[1][3]	= 0.f;

		m[2][0]	= -( CR * SP * CY + SR * SY );
		m[2][1]	= CY * SR - CR * SP * SY;
		m[2][2]	= CR * CP;
		m[2][3]	= 0.f;

		m[3][0]	= 0;
		m[3][1]	= 0;
		m[3][2]	= 0;
		m[3][3]	= 1.f;
	}
//-----------------------------------------------------------------------
	CGUIMatrix4 CGUIMatrix4::adjoint() const
	{
		return CGUIMatrix4( MINOR(*this, 1, 2, 3, 1, 2, 3),
			-MINOR(*this, 0, 2, 3, 1, 2, 3),
			MINOR(*this, 0, 1, 3, 1, 2, 3),
			-MINOR(*this, 0, 1, 2, 1, 2, 3),

			-MINOR(*this, 1, 2, 3, 0, 2, 3),
			MINOR(*this, 0, 2, 3, 0, 2, 3),
			-MINOR(*this, 0, 1, 3, 0, 2, 3),
			MINOR(*this, 0, 1, 2, 0, 2, 3),

			MINOR(*this, 1, 2, 3, 0, 1, 3),
			-MINOR(*this, 0, 2, 3, 0, 1, 3),
			MINOR(*this, 0, 1, 3, 0, 1, 3),
			-MINOR(*this, 0, 1, 2, 0, 1, 3),

			-MINOR(*this, 1, 2, 3, 0, 1, 2),
			MINOR(*this, 0, 2, 3, 0, 1, 2),
			-MINOR(*this, 0, 1, 3, 0, 1, 2),
			MINOR(*this, 0, 1, 2, 0, 1, 2));
	}
//-----------------------------------------------------------------------
	real CGUIMatrix4::determinant() const
	{
		return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
			m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
			m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
			m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
	}
//-----------------------------------------------------------------------
	CGUIMatrix4 CGUIMatrix4::inverse() const
	{
		return adjoint() * (1.0f / determinant());
	}
//-----------------------------------------------------------------------
	void CGUIMatrix4::makeTransform(const CGUIVector3& position, const CGUIVector3& scale, const CGUIQuaternion& orientation)
	{
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		CGUIMatrix3 rot3x3;
		orientation.ToRotationMatrix(rot3x3);

		// Set up final matrix with scale, rotation and translation
		m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
		m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
		m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

		// No projection term
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix4::makeInverseTransform(const CGUIVector3& position, const CGUIVector3& scale, const CGUIQuaternion& orientation)
	{
		// Invert the parameters
		CGUIVector3 invTranslate = -position;
		CGUIVector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
		CGUIQuaternion invRot = orientation.Inverse();

		// Because we're inverting, order is translation, rotation, scale
		// So make translation relative to scale & rotation
		invTranslate = invRot * invTranslate; // rotate
		invTranslate *= invScale; // scale

		// Next, make a 3x3 rotation matrix
		CGUIMatrix3 rot3x3;
		invRot.ToRotationMatrix(rot3x3);

		// Set up final matrix with scale, rotation and translation
		m[0][0] = invScale.x * rot3x3[0][0]; m[0][1] = invScale.x * rot3x3[0][1]; m[0][2] = invScale.x * rot3x3[0][2]; m[0][3] = invTranslate.x;
		m[1][0] = invScale.y * rot3x3[1][0]; m[1][1] = invScale.y * rot3x3[1][1]; m[1][2] = invScale.y * rot3x3[1][2]; m[1][3] = invTranslate.y;
		m[2][0] = invScale.z * rot3x3[2][0]; m[2][1] = invScale.z * rot3x3[2][1]; m[2][2] = invScale.z * rot3x3[2][2]; m[2][3] = invTranslate.z;		

		// No projection term
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}
	//-----------------------------------------------------------------------
	CGUIRotator CGUIMatrix4::Rotator() const
	{
		const CGUIVector3 XAxis	= GetAxis( 0 );
		const CGUIVector3 YAxis	= GetAxis( 1 );
		const CGUIVector3 ZAxis	= GetAxis( 2 );

		CGUIRotator	Rotator	= CGUIRotator( 
			atan2f( XAxis.z, sqrt(XAxis.x*XAxis.x+XAxis.y*XAxis.y) ) * 180.f / CGUIMath::GUI_PI, 
			atan2f( XAxis.y, XAxis.x ) * 180.f / CGUIMath::GUI_PI, 
			0.f
			);

		const CGUIVector3 SYAxis = CGUIMatrix4( Rotator ).GetAxis(1);
		Rotator.Roll = atan2f( ZAxis | SYAxis, YAxis | SYAxis ) * 180.f / CGUIMath::GUI_PI;
		return Rotator;
	}
	//-----------------------------------------------------------------------
}
