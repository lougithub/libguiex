
namespace guiex
{
	class CGUIColorRect
	{
	public:
		CGUIColorRect(void);
		CGUIColorRect(const CGUIColor& col);
		CGUIColorRect(const CGUIColor& top_left, const CGUIColor& top_right, const CGUIColor& bottom_left, const CGUIColor& bottom_right);

		void SetAlpha(real alpha);
		void SetColours(const CGUIColor	& col);

		bool IsMonochromatic() const;

		CGUIColorRect GetSubRectangle( real left, real right, real top, real bottom ) const;

		CGUIColor GetColorAtPoint( real x, real y ) const;

		void ModulateAlpha(real alpha);
	};
}

