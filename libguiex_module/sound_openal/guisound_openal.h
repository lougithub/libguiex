/** 
 * @file guissound_openal.h
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_OPENAL_20060831H__
#define __GUI_SOUND_OPENAL_20060831H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacesound.h>
#include <map>
#include <al.h>
#include <alc.h>

#include <windows.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUISound_openal : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_openal();
		
		/** 
		 * @brief destructor
		 */
		virtual ~IGUISound_openal();
		
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
		
		/// struct data
		struct SSoundData
		{
			uint32 m_nSourceId;
			uint32 m_nBufferId;
		};
		
		/// load wav file
		bool LoadWavFile( const CGUIString& rFilename, SSoundData& rSoundData);

		/// load ogg file
		bool LoadOggFile( const CGUIString& rFilename,SSoundData& rSoundData);
		
		bool SetAlBuffer( SSoundData& rSoundData, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq );

	private:
		ALCcontext * m_pContext;
		ALCdevice * m_pDevice;
		
		typedef std::map<uint32, CGUISoundData*> TMapSoundData;
		TMapSoundData m_mapSoundData;

		typedef std::map<int32, SSoundData>	TMapSound;
		TMapSound m_mapEffect; /// map of sound
		
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_OPENAL_20060831H__

