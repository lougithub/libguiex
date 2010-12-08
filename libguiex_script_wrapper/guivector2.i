
namespace guiex
{

	class CGUIVector2
	{
	public:
		CGUIVector2();
		CGUIVector2( real fX, real fY );
		CGUIVector2( const CGUIVector2& rkVector );

		void Set( real fX, real fY);
		
		void SetX( real _x );
		real GetX( ) const;
		void SetY( real _y );
		real GetY( ) const;
		
		real Length () const;
		bool IsZeroLength(void) const;

		real SquaredLength () const;
		
		real DotProduct(const CGUIVector2& vec) const;
		CGUIVector2 CrossProduct( const CGUIVector2& rkVector ) const;

		real Normalise();

		CGUIVector2 MidPoint( const CGUIVector2& vec ) const;

		void MakeFloor( const CGUIVector2& cmp );
		void MakeCeil( const CGUIVector2& cmp );

		CGUIVector2 Perpendicular(void) const;

		CGUIVector2 RandomDeviant( real angle) const;

		CGUIVector2 NormalisedCopy(void) const;

		CGUIVector2 Reflect(const CGUIVector2& normal) const;
	};
}
