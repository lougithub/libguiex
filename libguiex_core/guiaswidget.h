/** 
* @file guiaswidget.h
* @brief widget action sequence
* @author ken
* @date 2011-04-22
*/

#ifndef __GUI_ASWIDGET_20110422_H__
#define __GUI_ASWIDGET_20110422_H__

//============================================================================//
// include
//============================================================================// 
#include "guias.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAsWidgetAlpha
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetAlpha
	* @brief the as, change alpha of widget
	*/
	class GUIEXPORT CGUIAsWidgetAlpha : public CGUIAsInterpolation<real>
	{
	protected:
		CGUIAsWidgetAlpha(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetAlpha);
	};

	//*****************************************************************************
	//	CGUIAsWidgetChildrenAlpha
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetChildrenAlpha
	* @brief the as, change alpha of widget's children
	*/
	class GUIEXPORT CGUIAsWidgetChildrenAlpha : public CGUIAsInterpolation<real>
	{
	protected:
		CGUIAsWidgetChildrenAlpha(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetChildrenAlpha);
	};

	//*****************************************************************************
	//	CGUIAsWidgetScale
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetScale
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsWidgetScale : public CGUIAsInterpolation<CGUISize>
	{
	protected:
		CGUIAsWidgetScale(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetScale);
	};


	//*****************************************************************************
	//	CGUIAsWidgetRotation
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetRotation
	* @brief the as, change rotation of widget
	*/
	class GUIEXPORT CGUIAsWidgetRotation : public CGUIAsInterpolation<CGUIVector3>
	{
	protected:
		CGUIAsWidgetRotation(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetRotation);
	};


	//*****************************************************************************
	//	CGUIAsWidgetPosition
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetPosition
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsWidgetPosition : public CGUIAsInterpolation<CGUIVector2>
	{
	protected:
		CGUIAsWidgetPosition(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetPosition);
	};


	//*****************************************************************************
	//	CGUIAsWidgetSize
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetSize
	* @brief the as, change size of widget
	*/
	class GUIEXPORT CGUIAsWidgetSize : public CGUIAsInterpolation<CGUISize>
	{
	protected:
		CGUIAsWidgetSize(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetSize);
	};


	//*****************************************************************************
	//	CGUIAsWidgetColor
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetColor
	* @brief the as, change color of widget
	*/
	class GUIEXPORT CGUIAsWidgetColor : public CGUIAsInterpolation<CGUIColor>
	{
	protected:
		CGUIAsWidgetColor(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void ApplyValue();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetColor);
	};


	
	//*****************************************************************************
	//	CGUIAsWidgetMoveTo
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetMoveTo
	* @brief move widget to destination with given velocity.
	*/
	class GUIEXPORT CGUIAsWidgetMoveTo : public CGUIAs
	{
	public:
		void SetVelocity( real fVelocity );
		real GetVelocity( ) const;
		void SetDestination( const CGUIVector2& rDestination );
		const CGUIVector2& GetDestination( ) const;

		virtual real Update( real fDeltaTime );

	protected:
		CGUIAsWidgetMoveTo(const CGUIString& rAsName, const CGUIString& rSceneName);

	private:
		real m_fVelocity; //velocity per second
		CGUIVector2 m_aDestination;

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetMoveTo);
	};

	//*****************************************************************************
	//	CGUIAsWidgetVisible
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetVisible
	* @brief the as, change visible of widget
	*/
	class GUIEXPORT CGUIAsWidgetVisible : public CGUIAs
	{
	protected:
		CGUIAsWidgetVisible(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnRetired();

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

	protected:
		void SetVisible( bool bVisible );
		bool IsVisible() const;

	protected:
		bool m_bVisible;

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetVisible);
	};
	
	//*****************************************************************************
	//	CGUIAsWidgetChildrenVisible
	//*****************************************************************************
	/**
	* @class CGUIAsWidgetChildrenVisible
	* @brief the as, change visible of widget children
	*/
	class GUIEXPORT CGUIAsWidgetChildrenVisible : public CGUIAs
	{
	protected:
		CGUIAsWidgetChildrenVisible(const CGUIString& rAsName, const CGUIString& rSceneName);

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

	protected:
		virtual void OnRetired();

	protected:
		void SetVisible( bool bVisible );
		bool IsVisible() const;

	protected:
		bool m_bVisible;

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetChildrenVisible);
	};
}

#endif //__GUI_ASWIDGET_20110422_H__

