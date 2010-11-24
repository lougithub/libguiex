/** 
* @file guisingleton.h
* @brief singleton used in this system
* @author ken
* @date 2006-05-30
*/

#ifndef __GUI_SINGLETON_20060530_H__
#define __GUI_SINGLETON_20060530_H__

//============================================================================//
// include
//============================================================================// 
#include "guiconfig.h"
#include "guitypes.h"
#include "guicleanup.h"

//============================================================================//
// class
//============================================================================// 

namespace guiex
{

	/**
	* @class CGUISingleton
	*
	* @brief A Singleton Adapter uses the Adapter pattern to turn ordinary
	* classes into Singletons.a single <CGUISingleton<TYPE> > instance is 
	* allocated here,* not a <TYPE> instance.  The reason for this is to allow
	* registration with the ACE_Object_Manager, so that the Singleton can be 
	* cleaned up when the process exits.  For this scheme to work, a (static) 
	* cleanup() function must be provided.  ACE_Singleton provides one so that 
	* TYPE doesn't need to.
	*
	* If you want to make sure that only the singleton instance of <T> is created, 
	* and that users cannot create their own instances of <T>, do the following to 
	* class <T>:
	* (a) Make the constructor of <T> private (or protected)
	* (b) Make Singleton a friend of <T>
	* Here is an example:
	* @verbatim
	* class foo
	* {
	* friend class ACE_Singleton<foo>;
	* private:
	* foo () { cout << "foo constructed" << endl; }
	* ~foo () { cout << "foo destroyed" << endl; }
	* };
	* typedef CGUISingleton<foo> FOO;
	* @endverbatim
	*
	* @note this singleton only can be used in one process address, so if you
	* use it in different process, such as dll, there will be error!
	*/
	template <typename TYPE>
	class CGUISingleton : public CGUICleanup
	{
	public:
		/**
		* @brief get instance of singleton class
		*/
		static TYPE* Instance();

		/**
		* @brief destroy instance of singleton class
		*/
		static void DestroyInstance();

		/**
		* @brief to destroy this singleton
		* @note only for internal use by CGUIObjectManager
		*/
		virtual void Cleanup ();

	protected:
		/** 
		* @brief constructor
		*/
		CGUISingleton( );

		/// Get pointer to the Singleton instance.
		static CGUISingleton<TYPE> *&Instance_i (void);

	protected:
		TYPE m_aInstance;

		/// Pointer to the Singleton instance.
		static CGUISingleton<TYPE> * m_pSingleton;

	};

}	//namespace guiex


//============================================================================//
// define
//============================================================================// 

/**
* @brief allow application use singleton in different dll, 
* used in class declaration scope
*/
#define GUI_SINGLETON_DECLARE_EX(classname) \
public: \
	static classname* Instance(); \
	static void DestroyInstance(); \
	static void AfterDestroyInstance(); \
private: \
	static classname * m_pSingleton; \
	friend class ::guiex::CGUICleanupAdapter<classname>; \
	static ::guiex::CGUICleanupAdapter<classname>* m_cleanupAdapter; \

/**
* @brief allow application use singleton in different dll, 
* used in cpp file
*/
#define GUI_SINGLETON_IMPLEMENT_EX(classname) \
	classname * classname::m_pSingleton = NULL; \
	::guiex::CGUICleanupAdapter<classname> * classname::m_cleanupAdapter = NULL; \
	classname * classname::Instance () \
	{ \
		if (m_pSingleton == NULL) \
		{ \
			if (::guiex::CGUIObjectManager::IsStartingUp () || \
			::guiex::CGUIObjectManager::IsShuttingDown ()) \
			{ \
				if( !(m_pSingleton = new classname )) \
				{ \
					return NULL; \
				} \
			} \
			else \
			{ \
				if( !(m_pSingleton = new classname )) \
				{ \
					return NULL; \
				} \
				m_cleanupAdapter = new ::guiex::CGUICleanupAdapter<classname>(m_pSingleton); \
				::guiex::CGUIObjectManager::Instance()->Register(m_cleanupAdapter); \
			} \
		} \
		return m_pSingleton; \
	} \
	void classname::DestroyInstance() \
	{ \
		if (m_pSingleton != NULL) \
		{ \
			::guiex::CGUIObjectManager::Instance()->Unregister(m_cleanupAdapter); \
		} \
		AfterDestroyInstance(); \
	} \
	void classname::AfterDestroyInstance() \
	{ \
		m_pSingleton = NULL; \
		m_cleanupAdapter = NULL; \
	}



#include "guisingleton.inl"

#endif //__GUI_SINGLETON_20060530_H__
