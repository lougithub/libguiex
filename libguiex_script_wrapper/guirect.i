
namespace guiex
{

	class CGUIRect
	{
	public:
		CGUIRect(void);
		CGUIRect(real left, real top, real right, real bottom);
		CGUIRect( const CGUIVector2& pos, const CGUISize& sz);
		CGUIRect(const CGUIRect& rRect);

		CGUIVector2	GetPosition(void) const;

		void SetRect(const CGUIRect& rRect);
		void SetRect(real left, real top, real right, real bottom);
		void SetRect(const CGUIVector2& pos, const CGUISize& sz);

		real GetWidth(void) const;
		real GetHeight(void) const;
		CGUISize GetSize(void) const;
		void SetWidth(real width);
		void SetHeight(real height);
		void SetSize(const CGUISize& sze);
		void SetPosition(const CGUIVector2& pt);

		CGUIRect GetIntersection(const CGUIRect& rect) const;

		CGUIRect& Offset(const CGUIVector2& pt);

		bool IsPointInRect(const CGUIVector2& pt) const;

		CGUIRect& ConstrainSizeMax(const CGUISize& sz);
		CGUIRect& ConstrainSizeMin(const CGUISize& sz);
		CGUIRect& ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz);

		bool IsEqual( const CGUIRect& rhs ) const;
	};
}
