namespace guiex
{
	class CGUIAs
	{
	public:
		virtual void Reset( );
		void SetReverse( bool bReverse );

	protected:
		CGUIAs();
		virtual ~CGUIAs();
	};
	
	template< class T >
	class CGUIAsInterpolation : public CGUIAs
	{
	protected:
		CGUIAsInterpolation();
		virtual ~CGUIAsInterpolation();
	};
	%template(CGUIInterpolationBase_real) CGUIAsInterpolation<real>;
	%template(CGUIInterpolationBase_size) CGUIAsInterpolation<CGUISize>;
	%template(CGUIInterpolationBase_Vector3) CGUIAsInterpolation<CGUIVector3>;
	%template(CGUIInterpolationBase_Vector2) CGUIAsInterpolation<CGUIVector2>;
	%template(CGUIInterpolationBase_Color) CGUIAsInterpolation<CGUIColor>;
	
	class CGUIAsWidgetAlpha : public CGUIAsInterpolation<real>
	{
	protected:
		CGUIAsWidgetAlpha();
		virtual ~CGUIAsWidgetAlpha();
	};

	class CGUIAsWidgetScale : public CGUIAsInterpolation<CGUISize>
	{
	protected:
		CGUIAsWidgetScale();
		virtual ~CGUIAsWidgetScale();
	};
	
	class CGUIAsWidgetRotation: public CGUIAsInterpolation<CGUIVector3>
	{
	protected:
		CGUIAsWidgetRotation();
		virtual ~CGUIAsWidgetRotation();
	};	
	
	class CGUIAsWidgetPosition : public CGUIAsInterpolation<CGUIVector2>
	{
	protected:
		CGUIAsWidgetPosition();
		virtual ~CGUIAsWidgetPosition();
	};
	
	class CGUIAsWidgetColor : public CGUIAsInterpolation<CGUIColor>
	{
	protected:
		CGUIAsWidgetColor();
		virtual ~CGUIAsWidgetColor();
	};
	
	class CGUIAsContainer : public CGUIAs
	{
	protected:
		CGUIAsContainer();
		virtual ~CGUIAsContainer();
	};
}
