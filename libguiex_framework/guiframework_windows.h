/** 
* @file guiframework_windows.h
* @brief framework to use this engine
* @author ken
* @date 2010-12-10
*/

#ifndef __GUI_FRAMEWORKWINDOWS_20101210_H__
#define __GUI_FRAMEWORKWINDOWS_20101210_H__

//============================================================================//
// include
//============================================================================// 
#include "guiframeworkbase.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIFramework_Windows : public CGUIFrameworkBase
	{
	public:
		CGUIFramework_Windows( );
		virtual ~CGUIFramework_Windows();


	protected:
		virtual int32 InitializeGame( );
		virtual void ReleaseGame( );

		virtual int32 InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath );
		virtual void ReleaseSystem( );

		virtual void SetupLogSystem( );
		virtual void RegisterWidgetGenerators( );

		virtual void RegisterInterfaces_Render( );
		virtual void RegisterInterfaces_ImageLoader( );
		virtual void RegisterInterfaces_FileSys( );
		virtual void RegisterInterfaces_Mouse( );
		virtual void RegisterInterfaces_Keyboard( );
		virtual void RegisterInterfaces_Font( );
		virtual void RegisterInterfaces_ConfigFile( );
		virtual void RegisterInterfaces_StringConv( );
		virtual void RegisterInterfaces_Script( );
		virtual void RegisterInterfaces_Physics( );
		virtual void RegisterInterfaces_Sound( );
		virtual void RegisterInterfaces_Ime( );
		virtual void RegisterInterfaces_LocalizationLoader( );

		virtual void PreUpdate( real fDeltaTime );
		virtual void PostUpdate( real fDeltaTime );

		virtual void OutputFatalError( const char* pErrorDesc );

	private:
	};

}//namespace guiex


#endif	//__GUI_FRAMEWORKWINDOWS_20101210_H__

