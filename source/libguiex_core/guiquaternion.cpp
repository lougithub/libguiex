/** 
* @file guiquaternion.cpp
* @brief quaternion
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiquaternion.h>
#include <libguiex_core/guimath.h>
#include <libguiex_core/guimatrix3.h>
#include <libguiex_core/guivector3.h>



namespace guiex 
{

	const real CGUIQuaternion::ms_fEpsilon = 1e-03f;
	const CGUIQuaternion CGUIQuaternion::ZERO(0.0,0.0,0.0,0.0);
	const CGUIQuaternion CGUIQuaternion::IDENTITY(1.0,0.0,0.0,0.0);

	//-----------------------------------------------------------------------
	void CGUIQuaternion::FromRotationMatrix (const CGUIMatrix3& kRot)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "CGUIQuaternion Calculus and Fast Animation".

		real fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
		real fRoot;

		if ( fTrace > 0.0 )
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = CGUIMath::Sqrt(fTrace + 1.0f);  // 2w
			w = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)
			x = (kRot[2][1]-kRot[1][2])*fRoot;
			y = (kRot[0][2]-kRot[2][0])*fRoot;
			z = (kRot[1][0]-kRot[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if ( kRot[1][1] > kRot[0][0] )
				i = 1;
			if ( kRot[2][2] > kRot[i][i] )
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = CGUIMath::Sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
			real* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			w = (kRot[k][j]-kRot[j][k])*fRoot;
			*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
			*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
		}
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::ToRotationMatrix (CGUIMatrix3& kRot) const
	{
		real fTx  = 2.0f*x;
		real fTy  = 2.0f*y;
		real fTz  = 2.0f*z;
		real fTwx = fTx*w;
		real fTwy = fTy*w;
		real fTwz = fTz*w;
		real fTxx = fTx*x;
		real fTxy = fTy*x;
		real fTxz = fTz*x;
		real fTyy = fTy*y;
		real fTyz = fTz*y;
		real fTzz = fTz*z;

		kRot[0][0] = 1.0f-(fTyy+fTzz);
		kRot[0][1] = fTxy-fTwz;
		kRot[0][2] = fTxz+fTwy;
		kRot[1][0] = fTxy+fTwz;
		kRot[1][1] = 1.0f-(fTxx+fTzz);
		kRot[1][2] = fTyz-fTwx;
		kRot[2][0] = fTxz-fTwy;
		kRot[2][1] = fTyz+fTwx;
		kRot[2][2] = 1.0f-(fTxx+fTyy);
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::FromAngleAxis (const CGUIRadian& rfAngle,const CGUIVector3& rkAxis)
	{
		// assert:  axis[] is unit length
		//
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		CGUIRadian fHalfAngle ( 0.5f*rfAngle );
		real fSin = CGUIMath::Sin(fHalfAngle);
		w = CGUIMath::Cos(fHalfAngle);
		x = fSin*rkAxis.x;
		y = fSin*rkAxis.y;
		z = fSin*rkAxis.z;
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::ToAngleAxis (CGUIRadian& rfAngle, CGUIVector3& rkAxis) const
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		real fSqrLength = x*x+y*y+z*z;
		if ( fSqrLength > 0.0 )
		{
			rfAngle = 2.0f*CGUIMath::ACos(w);
			real fInvLength = CGUIMath::InvSqrt(fSqrLength);
			rkAxis.x = x*fInvLength;
			rkAxis.y = y*fInvLength;
			rkAxis.z = z*fInvLength;
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			rfAngle = CGUIRadian(0.0);
			rkAxis.x = 1.0;
			rkAxis.y = 0.0;
			rkAxis.z = 0.0;
		}
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::FromAxes (const CGUIVector3* akAxis)
	{
		CGUIMatrix3 kRot;

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			kRot[0][iCol] = akAxis[iCol].x;
			kRot[1][iCol] = akAxis[iCol].y;
			kRot[2][iCol] = akAxis[iCol].z;
		}

		FromRotationMatrix(kRot);
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::FromAxes (const CGUIVector3& xaxis, const CGUIVector3& yaxis, const CGUIVector3& zaxis)
	{
		CGUIMatrix3 kRot;

		kRot[0][0] = xaxis.x;
		kRot[1][0] = xaxis.y;
		kRot[2][0] = xaxis.z;

		kRot[0][1] = yaxis.x;
		kRot[1][1] = yaxis.y;
		kRot[2][1] = yaxis.z;

		kRot[0][2] = zaxis.x;
		kRot[1][2] = zaxis.y;
		kRot[2][2] = zaxis.z;

		FromRotationMatrix(kRot);

	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::ToAxes (CGUIVector3* akAxis) const
	{
		CGUIMatrix3 kRot;

		ToRotationMatrix(kRot);

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			akAxis[iCol].x = kRot[0][iCol];
			akAxis[iCol].y = kRot[1][iCol];
			akAxis[iCol].z = kRot[2][iCol];
		}
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIQuaternion::xAxis(void) const
	{
		real fTx  = 2.0f*x;
		real fTy  = 2.0f*y;
		real fTz  = 2.0f*z;
		real fTwy = fTy*w;
		real fTwz = fTz*w;
		real fTxy = fTy*x;
		real fTxz = fTz*x;
		real fTyy = fTy*y;
		real fTzz = fTz*z;

		return CGUIVector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIQuaternion::yAxis(void) const
	{
		real fTx  = 2.0f*x;
		real fTy  = 2.0f*y;
		real fTz  = 2.0f*z;
		real fTwx = fTx*w;
		real fTwz = fTz*w;
		real fTxx = fTx*x;
		real fTxy = fTy*x;
		real fTyz = fTz*y;
		real fTzz = fTz*z;

		return CGUIVector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIQuaternion::zAxis(void) const
	{
		real fTx  = 2.0f*x;
		real fTy  = 2.0f*y;
		real fTz  = 2.0f*z;
		real fTwx = fTx*w;
		real fTwy = fTy*w;
		real fTxx = fTx*x;
		real fTxz = fTz*x;
		real fTyy = fTy*y;
		real fTyz = fTz*y;

		return CGUIVector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::ToAxes (CGUIVector3& xaxis, CGUIVector3& yaxis, CGUIVector3& zaxis) const
	{
		CGUIMatrix3 kRot;

		ToRotationMatrix(kRot);

		xaxis.x = kRot[0][0];
		xaxis.y = kRot[1][0];
		xaxis.z = kRot[2][0];

		yaxis.x = kRot[0][1];
		yaxis.y = kRot[1][1];
		yaxis.z = kRot[2][1];

		zaxis.x = kRot[0][2];
		zaxis.y = kRot[1][2];
		zaxis.z = kRot[2][2];
	}

	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::operator+ (const CGUIQuaternion& rkQ) const
	{
		return CGUIQuaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::operator- (const CGUIQuaternion& rkQ) const
	{
		return CGUIQuaternion(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::operator* (const CGUIQuaternion& rkQ) const
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return CGUIQuaternion
			(
			w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
			w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
			w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
			w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
			);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::operator* (real fScalar) const
	{
		return CGUIQuaternion(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion operator* (real fScalar, const CGUIQuaternion& rkQ)
	{
		return CGUIQuaternion(fScalar*rkQ.w,fScalar*rkQ.x,fScalar*rkQ.y,
			fScalar*rkQ.z);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::operator- () const
	{
		return CGUIQuaternion(-w,-x,-y,-z);
	}
	//-----------------------------------------------------------------------
	real CGUIQuaternion::Dot (const CGUIQuaternion& rkQ) const
	{
		return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
	}
	//-----------------------------------------------------------------------
	real CGUIQuaternion::Norm () const
	{
		return w*w+x*x+y*y+z*z;
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::Inverse () const
	{
		real fNorm = w*w+x*x+y*y+z*z;
		if ( fNorm > 0.0 )
		{
			real fInvNorm = 1.0f/fNorm;
			return CGUIQuaternion(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
		}
		else
		{
			// return an invalid result to flag the error
			return ZERO;
		}
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::UnitInverse () const
	{
		// assert:  'this' is unit length
		return CGUIQuaternion(w,-x,-y,-z);
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::Exp () const
	{
		// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
		// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

		CGUIRadian fAngle ( CGUIMath::Sqrt(x*x+y*y+z*z) );
		real fSin = CGUIMath::Sin(fAngle);

		CGUIQuaternion kResult;
		kResult.w = CGUIMath::Cos(fAngle);

		if ( CGUIMath::Abs(fSin) >= ms_fEpsilon )
		{
			real fCoeff = fSin/(fAngle.valueRadians());
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
		}
		else
		{
			kResult.x = x;
			kResult.y = y;
			kResult.z = z;
		}

		return kResult;
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::Log () const
	{
		// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
		// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

		CGUIQuaternion kResult;
		kResult.w = 0.0;

		if ( CGUIMath::Abs(w) < 1.0 )
		{
			CGUIRadian fAngle ( CGUIMath::ACos(w) );
			real fSin = CGUIMath::Sin(fAngle);
			if ( CGUIMath::Abs(fSin) >= ms_fEpsilon )
			{
				real fCoeff = fAngle.valueRadians()/fSin;
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
				return kResult;
			}
		}

		kResult.x = x;
		kResult.y = y;
		kResult.z = z;

		return kResult;
	}
	//-----------------------------------------------------------------------
	CGUIVector3 CGUIQuaternion::operator* (const CGUIVector3& v) const
	{
		// nVidia SDK implementation
		CGUIVector3 uv, uuv; 
		CGUIVector3 qvec(x, y, z);
		uv = qvec.CrossProduct(v); 
		uuv = qvec.CrossProduct(uv); 
		uv *= (2.0f * w); 
		uuv *= 2.0f; 

		return v + uv + uuv;

	}
	//-----------------------------------------------------------------------
	bool CGUIQuaternion::equals(const CGUIQuaternion& rhs, const CGUIRadian& tolerance) const
	{
		real fCos = Dot(rhs);
		CGUIRadian angle = CGUIMath::ACos(fCos);

		return (CGUIMath::Abs(angle.valueRadians()) <= tolerance.valueRadians())
			|| CGUIMath::RealEqual(angle.valueRadians(), CGUIMath::GUI_PI, tolerance.valueRadians());


	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::Slerp (real fT, const CGUIQuaternion& rkP,
		const CGUIQuaternion& rkQ, bool shortestPath)
	{
		real fCos = rkP.Dot(rkQ);
		CGUIRadian fAngle ( CGUIMath::ACos(fCos) );

		if ( CGUIMath::Abs(fAngle.valueRadians()) < ms_fEpsilon )
			return rkP;

		real fSin = CGUIMath::Sin(fAngle);
		real fInvSin = 1.0f/fSin;
		real fCoeff0 = CGUIMath::Sin((1.0f-fT)*fAngle)*fInvSin;
		real fCoeff1 = CGUIMath::Sin(fT*fAngle)*fInvSin;
		// Do we need to invert rotation?
		if (fCos < 0.0f && shortestPath)
		{
			fCoeff0 = -fCoeff0;
			// taking the complement requires renormalisation
			CGUIQuaternion t(fCoeff0*rkP + fCoeff1*rkQ);
			t.Normalise();
			return t;
		}
		else
		{
			return fCoeff0*rkP + fCoeff1*rkQ;
		}
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::SlerpExtraSpins (real fT,
		const CGUIQuaternion& rkP, const CGUIQuaternion& rkQ, int iExtraSpins)
	{
		real fCos = rkP.Dot(rkQ);
		CGUIRadian fAngle ( CGUIMath::ACos(fCos) );

		if ( CGUIMath::Abs(fAngle.valueRadians()) < ms_fEpsilon )
			return rkP;

		real fSin = CGUIMath::Sin(fAngle);
		CGUIRadian fPhase ( CGUIMath::GUI_PI*iExtraSpins*fT );
		real fInvSin = 1.0f/fSin;
		real fCoeff0 = CGUIMath::Sin((1.0f-fT)*fAngle - fPhase)*fInvSin;
		real fCoeff1 = CGUIMath::Sin(fT*fAngle + fPhase)*fInvSin;
		return fCoeff0*rkP + fCoeff1*rkQ;
	}
	//-----------------------------------------------------------------------
	void CGUIQuaternion::Intermediate (const CGUIQuaternion& rkQ0,
		const CGUIQuaternion& rkQ1, const CGUIQuaternion& rkQ2,
		CGUIQuaternion& rkA, CGUIQuaternion& rkB)
	{
		// assert:  q0, q1, q2 are unit quaternions

		CGUIQuaternion kQ0inv = rkQ0.UnitInverse();
		CGUIQuaternion kQ1inv = rkQ1.UnitInverse();
		CGUIQuaternion rkP0 = kQ0inv*rkQ1;
		CGUIQuaternion rkP1 = kQ1inv*rkQ2;
		CGUIQuaternion kArg = 0.25*(rkP0.Log()-rkP1.Log());
		CGUIQuaternion kMinusArg = -kArg;

		rkA = rkQ1*kArg.Exp();
		rkB = rkQ1*kMinusArg.Exp();
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::Squad (real fT,
		const CGUIQuaternion& rkP, const CGUIQuaternion& rkA,
		const CGUIQuaternion& rkB, const CGUIQuaternion& rkQ, bool shortestPath)
	{
		real fSlerpT = 2.0f*fT*(1.0f-fT);
		CGUIQuaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
		CGUIQuaternion kSlerpQ = Slerp(fT, rkA, rkB);
		return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
	}
	//-----------------------------------------------------------------------
	real CGUIQuaternion::Normalise(void)
	{
		real len = Norm();
		real factor = 1.0f / CGUIMath::Sqrt(len);
		*this = *this * factor;
		return len;
	}
	//-----------------------------------------------------------------------
	CGUIRadian CGUIQuaternion::getRoll(void) const
	{
		return CGUIRadian(CGUIMath::ATan2(2*(x*y + w*z), w*w + x*x - y*y - z*z));
	}
	//-----------------------------------------------------------------------
	CGUIRadian CGUIQuaternion::getPitch(void) const
	{
		return CGUIRadian(CGUIMath::ATan2(2*(y*z + w*x), w*w - x*x - y*y + z*z));
	}
	//-----------------------------------------------------------------------
	CGUIRadian CGUIQuaternion::getYaw(void) const
	{
		return CGUIRadian(CGUIMath::ASin(-2*(x*z - w*y)));
	}
	//-----------------------------------------------------------------------
	CGUIQuaternion CGUIQuaternion::nlerp(real fT, const CGUIQuaternion& rkP, 
		const CGUIQuaternion& rkQ, bool shortestPath)
	{
		CGUIQuaternion result;
		real fCos = rkP.Dot(rkQ);
		if (fCos < 0.0f && shortestPath)
		{
			result = rkP + fT * ((-rkQ) - rkP);
		}
		else
		{
			result = rkP + fT * (rkQ - rkP);
		}
		result.Normalise();
		return result;
	}
}
