/** 
 * @file guissound_cocoa.h
 * @brief sound module, use dummy
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_COCOA_20060831H__
#define __GUI_SOUND_COCOA_20060831H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#include <map>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUISound_cocoa : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_cocoa();
		
		/** 
		 * @brief destructor
		 */
		virtual ~IGUISound_cocoa();
		
		/**
		 * @brief load effect.
		 * @return 0 for success.
		 */
		virtual int32 LoadEffect( int32 nID,  const CGUIString& rFileName);
		
		/**
		 * @brief unload effect.
		 */
		virtual void UnloadEffect( int32 nID );
		
		/**
		 * @brief play effect.
		 */
		virtual int32 PlayEffect( int32 nID );
		
		virtual void DeleteSelf();

		virtual CGUISoundData* CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nSoundID );
		virtual void DestroySoundData( CGUISoundData* pData );
		
	protected:
		/** 
		 * @brief initialize sound
		 * @return 0 for success
		 */
		virtual int DoInitialize(void* );
		
		/** 
		 * @brief destroy sound 
		 * @return 0 for success
		 */
		virtual void DoDestroy();
		
		
	private:
		typedef std::map<uint32, CGUISoundData*> TMapSoundData;
		TMapSoundData m_mapSoundData;
		
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_COCOA_20060831H__

