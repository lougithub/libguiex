/** 
* @file guimatrix3.cpp
* @brief 3x3 matrix
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guimatrix3.h>
#include <libguiex_core\guimath.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	const real CGUIMatrix3::EPSILON = 1e-06f;
	const CGUIMatrix3 CGUIMatrix3::ZERO(0,0,0,0,0,0,0,0,0);
	const CGUIMatrix3 CGUIMatrix3::IDENTITY(1,0,0,0,1,0,0,0,1);
	const real CGUIMatrix3::ms_fSvdEpsilon = 1e-04f;
	const unsigned int CGUIMatrix3::ms_iSvdMaxIterations = 32;
	//-----------------------------------------------------------------------
	CGUIMatrix3::CGUIMatrix3 () 
	{
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3::CGUIMatrix3 (const real arr[3][3])
	{
		memcpy(m,arr,9*sizeof(real));
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3::CGUIMatrix3 (const CGUIMatrix3& rkMatrix)
	{
		memcpy(m,rkMatrix.m,9*sizeof(real));
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3::CGUIMatrix3 (real fEntry00, real fEntry01, real fEntry02,
		real fEntry10, real fEntry11, real fEntry12,
		real fEntry20, real fEntry21, real fEntry22)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
	}
	//-----------------------------------------------------------------------
	real* CGUIMatrix3::operator[] (size_t iRow) const
	{
		return (real*)m[iRow];
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3& CGUIMatrix3::operator= (const CGUIMatrix3& rkMatrix)
	{
		memcpy(m,rkMatrix.m,9*sizeof(real));
		return *this;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::operator!= (const CGUIMatrix3& rkMatrix) const
	{
		return !operator==(rkMatrix);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::ToAxisAngle (CGUIVector3& rkAxis, CGUIDegree& rfAngle) const 
	{
		CGUIRadian r;
		ToAxisAngle ( rkAxis, r );
		rfAngle = r;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::ToAxisAngle (CGUIVector3& rkAxis, real& rfRadians) const
	{
		CGUIRadian r;
		ToAxisAngle ( rkAxis, r );
		rfRadians = r.valueRadians();
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromAxisAngle (const CGUIVector3& rkAxis, real fRadians)
	{
		FromAxisAngle ( rkAxis, CGUIRadian(fRadians) );
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesXYZ (real& rfYAngle, real& rfPAngle,real& rfRAngle) const 
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesXYZ(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesXZY (real& rfYAngle, real& rfPAngle,real& rfRAngle) const 
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesXZY(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesYXZ (real& rfYAngle, real& rfPAngle,real& rfRAngle) const 
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesYXZ(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesYZX (real& rfYAngle, real& rfPAngle,real& rfRAngle) const 
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesYZX(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesZXY (real& rfYAngle, real& rfPAngle,real& rfRAngle) const 
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesZXY(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesZYX (real& rfYAngle, real& rfPAngle,real& rfRAngle) const
	{
			CGUIRadian y, p, r;
			bool b = ToEulerAnglesZYX(y,p,r);
			rfYAngle = y.valueRadians();
			rfPAngle = p.valueRadians();
			rfRAngle = r.valueRadians();
			return b;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesXYZ (real fYAngle, real fPAngle, real fRAngle) 
	{
		FromEulerAnglesXYZ ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesXZY (real fYAngle, real fPAngle, real fRAngle)
	{
		FromEulerAnglesXZY ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesYXZ (real fYAngle, real fPAngle, real fRAngle) 
	{
		FromEulerAnglesYXZ ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesYZX (real fYAngle, real fPAngle, real fRAngle) 
	{
		FromEulerAnglesYZX ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesZXY (real fYAngle, real fPAngle, real fRAngle) 
	{
		FromEulerAnglesZXY ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesZYX (real fYAngle, real fPAngle, real fRAngle) 
	{
		FromEulerAnglesZYX ( CGUIRadian(fYAngle), CGUIRadian(fPAngle), CGUIRadian(fRAngle) );
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIMatrix3::GetColumn (size_t iCol) const
	{
		assert( 0 <= iCol && iCol < 3 );
		return CGUIVector3(m[0][iCol],m[1][iCol],
			m[2][iCol]);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::SetColumn(size_t iCol, const CGUIVector3& vec)
	{
		assert( 0 <= iCol && iCol < 3 );
		m[0][iCol] = vec.x;
		m[1][iCol] = vec.y;
		m[2][iCol] = vec.z;

	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromAxes(const CGUIVector3& xAxis, const CGUIVector3& yAxis, const CGUIVector3& zAxis)
	{
		SetColumn(0,xAxis);
		SetColumn(1,yAxis);
		SetColumn(2,zAxis);

	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::operator== (const CGUIMatrix3& rkMatrix) const
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				if ( m[iRow][iCol] != rkMatrix.m[iRow][iCol] )
					return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::operator+ (const CGUIMatrix3& rkMatrix) const
	{
		CGUIMatrix3 kSum;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kSum.m[iRow][iCol] = m[iRow][iCol] +
					rkMatrix.m[iRow][iCol];
			}
		}
		return kSum;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::operator- (const CGUIMatrix3& rkMatrix) const
	{
		CGUIMatrix3 kDiff;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kDiff.m[iRow][iCol] = m[iRow][iCol] -
					rkMatrix.m[iRow][iCol];
			}
		}
		return kDiff;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::operator* (const CGUIMatrix3& rkMatrix) const
	{
		CGUIMatrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kProd.m[iRow][iCol] =
					m[iRow][0]*rkMatrix.m[0][iCol] +
					m[iRow][1]*rkMatrix.m[1][iCol] +
					m[iRow][2]*rkMatrix.m[2][iCol];
			}
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIMatrix3::operator* (const CGUIVector3& rkPoint) const
	{
		CGUIVector3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				m[iRow][0]*rkPoint[0] +
				m[iRow][1]*rkPoint[1] +
				m[iRow][2]*rkPoint[2];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	CGUIVector3 operator* (const CGUIVector3& rkPoint, const CGUIMatrix3& rkMatrix)
	{
		CGUIVector3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				rkPoint[0]*rkMatrix.m[0][iRow] +
				rkPoint[1]*rkMatrix.m[1][iRow] +
				rkPoint[2]*rkMatrix.m[2][iRow];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::operator- () const
	{
		CGUIMatrix3 kNeg;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kNeg[iRow][iCol] = -m[iRow][iCol];
		}
		return kNeg;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::operator* (real fScalar) const
	{
		CGUIMatrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar*m[iRow][iCol];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 operator* (real fScalar, const CGUIMatrix3& rkMatrix)
	{
		CGUIMatrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
		}
		return kProd;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::Transpose () const
	{
		CGUIMatrix3 kTranspose;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kTranspose[iRow][iCol] = m[iCol][iRow];
		}
		return kTranspose;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::Inverse (CGUIMatrix3& rkInverse, real fTolerance) const
	{
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.

		rkInverse[0][0] = m[1][1]*m[2][2] -
			m[1][2]*m[2][1];
		rkInverse[0][1] = m[0][2]*m[2][1] -
			m[0][1]*m[2][2];
		rkInverse[0][2] = m[0][1]*m[1][2] -
			m[0][2]*m[1][1];
		rkInverse[1][0] = m[1][2]*m[2][0] -
			m[1][0]*m[2][2];
		rkInverse[1][1] = m[0][0]*m[2][2] -
			m[0][2]*m[2][0];
		rkInverse[1][2] = m[0][2]*m[1][0] -
			m[0][0]*m[1][2];
		rkInverse[2][0] = m[1][0]*m[2][1] -
			m[1][1]*m[2][0];
		rkInverse[2][1] = m[0][1]*m[2][0] -
			m[0][0]*m[2][1];
		rkInverse[2][2] = m[0][0]*m[1][1] -
			m[0][1]*m[1][0];

		real fDet =
			m[0][0]*rkInverse[0][0] +
			m[0][1]*rkInverse[1][0]+
			m[0][2]*rkInverse[2][0];

		if ( CGUIMath::Abs(fDet) <= fTolerance )
			return false;

		real fInvDet = 1.0f/fDet;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				rkInverse[iRow][iCol] *= fInvDet;
		}

		return true;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUIMatrix3::Inverse (real fTolerance) const
	{
		CGUIMatrix3 kInverse = CGUIMatrix3::ZERO;
		Inverse(kInverse,fTolerance);
		return kInverse;
	}
	//-----------------------------------------------------------------------
	real CGUIMatrix3::Determinant () const
	{
		real fCofactor00 = m[1][1]*m[2][2] -
			m[1][2]*m[2][1];
		real fCofactor10 = m[1][2]*m[2][0] -
			m[1][0]*m[2][2];
		real fCofactor20 = m[1][0]*m[2][1] -
			m[1][1]*m[2][0];

		real fDet =
			m[0][0]*fCofactor00 +
			m[0][1]*fCofactor10 +
			m[0][2]*fCofactor20;

		return fDet;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::Bidiagonalize (CGUIMatrix3& kA, CGUIMatrix3& kL,
		CGUIMatrix3& kR)
	{
		real afV[3], afW[3];
		real fLength, fSign, fT1, fInvT1, fT2;
		bool bIdentity;

		// map first column to (*,0,0)
		fLength = CGUIMath::Sqrt(kA[0][0]*kA[0][0] + kA[1][0]*kA[1][0] +
			kA[2][0]*kA[2][0]);
		if ( fLength > 0.0 )
		{
			fSign = (kA[0][0] > 0.0 ? 1.0f : -1.0f);
			fT1 = kA[0][0] + fSign*fLength;
			fInvT1 = 1.0f/fT1;
			afV[1] = kA[1][0]*fInvT1;
			afV[2] = kA[2][0]*fInvT1;

			fT2 = -2.0f/(1.0f+afV[1]*afV[1]+afV[2]*afV[2]);
			afW[0] = fT2*(kA[0][0]+kA[1][0]*afV[1]+kA[2][0]*afV[2]);
			afW[1] = fT2*(kA[0][1]+kA[1][1]*afV[1]+kA[2][1]*afV[2]);
			afW[2] = fT2*(kA[0][2]+kA[1][2]*afV[1]+kA[2][2]*afV[2]);
			kA[0][0] += afW[0];
			kA[0][1] += afW[1];
			kA[0][2] += afW[2];
			kA[1][1] += afV[1]*afW[1];
			kA[1][2] += afV[1]*afW[2];
			kA[2][1] += afV[2]*afW[1];
			kA[2][2] += afV[2]*afW[2];

			kL[0][0] = 1.0f+fT2;
			kL[0][1] = kL[1][0] = fT2*afV[1];
			kL[0][2] = kL[2][0] = fT2*afV[2];
			kL[1][1] = 1.0f+fT2*afV[1]*afV[1];
			kL[1][2] = kL[2][1] = fT2*afV[1]*afV[2];
			kL[2][2] = 1.0f+fT2*afV[2]*afV[2];
			bIdentity = false;
		}
		else
		{
			kL = CGUIMatrix3::IDENTITY;
			bIdentity = true;
		}

		// map first row to (*,*,0)
		fLength = CGUIMath::Sqrt(kA[0][1]*kA[0][1]+kA[0][2]*kA[0][2]);
		if ( fLength > 0.0 )
		{
			fSign = (kA[0][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = kA[0][1] + fSign*fLength;
			afV[2] = kA[0][2]/fT1;

			fT2 = -2.0f/(1.0f+afV[2]*afV[2]);
			afW[0] = fT2*(kA[0][1]+kA[0][2]*afV[2]);
			afW[1] = fT2*(kA[1][1]+kA[1][2]*afV[2]);
			afW[2] = fT2*(kA[2][1]+kA[2][2]*afV[2]);
			kA[0][1] += afW[0];
			kA[1][1] += afW[1];
			kA[1][2] += afW[1]*afV[2];
			kA[2][1] += afW[2];
			kA[2][2] += afW[2]*afV[2];

			kR[0][0] = 1.0f;
			kR[0][1] = kR[1][0] = 0.0f;
			kR[0][2] = kR[2][0] = 0.0f;
			kR[1][1] = 1.0f+fT2;
			kR[1][2] = kR[2][1] = fT2*afV[2];
			kR[2][2] = 1.0f+fT2*afV[2]*afV[2];
		}
		else
		{
			kR = CGUIMatrix3::IDENTITY;
		}

		// map second column to (*,*,0)
		fLength = CGUIMath::Sqrt(kA[1][1]*kA[1][1]+kA[2][1]*kA[2][1]);
		if ( fLength > 0.0f )
		{
			fSign = (kA[1][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = kA[1][1] + fSign*fLength;
			afV[2] = kA[2][1]/fT1;

			fT2 = -2.0f/(1.0f+afV[2]*afV[2]);
			afW[1] = fT2*(kA[1][1]+kA[2][1]*afV[2]);
			afW[2] = fT2*(kA[1][2]+kA[2][2]*afV[2]);
			kA[1][1] += afW[1];
			kA[1][2] += afW[2];
			kA[2][2] += afV[2]*afW[2];

			real fA = 1.0f+fT2;
			real fB = fT2*afV[2];
			real fC = 1.0f+fB*afV[2];

			if ( bIdentity )
			{
				kL[0][0] = 1.0f;
				kL[0][1] = kL[1][0] = 0.0f;
				kL[0][2] = kL[2][0] = 0.0f;
				kL[1][1] = fA;
				kL[1][2] = kL[2][1] = fB;
				kL[2][2] = fC;
			}
			else
			{
				for (int iRow = 0; iRow < 3; iRow++)
				{
					real fTmp0 = kL[iRow][1];
					real fTmp1 = kL[iRow][2];
					kL[iRow][1] = fA*fTmp0+fB*fTmp1;
					kL[iRow][2] = fB*fTmp0+fC*fTmp1;
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::GolubKahanStep (CGUIMatrix3& kA, CGUIMatrix3& kL,
		CGUIMatrix3& kR)
	{
		real fT11 = kA[0][1]*kA[0][1]+kA[1][1]*kA[1][1];
		real fT22 = kA[1][2]*kA[1][2]+kA[2][2]*kA[2][2];
		real fT12 = kA[1][1]*kA[1][2];
		real fTrace = fT11+fT22;
		real fDiff = fT11-fT22;
		real fDiscr = CGUIMath::Sqrt(fDiff*fDiff+4.0f*fT12*fT12);
		real fRoot1 = 0.5f*(fTrace+fDiscr);
		real fRoot2 = 0.5f*(fTrace-fDiscr);

		// adjust right
		real fY = kA[0][0] - (CGUIMath::Abs(fRoot1-fT22) <=
			CGUIMath::Abs(fRoot2-fT22) ? fRoot1 : fRoot2);
		real fZ = kA[0][1];
		real fInvLength = CGUIMath::InvSqrt(fY*fY+fZ*fZ);
		real fSin = fZ*fInvLength;
		real fCos = -fY*fInvLength;

		real fTmp0 = kA[0][0];
		real fTmp1 = kA[0][1];
		kA[0][0] = fCos*fTmp0-fSin*fTmp1;
		kA[0][1] = fSin*fTmp0+fCos*fTmp1;
		kA[1][0] = -fSin*kA[1][1];
		kA[1][1] *= fCos;

		size_t iRow;
		for (iRow = 0; iRow < 3; iRow++)
		{
			fTmp0 = kR[0][iRow];
			fTmp1 = kR[1][iRow];
			kR[0][iRow] = fCos*fTmp0-fSin*fTmp1;
			kR[1][iRow] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust left
		fY = kA[0][0];
		fZ = kA[1][0];
		fInvLength = CGUIMath::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[0][0] = fCos*kA[0][0]-fSin*kA[1][0];
		fTmp0 = kA[0][1];
		fTmp1 = kA[1][1];
		kA[0][1] = fCos*fTmp0-fSin*fTmp1;
		kA[1][1] = fSin*fTmp0+fCos*fTmp1;
		kA[0][2] = -fSin*kA[1][2];
		kA[1][2] *= fCos;

		size_t iCol;
		for (iCol = 0; iCol < 3; iCol++)
		{
			fTmp0 = kL[iCol][0];
			fTmp1 = kL[iCol][1];
			kL[iCol][0] = fCos*fTmp0-fSin*fTmp1;
			kL[iCol][1] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust right
		fY = kA[0][1];
		fZ = kA[0][2];
		fInvLength = CGUIMath::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[0][1] = fCos*kA[0][1]-fSin*kA[0][2];
		fTmp0 = kA[1][1];
		fTmp1 = kA[1][2];
		kA[1][1] = fCos*fTmp0-fSin*fTmp1;
		kA[1][2] = fSin*fTmp0+fCos*fTmp1;
		kA[2][1] = -fSin*kA[2][2];
		kA[2][2] *= fCos;

		for (iRow = 0; iRow < 3; iRow++)
		{
			fTmp0 = kR[1][iRow];
			fTmp1 = kR[2][iRow];
			kR[1][iRow] = fCos*fTmp0-fSin*fTmp1;
			kR[2][iRow] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust left
		fY = kA[1][1];
		fZ = kA[2][1];
		fInvLength = CGUIMath::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		kA[1][1] = fCos*kA[1][1]-fSin*kA[2][1];
		fTmp0 = kA[1][2];
		fTmp1 = kA[2][2];
		kA[1][2] = fCos*fTmp0-fSin*fTmp1;
		kA[2][2] = fSin*fTmp0+fCos*fTmp1;

		for (iCol = 0; iCol < 3; iCol++)
		{
			fTmp0 = kL[iCol][1];
			fTmp1 = kL[iCol][2];
			kL[iCol][1] = fCos*fTmp0-fSin*fTmp1;
			kL[iCol][2] = fSin*fTmp0+fCos*fTmp1;
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::SingularValueDecomposition (CGUIMatrix3& kL, CGUIVector3& kS,
		CGUIMatrix3& kR) const
	{
		// temas: currently unused
		//const int iMax = 16;
		size_t iRow, iCol;

		CGUIMatrix3 kA = *this;
		Bidiagonalize(kA,kL,kR);

		for (unsigned int i = 0; i < ms_iSvdMaxIterations; i++)
		{
			real fTmp, fTmp0, fTmp1;
			real fSin0, fCos0, fTan0;
			real fSin1, fCos1, fTan1;

			bool bTest1 = (CGUIMath::Abs(kA[0][1]) <=
				ms_fSvdEpsilon*(CGUIMath::Abs(kA[0][0])+CGUIMath::Abs(kA[1][1])));
			bool bTest2 = (CGUIMath::Abs(kA[1][2]) <=
				ms_fSvdEpsilon*(CGUIMath::Abs(kA[1][1])+CGUIMath::Abs(kA[2][2])));
			if ( bTest1 )
			{
				if ( bTest2 )
				{
					kS[0] = kA[0][0];
					kS[1] = kA[1][1];
					kS[2] = kA[2][2];
					break;
				}
				else
				{
					// 2x2 closed form factorization
					fTmp = (kA[1][1]*kA[1][1] - kA[2][2]*kA[2][2] +
						kA[1][2]*kA[1][2])/(kA[1][2]*kA[2][2]);
					fTan0 = 0.5f*(fTmp+CGUIMath::Sqrt(fTmp*fTmp + 4.0f));
					fCos0 = CGUIMath::InvSqrt(1.0f+fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for (iCol = 0; iCol < 3; iCol++)
					{
						fTmp0 = kL[iCol][1];
						fTmp1 = kL[iCol][2];
						kL[iCol][1] = fCos0*fTmp0-fSin0*fTmp1;
						kL[iCol][2] = fSin0*fTmp0+fCos0*fTmp1;
					}

					fTan1 = (kA[1][2]-kA[2][2]*fTan0)/kA[1][1];
					fCos1 = CGUIMath::InvSqrt(1.0f+fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for (iRow = 0; iRow < 3; iRow++)
					{
						fTmp0 = kR[1][iRow];
						fTmp1 = kR[2][iRow];
						kR[1][iRow] = fCos1*fTmp0-fSin1*fTmp1;
						kR[2][iRow] = fSin1*fTmp0+fCos1*fTmp1;
					}

					kS[0] = kA[0][0];
					kS[1] = fCos0*fCos1*kA[1][1] -
						fSin1*(fCos0*kA[1][2]-fSin0*kA[2][2]);
					kS[2] = fSin0*fSin1*kA[1][1] +
						fCos1*(fSin0*kA[1][2]+fCos0*kA[2][2]);
					break;
				}
			}
			else
			{
				if ( bTest2 )
				{
					// 2x2 closed form factorization
					fTmp = (kA[0][0]*kA[0][0] + kA[1][1]*kA[1][1] -
						kA[0][1]*kA[0][1])/(kA[0][1]*kA[1][1]);
					fTan0 = 0.5f*(-fTmp+CGUIMath::Sqrt(fTmp*fTmp + 4.0f));
					fCos0 = CGUIMath::InvSqrt(1.0f+fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for (iCol = 0; iCol < 3; iCol++)
					{
						fTmp0 = kL[iCol][0];
						fTmp1 = kL[iCol][1];
						kL[iCol][0] = fCos0*fTmp0-fSin0*fTmp1;
						kL[iCol][1] = fSin0*fTmp0+fCos0*fTmp1;
					}

					fTan1 = (kA[0][1]-kA[1][1]*fTan0)/kA[0][0];
					fCos1 = CGUIMath::InvSqrt(1.0f+fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for (iRow = 0; iRow < 3; iRow++)
					{
						fTmp0 = kR[0][iRow];
						fTmp1 = kR[1][iRow];
						kR[0][iRow] = fCos1*fTmp0-fSin1*fTmp1;
						kR[1][iRow] = fSin1*fTmp0+fCos1*fTmp1;
					}

					kS[0] = fCos0*fCos1*kA[0][0] -
						fSin1*(fCos0*kA[0][1]-fSin0*kA[1][1]);
					kS[1] = fSin0*fSin1*kA[0][0] +
						fCos1*(fSin0*kA[0][1]+fCos0*kA[1][1]);
					kS[2] = kA[2][2];
					break;
				}
				else
				{
					GolubKahanStep(kA,kL,kR);
				}
			}
		}

		// positize diagonal
		for (iRow = 0; iRow < 3; iRow++)
		{
			if ( kS[iRow] < 0.0 )
			{
				kS[iRow] = -kS[iRow];
				for (iCol = 0; iCol < 3; iCol++)
					kR[iRow][iCol] = -kR[iRow][iCol];
			}
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::SingularValueComposition (const CGUIMatrix3& kL,
		const CGUIVector3& kS, const CGUIMatrix3& kR)
	{
		size_t iRow, iCol;
		CGUIMatrix3 kTmp;

		// product S*R
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
				kTmp[iRow][iCol] = kS[iRow]*kR[iRow][iCol];
		}

		// product L*S*R
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
			{
				m[iRow][iCol] = 0.0;
				for (int iMid = 0; iMid < 3; iMid++)
					m[iRow][iCol] += kL[iRow][iMid]*kTmp[iMid][iCol];
			}
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::Orthonormalize ()
	{
		// Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
		// M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.

		// compute q0
		real fInvLength = CGUIMath::InvSqrt(m[0][0]*m[0][0]
		+ m[1][0]*m[1][0] +
			m[2][0]*m[2][0]);

		m[0][0] *= fInvLength;
		m[1][0] *= fInvLength;
		m[2][0] *= fInvLength;

		// compute q1
		real fDot0 =
			m[0][0]*m[0][1] +
			m[1][0]*m[1][1] +
			m[2][0]*m[2][1];

		m[0][1] -= fDot0*m[0][0];
		m[1][1] -= fDot0*m[1][0];
		m[2][1] -= fDot0*m[2][0];

		fInvLength = CGUIMath::InvSqrt(m[0][1]*m[0][1] +
			m[1][1]*m[1][1] +
			m[2][1]*m[2][1]);

		m[0][1] *= fInvLength;
		m[1][1] *= fInvLength;
		m[2][1] *= fInvLength;

		// compute q2
		real fDot1 =
			m[0][1]*m[0][2] +
			m[1][1]*m[1][2] +
			m[2][1]*m[2][2];

		fDot0 =
			m[0][0]*m[0][2] +
			m[1][0]*m[1][2] +
			m[2][0]*m[2][2];

		m[0][2] -= fDot0*m[0][0] + fDot1*m[0][1];
		m[1][2] -= fDot0*m[1][0] + fDot1*m[1][1];
		m[2][2] -= fDot0*m[2][0] + fDot1*m[2][1];

		fInvLength = CGUIMath::InvSqrt(m[0][2]*m[0][2] +
			m[1][2]*m[1][2] +
			m[2][2]*m[2][2]);

		m[0][2] *= fInvLength;
		m[1][2] *= fInvLength;
		m[2][2] *= fInvLength;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::QDUDecomposition (CGUIMatrix3& kQ,
		CGUIVector3& kD, CGUIVector3& kU) const
	{
		// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
		// and U is upper triangular with ones on its diagonal.  Algorithm uses
		// Gram-Schmidt orthogonalization (the QR algorithm).
		//
		// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.  The matrix R has entries
		//
		//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
		//   r10 = 0      r11 = q1*m1  r12 = q1*m2
		//   r20 = 0      r21 = 0      r22 = q2*m2
		//
		// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
		// u02 = r02/r00, and u12 = r12/r11.

		// Q = rotation
		// D = scaling
		// U = shear

		// D stores the three diagonal entries r00, r11, r22
		// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

		// build orthogonal matrix Q
		real fInvLength = CGUIMath::InvSqrt(m[0][0]*m[0][0]
		+ m[1][0]*m[1][0] +
			m[2][0]*m[2][0]);
		kQ[0][0] = m[0][0]*fInvLength;
		kQ[1][0] = m[1][0]*fInvLength;
		kQ[2][0] = m[2][0]*fInvLength;

		real fDot = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
			kQ[2][0]*m[2][1];
		kQ[0][1] = m[0][1]-fDot*kQ[0][0];
		kQ[1][1] = m[1][1]-fDot*kQ[1][0];
		kQ[2][1] = m[2][1]-fDot*kQ[2][0];
		fInvLength = CGUIMath::InvSqrt(kQ[0][1]*kQ[0][1] + kQ[1][1]*kQ[1][1] +
			kQ[2][1]*kQ[2][1]);
		kQ[0][1] *= fInvLength;
		kQ[1][1] *= fInvLength;
		kQ[2][1] *= fInvLength;

		fDot = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
			kQ[2][0]*m[2][2];
		kQ[0][2] = m[0][2]-fDot*kQ[0][0];
		kQ[1][2] = m[1][2]-fDot*kQ[1][0];
		kQ[2][2] = m[2][2]-fDot*kQ[2][0];
		fDot = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
			kQ[2][1]*m[2][2];
		kQ[0][2] -= fDot*kQ[0][1];
		kQ[1][2] -= fDot*kQ[1][1];
		kQ[2][2] -= fDot*kQ[2][1];
		fInvLength = CGUIMath::InvSqrt(kQ[0][2]*kQ[0][2] + kQ[1][2]*kQ[1][2] +
			kQ[2][2]*kQ[2][2]);
		kQ[0][2] *= fInvLength;
		kQ[1][2] *= fInvLength;
		kQ[2][2] *= fInvLength;

		// guarantee that orthogonal matrix has determinant 1 (no reflections)
		real fDet = kQ[0][0]*kQ[1][1]*kQ[2][2] + kQ[0][1]*kQ[1][2]*kQ[2][0] +
			kQ[0][2]*kQ[1][0]*kQ[2][1] - kQ[0][2]*kQ[1][1]*kQ[2][0] -
			kQ[0][1]*kQ[1][0]*kQ[2][2] - kQ[0][0]*kQ[1][2]*kQ[2][1];

		if ( fDet < 0.0 )
		{
			for (size_t iRow = 0; iRow < 3; iRow++)
				for (size_t iCol = 0; iCol < 3; iCol++)
					kQ[iRow][iCol] = -kQ[iRow][iCol];
		}

		// build "right" matrix R
		CGUIMatrix3 kR;
		kR[0][0] = kQ[0][0]*m[0][0] + kQ[1][0]*m[1][0] +
			kQ[2][0]*m[2][0];
		kR[0][1] = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
			kQ[2][0]*m[2][1];
		kR[1][1] = kQ[0][1]*m[0][1] + kQ[1][1]*m[1][1] +
			kQ[2][1]*m[2][1];
		kR[0][2] = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
			kQ[2][0]*m[2][2];
		kR[1][2] = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
			kQ[2][1]*m[2][2];
		kR[2][2] = kQ[0][2]*m[0][2] + kQ[1][2]*m[1][2] +
			kQ[2][2]*m[2][2];

		// the scaling component
		kD[0] = kR[0][0];
		kD[1] = kR[1][1];
		kD[2] = kR[2][2];

		// the shear component
		real fInvD0 = 1.0f/kD[0];
		kU[0] = kR[0][1]*fInvD0;
		kU[1] = kR[0][2]*fInvD0;
		kU[2] = kR[1][2]/kD[1];
	}
	//-----------------------------------------------------------------------
	real CGUIMatrix3::MaxCubicRoot (real afCoeff[3])
	{
		// Spectral norm is for A^T*A, so characteristic polynomial
		// P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 has three positive real roots.
		// This yields the assertions c[0] < 0 and c[2]*c[2] >= 3*c[1].

		// quick out for uniform scale (triple root)
		const real fOneThird = 1.0f/3.0f;
		const real fEpsilon = 1e-06f;
		real fDiscr = afCoeff[2]*afCoeff[2] - 3.0f*afCoeff[1];
		if ( fDiscr <= fEpsilon )
			return -fOneThird*afCoeff[2];

		// Compute an upper bound on roots of P(x).  This assumes that A^T*A
		// has been scaled by its largest entry.
		real fX = 1.0f;
		real fPoly = afCoeff[0]+fX*(afCoeff[1]+fX*(afCoeff[2]+fX));
		if ( fPoly < 0.0f )
		{
			// uses a matrix norm to find an upper bound on maximum root
			fX = CGUIMath::Abs(afCoeff[0]);
			real fTmp = 1.0f+CGUIMath::Abs(afCoeff[1]);
			if ( fTmp > fX )
				fX = fTmp;
			fTmp = 1.0f+CGUIMath::Abs(afCoeff[2]);
			if ( fTmp > fX )
				fX = fTmp;
		}

		// Newton's method to find root
		real fTwoC2 = 2.0f*afCoeff[2];
		for (int i = 0; i < 16; i++)
		{
			fPoly = afCoeff[0]+fX*(afCoeff[1]+fX*(afCoeff[2]+fX));
			if ( CGUIMath::Abs(fPoly) <= fEpsilon )
				return fX;

			real fDeriv = afCoeff[1]+fX*(fTwoC2+3.0f*fX);
			fX -= fPoly/fDeriv;
		}

		return fX;
	}
	//-----------------------------------------------------------------------
	real CGUIMatrix3::SpectralNorm () const
	{
		CGUIMatrix3 kP;
		size_t iRow, iCol;
		real fPmax = 0.0f;
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
			{
				kP[iRow][iCol] = 0.0f;
				for (int iMid = 0; iMid < 3; iMid++)
				{
					kP[iRow][iCol] +=
						m[iMid][iRow]*m[iMid][iCol];
				}
				if ( kP[iRow][iCol] > fPmax )
					fPmax = kP[iRow][iCol];
			}
		}

		real fInvPmax = 1.0f/fPmax;
		for (iRow = 0; iRow < 3; iRow++)
		{
			for (iCol = 0; iCol < 3; iCol++)
				kP[iRow][iCol] *= fInvPmax;
		}

		real afCoeff[3];
		afCoeff[0] = -(kP[0][0]*(kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1]) +
			kP[0][1]*(kP[2][0]*kP[1][2]-kP[1][0]*kP[2][2]) +
			kP[0][2]*(kP[1][0]*kP[2][1]-kP[2][0]*kP[1][1]));
		afCoeff[1] = kP[0][0]*kP[1][1]-kP[0][1]*kP[1][0] +
			kP[0][0]*kP[2][2]-kP[0][2]*kP[2][0] +
			kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1];
		afCoeff[2] = -(kP[0][0]+kP[1][1]+kP[2][2]);

		real fRoot = MaxCubicRoot(afCoeff);
		real fNorm = CGUIMath::Sqrt(fPmax*fRoot);
		return fNorm;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::ToAxisAngle (CGUIVector3& rkAxis, CGUIRadian& rfRadians) const
	{
		// Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
		// The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
		// I is the identity and
		//
		//       +-        -+
		//   P = |  0 -z +y |
		//       | +z  0 -x |
		//       | -y +x  0 |
		//       +-        -+
		//
		// If A > 0, R represents a counterclockwise rotation about the axis in
		// the sense of looking from the tip of the axis vector towards the
		// origin.  Some algebra will show that
		//
		//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
		//
		// In the event that A = pi, R-R^t = 0 which prevents us from extracting
		// the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
		// P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
		// z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
		// it does not matter which sign you choose on the square roots.

		real fTrace = m[0][0] + m[1][1] + m[2][2];
		real fCos = 0.5f*(fTrace-1.0f);
		rfRadians = CGUIMath::ACos(fCos);  // in [0,GUI_PI]

		if ( rfRadians > CGUIRadian(0.0) )
		{
			if ( rfRadians < CGUIRadian(CGUIMath::GUI_PI) )
			{
				rkAxis.x = m[2][1]-m[1][2];
				rkAxis.y = m[0][2]-m[2][0];
				rkAxis.z = m[1][0]-m[0][1];
				rkAxis.Normalise();
			}
			else
			{
				// angle is GUI_PI
				real fHalfInverse;
				if ( m[0][0] >= m[1][1] )
				{
					// r00 >= r11
					if ( m[0][0] >= m[2][2] )
					{
						// r00 is maximum diagonal term
						rkAxis.x = 0.5f*CGUIMath::Sqrt(m[0][0] -m[1][1] - m[2][2] + 1.0f);
						fHalfInverse = 0.5f/rkAxis.x;
						rkAxis.y = fHalfInverse*m[0][1];
						rkAxis.z = fHalfInverse*m[0][2];
					}
					else
					{
						// r22 is maximum diagonal term
						rkAxis.z = 0.5f*CGUIMath::Sqrt(m[2][2] -m[0][0] - m[1][1] + 1.0f);
						fHalfInverse = 0.5f/rkAxis.z;
						rkAxis.x = fHalfInverse*m[0][2];
						rkAxis.y = fHalfInverse*m[1][2];
					}
				}
				else
				{
					// r11 > r00
					if ( m[1][1] >= m[2][2] )
					{
						// r11 is maximum diagonal term
						rkAxis.y = 0.5f*CGUIMath::Sqrt(m[1][1] -m[0][0] - m[2][2] + 1.0f);
						fHalfInverse  = 0.5f/rkAxis.y;
						rkAxis.x = fHalfInverse*m[0][1];
						rkAxis.z = fHalfInverse*m[1][2];
					}
					else
					{
						// r22 is maximum diagonal term
						rkAxis.z = 0.5f*CGUIMath::Sqrt(m[2][2] -m[0][0] - m[1][1] + 1.0f);
						fHalfInverse = 0.5f/rkAxis.z;
						rkAxis.x = fHalfInverse*m[0][2];
						rkAxis.y = fHalfInverse*m[1][2];
					}
				}
			}
		}
		else
		{
			// The angle is 0 and the matrix is the identity.  Any axis will
			// work, so just use the x-axis.
			rkAxis.x = 1.0f;
			rkAxis.y = 0.0f;
			rkAxis.z = 0.0f;
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromAxisAngle (const CGUIVector3& rkAxis, const CGUIRadian& fRadians)
	{
		real fCos = CGUIMath::Cos(fRadians);
		real fSin = CGUIMath::Sin(fRadians);
		real fOneMinusCos = 1.0f-fCos;
		real fX2 = rkAxis.x*rkAxis.x;
		real fY2 = rkAxis.y*rkAxis.y;
		real fZ2 = rkAxis.z*rkAxis.z;
		real fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
		real fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
		real fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
		real fXSin = rkAxis.x*fSin;
		real fYSin = rkAxis.y*fSin;
		real fZSin = rkAxis.z*fSin;

		m[0][0] = fX2*fOneMinusCos+fCos;
		m[0][1] = fXYM-fZSin;
		m[0][2] = fXZM+fYSin;
		m[1][0] = fXYM+fZSin;
		m[1][1] = fY2*fOneMinusCos+fCos;
		m[1][2] = fYZM-fXSin;
		m[2][0] = fXZM-fYSin;
		m[2][1] = fYZM+fXSin;
		m[2][2] = fZ2*fOneMinusCos+fCos;
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesXYZ (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz          -cy*sz           sy
		//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
		//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

		rfPAngle = CGUIRadian(CGUIMath::ASin(m[0][2]));
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(-m[1][2],m[2][2]);
				rfRAngle = CGUIMath::ATan2(-m[0][1],m[0][0]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(m[1][0],m[1][1]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(m[1][0],m[1][1]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesXZY (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz          -sz              cz*sy
		//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
		//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

		rfPAngle = CGUIMath::ASin(-m[0][1]);
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(m[2][1],m[1][1]);
				rfRAngle = CGUIMath::ATan2(m[0][2],m[0][0]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(-m[2][0],m[2][2]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(-m[2][0],m[2][2]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesYXZ (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
		//        cx*sz           cx*cz          -sx
		//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

		rfPAngle = CGUIMath::ASin(-m[1][2]);
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(m[0][2],m[2][2]);
				rfRAngle = CGUIMath::ATan2(m[1][0],m[1][1]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(-m[0][1],m[0][0]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(-m[0][1],m[0][0]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesYZX (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
		//        sz              cx*cz          -cz*sx
		//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

		rfPAngle = CGUIMath::ASin(m[1][0]);
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(-m[2][0],m[0][0]);
				rfRAngle = CGUIMath::ATan2(-m[1][2],m[1][1]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(m[2][1],m[2][2]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(m[2][1],m[2][2]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesZXY (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
		//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
		//       -cx*sy           sx              cx*cy

		rfPAngle = CGUIMath::ASin(m[2][1]);
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(-m[0][1],m[1][1]);
				rfRAngle = CGUIMath::ATan2(-m[2][0],m[2][2]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(m[0][2],m[0][0]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(m[0][2],m[0][0]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::ToEulerAnglesZYX (CGUIRadian& rfYAngle, CGUIRadian& rfPAngle,
		CGUIRadian& rfRAngle) const
	{
		// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
		//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
		//       -sy              cy*sx           cx*cy

		rfPAngle = CGUIMath::ASin(-m[2][0]);
		if ( rfPAngle < CGUIRadian(CGUIMath::GUI_HALF_PI) )
		{
			if ( rfPAngle > CGUIRadian(-CGUIMath::GUI_HALF_PI) )
			{
				rfYAngle = CGUIMath::ATan2(m[1][0],m[0][0]);
				rfRAngle = CGUIMath::ATan2(m[2][1],m[2][2]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				CGUIRadian fRmY = CGUIMath::ATan2(-m[0][1],m[0][2]);
				rfRAngle = CGUIRadian(0.0);  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			CGUIRadian fRpY = CGUIMath::ATan2(-m[0][1],m[0][2]);
			rfRAngle = CGUIRadian(0.0);  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return false;
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesXYZ (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		*this = kXMat*(kYMat*kZMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesXZY (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		*this = kXMat*(kZMat*kYMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesYXZ (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		*this = kYMat*(kXMat*kZMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesYZX (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		*this = kYMat*(kZMat*kXMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesZXY (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		*this = kZMat*(kXMat*kYMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::FromEulerAnglesZYX (const CGUIRadian& fYAngle, const CGUIRadian& fPAngle,
		const CGUIRadian& fRAngle)
	{
		real fCos, fSin;

		fCos = CGUIMath::Cos(fYAngle);
		fSin = CGUIMath::Sin(fYAngle);
		CGUIMatrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = CGUIMath::Cos(fPAngle);
		fSin = CGUIMath::Sin(fPAngle);
		CGUIMatrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = CGUIMath::Cos(fRAngle);
		fSin = CGUIMath::Sin(fRAngle);
		CGUIMatrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		*this = kZMat*(kYMat*kXMat);
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::Tridiagonal (real afDiag[3], real afSubDiag[3])
	{
		// Householder reduction T = Q^t M Q
		//   Input:
		//     mat, symmetric 3x3 matrix M
		//   Output:
		//     mat, orthogonal matrix Q
		//     diag, diagonal entries of T
		//     subd, subdiagonal entries of T (T is symmetric)

		real fA = m[0][0];
		real fB = m[0][1];
		real fC = m[0][2];
		real fD = m[1][1];
		real fE = m[1][2];
		real fF = m[2][2];

		afDiag[0] = fA;
		afSubDiag[2] = 0.0;
		if ( CGUIMath::Abs(fC) >= EPSILON )
		{
			real fLength = CGUIMath::Sqrt(fB*fB+fC*fC);
			real fInvLength = 1.0f/fLength;
			fB *= fInvLength;
			fC *= fInvLength;
			real fQ = 2.0f*fB*fE+fC*(fF-fD);
			afDiag[1] = fD+fC*fQ;
			afDiag[2] = fF-fC*fQ;
			afSubDiag[0] = fLength;
			afSubDiag[1] = fE-fB*fQ;
			m[0][0] = 1.0f;
			m[0][1] = 0.0f;
			m[0][2] = 0.0f;
			m[1][0] = 0.0f;
			m[1][1] = fB;
			m[1][2] = fC;
			m[2][0] = 0.0f;
			m[2][1] = fC;
			m[2][2] = -fB;
		}
		else
		{
			afDiag[1] = fD;
			afDiag[2] = fF;
			afSubDiag[0] = fB;
			afSubDiag[1] = fE;
			m[0][0] = 1.0;
			m[0][1] = 0.0;
			m[0][2] = 0.0;
			m[1][0] = 0.0;
			m[1][1] = 1.0;
			m[1][2] = 0.0;
			m[2][0] = 0.0;
			m[2][1] = 0.0;
			m[2][2] = 1.0;
		}
	}
	//-----------------------------------------------------------------------
	bool CGUIMatrix3::QLAlgorithm (real afDiag[3], real afSubDiag[3])
	{
		// QL iteration with implicit shifting to reduce matrix from tridiagonal
		// to diagonal

		for (int i0 = 0; i0 < 3; i0++)
		{
			const unsigned int iMaxIter = 32;
			unsigned int iIter;
			for (iIter = 0; iIter < iMaxIter; iIter++)
			{
				int i1;
				for (i1 = i0; i1 <= 1; i1++)
				{
					real fSum = CGUIMath::Abs(afDiag[i1]) +
						CGUIMath::Abs(afDiag[i1+1]);
					if ( CGUIMath::Abs(afSubDiag[i1]) + fSum == fSum )
						break;
				}
				if ( i1 == i0 )
					break;

				real fTmp0 = (afDiag[i0+1]-afDiag[i0])/(2.0f*afSubDiag[i0]);
				real fTmp1 = CGUIMath::Sqrt(fTmp0*fTmp0+1.0f);
				if ( fTmp0 < 0.0f )
					fTmp0 = afDiag[i1]-afDiag[i0]+afSubDiag[i0]/(fTmp0-fTmp1);
				else
					fTmp0 = afDiag[i1]-afDiag[i0]+afSubDiag[i0]/(fTmp0+fTmp1);
				real fSin = 1.0f;
				real fCos = 1.0f;
				real fTmp2 = 0.0f;
				for (int i2 = i1-1; i2 >= i0; i2--)
				{
					real fTmp3 = fSin*afSubDiag[i2];
					real fTmp4 = fCos*afSubDiag[i2];
					if ( CGUIMath::Abs(fTmp3) >= CGUIMath::Abs(fTmp0) )
					{
						fCos = fTmp0/fTmp3;
						fTmp1 = CGUIMath::Sqrt(fCos*fCos+1.0f);
						afSubDiag[i2+1] = fTmp3*fTmp1;
						fSin = 1.0f/fTmp1;
						fCos *= fSin;
					}
					else
					{
						fSin = fTmp3/fTmp0;
						fTmp1 = CGUIMath::Sqrt(fSin*fSin+1.0f);
						afSubDiag[i2+1] = fTmp0*fTmp1;
						fCos = 1.0f/fTmp1;
						fSin *= fCos;
					}
					fTmp0 = afDiag[i2+1]-fTmp2;
					fTmp1 = (afDiag[i2]-fTmp0)*fSin+2.0f*fTmp4*fCos;
					fTmp2 = fSin*fTmp1;
					afDiag[i2+1] = fTmp0+fTmp2;
					fTmp0 = fCos*fTmp1-fTmp4;

					for (int iRow = 0; iRow < 3; iRow++)
					{
						fTmp3 = m[iRow][i2+1];
						m[iRow][i2+1] = fSin*m[iRow][i2] +
							fCos*fTmp3;
						m[iRow][i2] = fCos*m[iRow][i2] -
							fSin*fTmp3;
					}
				}
				afDiag[i0] -= fTmp2;
				afSubDiag[i0] = fTmp0;
				afSubDiag[i1] = 0.0f;
			}

			if ( iIter == iMaxIter )
			{
				// should not get here under normal circumstances
				return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::EigenSolveSymmetric (real afEigenvalue[3],
		CGUIVector3 akEigenvector[3]) const
	{
		CGUIMatrix3 kMatrix = *this;
		real afSubDiag[3];
		kMatrix.Tridiagonal(afEigenvalue,afSubDiag);
		kMatrix.QLAlgorithm(afEigenvalue,afSubDiag);

		for (size_t i = 0; i < 3; i++)
		{
			akEigenvector[i][0] = kMatrix[0][i];
			akEigenvector[i][1] = kMatrix[1][i];
			akEigenvector[i][2] = kMatrix[2][i];
		}

		// make eigenvectors form a right--handed system
		CGUIVector3 kCross = akEigenvector[1].CrossProduct(akEigenvector[2]);
		real fDet = akEigenvector[0].DotProduct(kCross);
		if ( fDet < 0.0 )
		{
			akEigenvector[2][0] = - akEigenvector[2][0];
			akEigenvector[2][1] = - akEigenvector[2][1];
			akEigenvector[2][2] = - akEigenvector[2][2];
		}
	}
	//-----------------------------------------------------------------------
	void CGUIMatrix3::TensorProduct (const CGUIVector3& rkU, const CGUIVector3& rkV,
		CGUIMatrix3& rkProduct)
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				rkProduct[iRow][iCol] = rkU[iRow]*rkV[iCol];
		}
	}
	//-----------------------------------------------------------------------
}
