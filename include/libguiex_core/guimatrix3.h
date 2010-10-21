/** 
* @file guimatrix3.h
* @brief 3x3 matrix
* @author ken
* @date 2007-11-23
*/


#ifndef __GUI_MATRIX3_20071123_H__
#define __GUI_MATRIX3_20071123_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"


// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIDegree;
	class CGUIRadian;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** A 3x3 matrix which can represent rotations around axes.
	The coordinate system is assumed to be <b>right-handed</b>.
	*/
	class GUIEXPORT CGUIMatrix3
	{
	public:
		/** Default constructor.
		@note
		It does <b>NOT</b> initialize the matrix for efficiency.
		*/
		CGUIMatrix3 ();

		explicit CGUIMatrix3 (const real arr[3][3]);

		CGUIMatrix3 (const CGUIMatrix3& rkMatrix);

		CGUIMatrix3 (real fEntry00, real fEntry01, real fEntry02,
			real fEntry10, real fEntry11, real fEntry12,
			real fEntry20, real fEntry21, real fEntry22);

		//!< member access, allows use of construct mat[r][c]
		real* operator[] (size_t iRow) const;


		CGUIVector3 GetColumn (size_t iCol) const;
		void SetColumn(size_t iCol, const CGUIVector3& vec);
		void FromAxes(const CGUIVector3& xAxis, const CGUIVector3& yAxis, const CGUIVector3& zAxis);

		// assignment and comparison
		inline CGUIMatrix3& operator= (const CGUIMatrix3& rkMatrix);
		bool operator== (const CGUIMatrix3& rkMatrix) const;
		inline bool operator!= (const CGUIMatrix3& rkMatrix) const;

		// arithmetic operations
		CGUIMatrix3 operator+ (const CGUIMatrix3& rkMatrix) const;
		CGUIMatrix3 operator- (const CGUIMatrix3& rkMatrix) const;
		CGUIMatrix3 operator* (const CGUIMatrix3& rkMatrix) const;
		CGUIMatrix3 operator- () const;

		// matrix * vector [3x3 * 3x1 = 3x1]
		CGUIVector3 operator* (const CGUIVector3& rkVector) const;

		// vector * matrix [1x3 * 3x3 = 1x3]
		GUIEXPORT friend CGUIVector3 operator* (const CGUIVector3& rkVector,const CGUIMatrix3& rkMatrix);

		// matrix * scalar
		CGUIMatrix3 operator* (real fScalar) const;

		// scalar * matrix
		GUIEXPORT friend CGUIMatrix3 operator* (real fScalar, const CGUIMatrix3& rkMatrix);

		// utilities
		CGUIMatrix3 Transpose () const;
		bool Inverse (CGUIMatrix3& rkInverse, real fTolerance = 1e-06) const;
		CGUIMatrix3 Inverse (real fTolerance = 1e-06) const;
		real Determinant () const;

		// singular value decomposition
		void SingularValueDecomposition (CGUIMatrix3& rkL, CGUIVector3& rkS,CGUIMatrix3& rkR) const;
		void SingularValueComposition (const CGUIMatrix3& rkL,const CGUIVector3& rkS, const CGUIMatrix3& rkR);

		// Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
		void Orthonormalize ();

		// orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
		void QDUDecomposition (CGUIMatrix3& rkQ, CGUIVector3& rkD,CGUIVector3& rkU) const;

		real SpectralNorm () const;

		// matrix must be orthonormal
		void ToAxisAngle (CGUIVector3& rkAxis, CGUIRadian& rfAngle) const;
		void ToAxisAngle (CGUIVector3& rkAxis, CGUIDegree& rfAngle) const;

		void FromAxisAngle (const CGUIVector3& rkAxis, const CGUIRadian& fRadians);

		inline void ToAxisAngle (CGUIVector3& rkAxis, real& rfRadians) const;
		inline void FromAxisAngle (const CGUIVector3& rkAxis, real fRadians);

		// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
		// where yaw is rotation about the Up vector, pitch is rotation about the
		// Right axis, and roll is rotation about the Direction axis.
		bool ToEulerAnglesXYZ (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		bool ToEulerAnglesXZY (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		bool ToEulerAnglesYXZ (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		bool ToEulerAnglesYZX (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		bool ToEulerAnglesZXY (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		bool ToEulerAnglesZYX (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,CGUIRadian& rfRAngle) const;
		void FromEulerAnglesXYZ (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);
		void FromEulerAnglesXZY (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);
		void FromEulerAnglesYXZ (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);
		void FromEulerAnglesYZX (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);
		void FromEulerAnglesZXY (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);
		void FromEulerAnglesZYX (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle, const CGUIRadian& fRAngle);

		inline bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
		inline bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const ;
		inline bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
		inline bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const ;
		inline bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
		inline bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
		inline void FromEulerAnglesXYZ (float fYAngle, float fPAngle, float fRAngle);
		inline void FromEulerAnglesXZY (float fYAngle, float fPAngle, float fRAngle);
		inline void FromEulerAnglesYXZ (float fYAngle, float fPAngle, float fRAngle) ;
		inline void FromEulerAnglesYZX (float fYAngle, float fPAngle, float fRAngle);
		inline void FromEulerAnglesZXY (float fYAngle, float fPAngle, float fRAngle);
		inline void FromEulerAnglesZYX (float fYAngle, float fPAngle, float fRAngle);

		// eigensolver, matrix must be symmetric
		void EigenSolveSymmetric (real afEigenvalue[3],CGUIVector3 akEigenvector[3]) const;

		static void TensorProduct (const CGUIVector3& rkU, const CGUIVector3& rkV,CGUIMatrix3& rkProduct);

		static const real EPSILON;
		static const CGUIMatrix3 ZERO;
		static const CGUIMatrix3 IDENTITY;

	protected:
		// support for eigensolver
		void Tridiagonal (real afDiag[3], real afSubDiag[3]);
		bool QLAlgorithm (real afDiag[3], real afSubDiag[3]);

		// support for singular value decomposition
		static const real ms_fSvdEpsilon;
		static const unsigned int ms_iSvdMaxIterations;
		static void Bidiagonalize (CGUIMatrix3& kA, CGUIMatrix3& kL,CGUIMatrix3& kR);
		static void GolubKahanStep (CGUIMatrix3& kA, CGUIMatrix3& kL,CGUIMatrix3& kR);

		// support for spectral norm
		static real MaxCubicRoot (real afCoeff[3]);

		real m[3][3];

		// for faster access
		friend class CGUIMatrix4;
	};
}
#endif	//__GUI_MATRIX3_20071123_H__
