
namespace guiex
{

	class CGUIVector3
	{
	public:
		CGUIVector3();
		CGUIVector3( real fX, real fY, real fZ );
		CGUIVector3( const CGUIVector3& rkVector );

		real Length () const;
		real SquaredLength () const;
		bool IsZeroLength(void) const;

		real DotProduct(const CGUIVector3& vec) const;
		CGUIVector3 CrossProduct( const CGUIVector3& rkVector ) const;

		real Normalise();
		CGUIVector3 NormalisedCopy(void) const;

		CGUIVector3 MidPoint( const CGUIVector3& vec ) const;

		void MakeFloor( const CGUIVector3& cmp );
		void MakeCeil( const CGUIVector3& cmp );

		CGUIVector3 Perpendicular(void) const;

		CGUIVector3 Reflect(const CGUIVector3& normal) const;

		bool PositionEquals(const CGUIVector3& rhs ) const;
	};
}
