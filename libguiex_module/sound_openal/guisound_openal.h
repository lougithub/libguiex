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
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace guiex
{
	
	//============================================================================//
	// class
	//============================================================================// 
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
		virtual int32 LoadEffect( int32 nIdx,  const CGUIString& rFileName);
		
		/**
		 * @brief unload effect.
		 */
		virtual void UnloadEffect( int32 nIdx );
		
		/**
		 * @brief play effect.
		 */
		virtual int32 PlayEffect( int32 nIdx );
		
		virtual void DeleteSelf();
		
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
			unsigned int m_nSourceId;
			unsigned int m_nBufferId;
		};
		
		/// load wav file
		bool LoadWavFile( const CGUIString& rFilename, SSoundData& rSoundData);
		
		/// load ogg file
		bool LoadOggFile( const CGUIString& rFilename,SSoundData& rSoundData);
		
	private:
		ALCcontext * m_pContext;
		ALCdevice * m_pDevice;
		
		
		typedef std::map<int32, SSoundData>	TMapSound;
		TMapSound m_mapEffect; /// map of sound
		
	public:
		const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namesace guiex

#endif //__GUI_SOUND_OPENAL_20060831H__

