/** 
 * @file guissound_dummy.h
 * @brief sound module, use dummy
 * @author ken
 * @date 2006-08-31
 */

#ifndef __GUI_SOUND_DUMMY_20060831H__
#define __GUI_SOUND_DUMMY_20060831H__

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
	class GUIEXPORT IGUISound_dummy : public IGUIInterfaceSound
	{
	public:
		/** 
		 * @brief constructor
		 */
		IGUISound_dummy();
		
		/** 
		 * @brief destructor
		 */
		virtual ~IGUISound_dummy();
		
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
		
	private:
		typedef std::map<int32, SSoundData>	TMapSound;
		TMapSound m_mapEffect; /// map of sound
		
	public:
		static const char* StaticGetModuleName();
	};
	
	GUI_INTERFACE_DECLARE();
	
}//namespace guiex

#endif //__GUI_SOUND_DUMMY_20060831H__

