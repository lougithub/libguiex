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
		CGUIFrameworkBase( const CGUISize& rScreenSize, const CGUIString& rDataPath );
		virtual ~CGUIFrameworkBase();

		int32 Initialize( );
		void Release( );

		void Update( real fDeltaTime );
		void Render( );

		const CGUISize& GetScreenSize( ) const; 
		const CGUIString& GetDataPath( ) const; 

	protected:
		virtual int32 InitializeGame( ) = 0;
		virtual void ReleaseGame( ) = 0;
		
		virtual int32 InitializeSystem( ) = 0;
		virtual void ReleaseSystem( ) = 0;

		virtual void PreUpdate( real fDeltaTime ) = 0;
		virtual void PostUpdate( real fDeltaTime ) = 0;

		void UpdateSystem( real fDeltaTime );

		virtual void OutputFatalError( const char* pErrorDesc ) = 0;

	private:
		bool m_bIsInitialized;

		CGUISize m_aScreenSize;
		CGUIString m_strDataPath;
	};

}//namespace guiex


#endif	//__GUI_FRAMEWORKBASE_20101210_H__

