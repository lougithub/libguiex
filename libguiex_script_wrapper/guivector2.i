
namespace guiex
{

	class CGUIVector2
	{
	public:
		real x, y;        

		CGUIVector2();
		CGUIVector2( real fX, real fY );
		CGUIVector2( const CGUIVector2& rkVector );
		~CGUIVector2();

		bool operator == ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator + ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator - ( const CGUIVector2& rkVector ) const;
		CGUIVector2 operator * ( real fScalar ) const;
		CGUIVector2 operator * ( const CGUIVector2& rhs) const;
		CGUIVector2 operator / ( real fScalar ) const;
		CGUIVector2 operator - () const;

		void Set( real fX, real fY);
		real Length () const;
		real SquaredLength () const;
		real DotProduct(const CGUIVector2& vec) const;
		real Normalise();
		CGUIVector2 MidPoint( const CGUIVector2& vec ) const;
		void MakeFloor( const CGUIVector2& cmp );
		void MakeCeil( const CGUIVector2& cmp );
		CGUIVector2 Perpendicular(void) const;
		CGUIVector2 CrossProduct( const CGUIVector2& rkVector ) const;
		CGUIVector2 RandomDeviant( real angle) const;
		bool IsZeroLength(void) const;
		CGUIVector2 NormalisedCopy(void) const;
		CGUIVector2 Reflect(const CGUIVector2& normal) const;

	};
}
