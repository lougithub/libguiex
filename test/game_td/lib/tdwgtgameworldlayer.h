/** 
* @file tdwgtgameworldlayer.h
* @brief used to render monster and other things.
* @author ken
* @date 2011-08-19
*/

#ifndef __TD_WGTGAMEWORLDLAYER_20110819_H__
#define __TD_WGTGAMEWORLDLAYER_20110819_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CTDWgtGameWorldLayer
	* @brief used to render monster and other things.
	*/
	class GUIEXPORT CTDWgtGameWorldLayer : public CGUIWidget
	{
	public:
		CTDWgtGameWorldLayer( const CGUIString& rName, const CGUIString& rSceneName );

		void ApplyGameWorld( class CTDGameWorldBase* pWorld );
		class CTDGameWorldBase* GetGameWorld( ) const;

	protected:
		void InitGameLayer();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		virtual void OnUpdate( real fDeltaTime );

	protected:	//!< callback function

	protected:
		class CTDGameWorldBase* m_pGameWorld;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CTDWgtGameWorldLayer);
	};
}//namespace guiex

#endif //__TD_WGTGAMEWORLDLAYER_20110819_H__

