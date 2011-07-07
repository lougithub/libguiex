/** 
* @file guiframeworkbase.h
* @brief framework to use this engine
* @author ken
* @date 2010-12-10
*/

#ifndef __GUI_FRAMEWORKBASE_20101210_H__
#define __GUI_FRAMEWORKBASE_20101210_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIFrameworkBase
	{
	public:
		CGUIFrameworkBase( );
		virtual ~CGUIFrameworkBase();

		int32 Initialize( const CGUIIntSize& rScreenSize, const char* pDataPath );
		void Release( );

		void Update( real fDeltaTime );
		void Render( );

	protected:
		virtual int32 InitializeGame( ) = 0;
		virtual void ReleaseGame( ) = 0;
		
		virtual int32 InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath ) = 0;
		virtual void ReleaseSystem( ) = 0;

		virtual void SetupLogSystem( ) = 0;
		virtual void RegisterWidgetGenerators( ) = 0;

		virtual void RegisterInterfaces( );
		virtual void RegisterInterfaces_Render( ) = 0;
		virtual void RegisterInterfaces_ImageLoader( ) = 0;
		virtual void RegisterInterfaces_FileSys( ) = 0;
		virtual void RegisterInterfaces_Mouse( ) = 0;
		virtual void RegisterInterfaces_Keyboard( ) = 0;
		virtual void RegisterInterfaces_Font( ) = 0;
		virtual void RegisterInterfaces_ConfigFile( ) = 0;
		virtual void RegisterInterfaces_StringConv( ) = 0;
		virtual void RegisterInterfaces_Script( ) = 0;
		virtual void RegisterInterfaces_Physics( ) = 0;
		virtual void RegisterInterfaces_Sound( ) = 0;
		virtual void RegisterInterfaces_Ime( ) = 0;
		virtual void RegisterInterfaces_LocalizationLoader( ) = 0;

		virtual void PreUpdate( real fDeltaTime ) = 0;
		virtual void PostUpdate( real fDeltaTime ) = 0;
		virtual void PreRender( );
		virtual void PostRender( );

		void UpdateSystem( real fDeltaTime );

		virtual void OutputFatalError( const char* pErrorDesc ) = 0;

	private:
		bool m_bIsInitialized;
	};

}//namespace guiex


#endif	//__GUI_FRAMEWORKBASE_20101210_H__

