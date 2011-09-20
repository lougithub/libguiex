/** 
* @file guiwgtparticle2d.h
* @brief used to show a particle 2d system
* @author ken
* @date 2011-01-14
*/

#ifndef __GUI_WGTSTATICPARTICLE2DSYSTEM_20110114_H__
#define __GUI_WGTSTATICPARTICLE2DSYSTEM_20110114_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include <libguiex_core/guimatrix4.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticle2DSystem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtParticle2D
	*/
	class GUIEXPORT CGUIWgtParticle2D : public CGUIWidget
	{
	public:
		CGUIWgtParticle2D( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtParticle2D();

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		void SetParticle2D( const CGUIString& rParticle2DName );
		void SetParticle2D( CGUIParticle2DSystem* pParticle2D );

		void SetParticleOffset( const CGUIVector2& rOffset);
		const CGUIVector2& GetParticleOffset(  ) const;

	protected:
		CGUIWgtParticle2D( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitStaticParticle2DSystem();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnUpdate( real fDeltaTime );
		virtual void OnDestroy();
		virtual void RefreshSelf();

	protected:
		CGUIParticle2DSystem* m_pParticle2DSystem;
		CGUIMatrix4 m_aOffsetMatrix;

		bool m_bAutoDestoryOnFinish;
		CGUIVector2 m_aParticleOffset;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtParticle2D);
	};


}//namespace guiex

#endif //__GUI_WGTSTATICPARTICLE2DSYSTEM_20110114_H__

