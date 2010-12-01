/** 
* @file guimatrix4.cpp
* @brief 4x4 matrix
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guimatrix4.h>



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
	CGUIMatrix4::CGUIMatrix4(const CGUIMatrix3& m3x3)
	{
		operator=(CGUIMatrix4::IDENTITY);
		operator=(m3x3);
	}
	//-----------------------------------------------------------------------
	CGUIMatrix4::CGUIMatrix4(const CGUIQuaternion& rot)
	{
		CGUIMatrix3 m3x3;
		rot.ToRotationMatrix(m3x3);
		operator=(CGUIMatrix4::IDENTITY);
		operator=(m3x3);
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
}
