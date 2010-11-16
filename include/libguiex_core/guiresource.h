/** 
 * @file guiresource.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-26
 */






#ifndef __KEN_GUIRESOURCE_20091026_H__
#define	__KEN_GUIRESOURCE_20091026_H__





//============================================================================//
// include
//============================================================================//





//============================================================================//
// declare
//============================================================================//





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

		int32	Load();
		void	Unload();
		bool	IsLoaded() const;

		const CGUIString&	GetName() const;
		const CGUIString&	GetSceneName() const;
		const CGUIString&	GetResourceType() const;

	protected:
		virtual int32	DoLoad() = 0;
		virtual void	DoUnload() = 0;

	protected:
		enum	ELoadState
		{
			LOADSTATE_Unloaded,
			LOADSTATE_Loaded,
		};
		ELoadState	m_eIsLoaded;

		CGUIString	m_strName;
		CGUIString	m_strSceneName;
		CGUIString	m_strResourceType;
	};
}





#endif //__KEN_GUIRESOURCE_20091026_H__
