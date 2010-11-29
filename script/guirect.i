
namespace guiex
{

	class CGUIRect
	{
	public:
		CGUIRect(void);
		CGUIRect(real left, real top, real right, real bottom);
		CGUIRect(CGUIVector2 pos, CGUISize sz);
		CGUIRect(const CGUIRect& rRect);
		~CGUIRect(void);

		CGUIVector2 GetPosition(void) const;
		void	SetRect(const CGUIRect& rRect);
		real	GetWidth(void) const;
		real	GetHeight(void) const;
		CGUISize	GetSize(void) const;
		void	SetPosition(const CGUIVector2 & pt);
		void	SetWidth(real width);
		void	SetHeight(real height);
		void	SetSize(const CGUISize& sze);

		CGUIRect	GetIntersection(const CGUIRect& rect) const;
		CGUIRect&	Offset(const CGUIVector2& pt);
		bool	IsPointInRect(const CGUIVector2& pt) const;
		CGUIRect&	ConstrainSizeMax(const CGUISize& sz);
		CGUIRect&	ConstrainSizeMin(const CGUISize& sz);
		CGUIRect&	ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz);

		bool	operator==(const CGUIRect& rhs) const;
		CGUIRect operator+(const CGUIVector2& rPoint) const;
		CGUIRect operator*(real scalar) const;

		real	m_fTop;
		real	m_fBottom;
		real	m_fLeft;
		real	m_fRight;
	};
}
