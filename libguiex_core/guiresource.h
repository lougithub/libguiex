/** 
 * @file guiresource.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-26
 */


#ifndef __KEN_GUIRESOURCE_20091026_H__
#define	__KEN_GUIRESOURCE_20091026_H__

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUIResource
	{
	public:
		CGUIResource(const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rResourceType);
		virtual ~CGUIResource();

		int32 Load() const;
		void Unload();
		bool IsLoaded() const;

		const CGUIString& GetName() const;
		const CGUIString& GetSceneName() const;
		const CGUIString& GetResourceType() const;

		void RefRetain() const;
		void RefRelease() const;
		void RefClear() const;
		uint32 GetRefCount() const;

	protected:
		virtual int32 DoLoad() const = 0;
		virtual void DoUnload() = 0;

	protected:
		enum ELoadState
		{
			LOADSTATE_Unloaded,
			LOADSTATE_Loaded,
		};
		mutable ELoadState m_eIsLoaded;

		CGUIString m_strName;
		CGUIString m_strSceneName;
		CGUIString m_strResourceType;

	private:
		mutable uint32 m_nReferenceCount;
	};
}





#endif //__KEN_GUIRESOURCE_20091026_H__
