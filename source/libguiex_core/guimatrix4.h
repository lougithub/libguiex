/** 
* @file guimatrix4.h
* @brief 4x4 matrix
* @author ken
* @date 2007-11-23
*/

#ifndef __GUI_MATRIX4_20071123_H__
#define __GUI_MATRIX4_20071123_H__



//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"
#include "guivector4.h"
#include "guirotator.h"
#include "guimatrix3.h"
#include "guiquaternion.h"
#include "guiplane.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIQuaternion;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** 
	* Class encapsulating a standard 4x4 homogenous matrix.
	@remarks
	LIBGUIEX uses column vectors when applying matrix multiplications,
	This means a vector is represented as a single column, 4-row
	matrix. This has the effect that the tranformations implemented
	by the matrices happens right-to-left e.g. if vector V is to be
	transformed by M1 then M2 then M3, the calculation would be
	M3 * M2 * M1 * V. The order that matrices are concatenated is
	vital since matrix multiplication is not cummatative, i.e. you
	can get a different result if you concatenate in the wrong order.
	@par
	The use of column vectors and right-to-left ordering is the
	standard in most mathematical texts, and id the same as used in
	OpenGL. It is, however, the opposite of Direct3D, which has
	inexplicably chosen to differ from the accepted standard and uses
	row vectors and left-to-right matrix multiplication.
	@par
	LIBGUIEX deals with the differences between D3D and OpenGL etc.
	internally when operating through different render systems. LIBGUIEX
	users only need to conform to standard maths conventions, i.e.
	right-to-left matrix multiplication, (LIBGUIEX transposes matrices it
	passes to D3D to compensate).
	@par
	The generic form M * V which shows the layout of the matrix 
	entries is shown below:
	<pre>
	[ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
	| m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
	| m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
	[ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
	</pre>
	*/
	class GUIEXPORT CGUIMatrix4
	{
	public:
		/// The matrix entries, indexed by [row][col].
		union 
		{
			real m[4][4];
			real _m[16];
		};

	public:
		inline CGUIMatrix4()
		{
		}

		inline CGUIMatrix4(
			real m00, real m01, real m02, real m03,
			real m10, real m11, real m12, real m13,
			real m20, real m21, real m22, real m23,
			real m30, real m31, real m32, real m33 )
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;
			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		CGUIMatrix4(const CGUIMatrix3& m3x3);
		CGUIMatrix4(const CGUIQuaternion& Q);
		CGUIMatrix4(const CGUIRotator& Rot );

		inline real* operator [] ( size_t iRow )
		{
			assert( iRow < 4 );
			return m[iRow];
		}

		inline const real *const operator [] ( size_t iRow ) const
		{
			assert( iRow < 4 );
			return m[iRow];
		}

		inline CGUIMatrix4 concatenate(const CGUIMatrix4 &m2) const
		{
			CGUIMatrix4 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
		}

		/** Matrix concatenation using '*'.
		*/
		inline CGUIMatrix4 operator * ( const CGUIMatrix4 &m2 ) const
		{
			return concatenate( m2 );
		}

		/** Vector transformation using '*'.
		@remarks
		Transforms the given 3-D vector by the matrix, projecting the 
		result back into <i>w</i> = 1.
		@note
		This means that the initial <i>w</i> is considered to be 1.0,
		and then all the tree elements of the resulting 3-D vector are
		divided by the resulting <i>w</i>.
		*/
		inline CGUIVector3 operator * ( const CGUIVector3 &v ) const
		{
			CGUIVector3 r;

			real fInvW = 1.0f / ( m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] );

			r.x = ( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] ) * fInvW;
			r.y = ( m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] ) * fInvW;
			r.z = ( m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] ) * fInvW;

			return r;
		}
		inline CGUIVector4 operator * (const CGUIVector4& v) const
		{
			return CGUIVector4(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
				);
		}
		inline CGUIPlane operator * (const CGUIPlane& p) const
		{
			CGUIPlane ret;
			CGUIMatrix4 invTrans = inverse().transpose();
			ret.normal.x =
				invTrans[0][0] * p.normal.x + invTrans[0][1] * p.normal.y + invTrans[0][2] * p.normal.z;
			ret.normal.y = 
				invTrans[1][0] * p.normal.x + invTrans[1][1] * p.normal.y + invTrans[1][2] * p.normal.z;
			ret.normal.z = 
				invTrans[2][0] * p.normal.x + invTrans[2][1] * p.normal.y + invTrans[2][2] * p.normal.z;
			CGUIVector3 pt = p.normal * -p.d;
			pt = *this * pt;
			ret.d = - pt.DotProduct(ret.normal);
			return ret;
		}


		/** Matrix addition.
		*/
		inline CGUIMatrix4 operator + ( const CGUIMatrix4 &m2 ) const
		{
			CGUIMatrix4 r;

			r.m[0][0] = m[0][0] + m2.m[0][0];
			r.m[0][1] = m[0][1] + m2.m[0][1];
			r.m[0][2] = m[0][2] + m2.m[0][2];
			r.m[0][3] = m[0][3] + m2.m[0][3];

			r.m[1][0] = m[1][0] + m2.m[1][0];
			r.m[1][1] = m[1][1] + m2.m[1][1];
			r.m[1][2] = m[1][2] + m2.m[1][2];
			r.m[1][3] = m[1][3] + m2.m[1][3];

			r.m[2][0] = m[2][0] + m2.m[2][0];
			r.m[2][1] = m[2][1] + m2.m[2][1];
			r.m[2][2] = m[2][2] + m2.m[2][2];
			r.m[2][3] = m[2][3] + m2.m[2][3];

			r.m[3][0] = m[3][0] + m2.m[3][0];
			r.m[3][1] = m[3][1] + m2.m[3][1];
			r.m[3][2] = m[3][2] + m2.m[3][2];
			r.m[3][3] = m[3][3] + m2.m[3][3];

			return r;
		}

		/** Matrix subtraction.
		*/
		inline CGUIMatrix4 operator - ( const CGUIMatrix4 &m2 ) const
		{
			CGUIMatrix4 r;
			r.m[0][0] = m[0][0] - m2.m[0][0];
			r.m[0][1] = m[0][1] - m2.m[0][1];
			r.m[0][2] = m[0][2] - m2.m[0][2];
			r.m[0][3] = m[0][3] - m2.m[0][3];

			r.m[1][0] = m[1][0] - m2.m[1][0];
			r.m[1][1] = m[1][1] - m2.m[1][1];
			r.m[1][2] = m[1][2] - m2.m[1][2];
			r.m[1][3] = m[1][3] - m2.m[1][3];

			r.m[2][0] = m[2][0] - m2.m[2][0];
			r.m[2][1] = m[2][1] - m2.m[2][1];
			r.m[2][2] = m[2][2] - m2.m[2][2];
			r.m[2][3] = m[2][3] - m2.m[2][3];

			r.m[3][0] = m[3][0] - m2.m[3][0];
			r.m[3][1] = m[3][1] - m2.m[3][1];
			r.m[3][2] = m[3][2] - m2.m[3][2];
			r.m[3][3] = m[3][3] - m2.m[3][3];

			return r;
		}

		/** Tests 2 matrices for equality.
		*/
		inline bool operator == ( const CGUIMatrix4& m2 ) const
		{
			if( 
				m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
				m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
				return false;
			return true;
		}

		/** Tests 2 matrices for inequality.
		*/
		inline bool operator != ( const CGUIMatrix4& m2 ) const
		{
			if( 
				m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
				m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
				return true;
			return false;
		}

		/** Assignment from 3x3 matrix.
		*/
		inline void operator = ( const CGUIMatrix3& mat3 )
		{
			m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
			m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
			m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
		}

		inline CGUIMatrix4 transpose(void) const
		{
			return CGUIMatrix4(m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		/*
		-----------------------------------------------------------------------
		Translation Transformation
		-----------------------------------------------------------------------
		*/
		/** Sets the translation transformation part of the matrix.
		*/
		inline void setTrans( const CGUIVector3& v )
		{
			m[0][3] = v.x;
			m[1][3] = v.y;
			m[2][3] = v.z;
		}

		/** Extracts the translation transformation part of the matrix.
		*/
		inline CGUIVector3 getTrans() const
		{
			return CGUIVector3(m[0][3], m[1][3], m[2][3]);
		}


		/** Builds a translation matrix
		*/
		inline void makeTrans( const CGUIVector3& v )
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		inline void makeTrans( real tx, real ty, real tz )
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		/** Gets a translation matrix.
		*/
		inline static CGUIMatrix4 getTrans( const CGUIVector3& v )
		{
			return getTrans( v.x, v.y, v.z );
		}

		/** Gets a translation matrix - variation for not using a vector.
		*/
		inline static CGUIMatrix4 getTrans( real t_x, real t_y, real t_z )
		{
			return CGUIMatrix4(
				1.0, 0.0, 0.0, t_x,
				0.0, 1.0, 0.0, t_y,
				0.0, 0.0, 1.0, t_z,
				0.0, 0.0, 0.0, 1.0 );
		}

		/*
		-----------------------------------------------------------------------
		Scale Transformation
		-----------------------------------------------------------------------
		*/
		/** Sets the scale part of the matrix.
		*/
		inline void setScale( const CGUIVector3& v )
		{
			m[0][0] = v.x;
			m[1][1] = v.y;
			m[2][2] = v.z;
		}

		/** Gets a scale matrix.
		*/
		inline static CGUIMatrix4 getScale( const CGUIVector3& v )
		{
			CGUIMatrix4 r;
			r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		/** Gets a scale matrix - variation for not using a vector.
		*/
		inline static CGUIMatrix4 getScale( real s_x, real s_y, real s_z )
		{
			CGUIMatrix4 r;
			r.m[0][0] = s_x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = s_y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = s_z; r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		/** Extracts the rotation / scaling part of the Matrix as a 3x3 matrix. 
		@param m3x3 Destination CGUIMatrix3
		*/
		inline void extract3x3Matrix(CGUIMatrix3& m3x3) const
		{
			m3x3.m[0][0] = m[0][0];
			m3x3.m[0][1] = m[0][1];
			m3x3.m[0][2] = m[0][2];
			m3x3.m[1][0] = m[1][0];
			m3x3.m[1][1] = m[1][1];
			m3x3.m[1][2] = m[1][2];
			m3x3.m[2][0] = m[2][0];
			m3x3.m[2][1] = m[2][1];
			m3x3.m[2][2] = m[2][2];

		}

		/** Extracts the rotation / scaling part as a quaternion from the Matrix.
		*/
		inline CGUIQuaternion extractQuaternion() const
		{
			CGUIMatrix3 m3x3;
			extract3x3Matrix(m3x3);
			return CGUIQuaternion(m3x3);
		}

		static const CGUIMatrix4 ZERO;
		static const CGUIMatrix4 IDENTITY;
		/** Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
		and inverts the Y. */
		static const CGUIMatrix4 CLIPSPACE2DTOIMAGESPACE;

		inline CGUIMatrix4 operator*(real scalar)
		{
			return CGUIMatrix4(
				scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
				scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
				scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
				scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
		}


		CGUIMatrix4 adjoint() const;
		real determinant() const;
		CGUIMatrix4 inverse() const;

		/** Building a Matrix4 from orientation / scale / position.
        @remarks
            Transform is performed in the order scale, rotate, translation, i.e. translation is independent
            of orientation axes, scale does not affect size of translation, rotation and scaling are always
            centered on the origin.
        */
        void makeTransform(const CGUIVector3& position, const CGUIVector3& scale, const CGUIQuaternion& orientation);

        /** Building an inverse Matrix4 from orientation / scale / position.
        @remarks
            As makeTransform except it build the inverse given the same data as makeTransform, so
            performing -translation, -rotate, 1/scale in that order.
        */
        void makeInverseTransform(const CGUIVector3& position, const CGUIVector3& scale, const CGUIQuaternion& orientation);

		inline CGUIVector3 GetAxis(uint32 i) const
		{
			GUI_ASSERT(i >= 0 && i <= 2, "invalid parameter");
			return CGUIVector3(m[i][0], m[i][1], m[i][2]);
		}

		CGUIRotator Rotator() const;
	};

	/* Removed from CGUIVector4 and made a non-member here because otherwise
	OgreMatrix4.h and OgreVector4.h have to try to include and inline each 
	other, which frankly doesn't work ;)
	*/
	inline CGUIVector4 operator * (const CGUIVector4& v, const CGUIMatrix4& mat)
	{
		return CGUIVector4(
			v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
			v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
			v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
			v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
		);
	}


}
#endif
