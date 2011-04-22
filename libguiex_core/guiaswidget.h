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
		virtual void OnUpdate();

		GUI_AS_GENERATOR_DECLARE( CGUIAsWidgetAlpha);
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
		virtual void OnUpdate();

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
		virtual void OnUpdate();

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
		virtual void OnUpdate();

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
		virtual void OnUpdate();

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
		virtual void OnUpdate();

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

}

#endif //__GUI_ASWIDGET_20110422_H__

