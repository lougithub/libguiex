/** 
 * @file guistring.h
 * @brief string used in this system, which could convert 
 * string code automatically
 * @author ken
 * @date 2006-04-09
 */

#ifndef __GUI_STRING_EX_20060409_H__
#define __GUI_STRING_EX_20060409_H__

//============================================================================//
// 	include
//============================================================================// 
#include "guibase.h"
#include "guicolor.h"
#include "guistring.h"
#include  <string>
#include  <vector>

//============================================================================//
// 	class
//============================================================================// 

namespace guiex
{

	class GUIEXPORT CGUIStringExInfo
	{	

	public:
		CGUIStringExInfo();
		CGUIStringExInfo(int16 nFontIdx,uint16 nFontSize,const CGUIColor& rColor);
		CGUIStringExInfo(const CGUIStringExInfo& rInfo);

		bool					operator==(const CGUIStringExInfo& rInfo ) const;
		bool					operator!=(const CGUIStringExInfo& rInfo ) const;
		const CGUIStringExInfo&	operator=(const CGUIStringExInfo& rInfo );

	public:
		int16		m_nFontIdx;
		uint16		m_nFontSize;
		CGUIColor	m_aColor;
	};

	class GUIEXPORT CGUIStringEx
	{
	public:


	public:
		/**
		* @brief constructor
		*/
		CGUIStringEx();

		/**
		* @brief copy constructor
		*/
		CGUIStringEx(const CGUIStringEx& rString );

		/** 
		* @brief constructor 
		* @param pChar point of character.
		*/
		CGUIStringEx(const wchar_t* pChar);

		/** 
		* @brief constructor 
		* @param std::wstring.object containing the string value to be assigned
		*/
		CGUIStringEx(const std::wstring& str);

		/** 
		* @brief constructor 
		* @param rString.object containing the multi byte string value 
		* @param pSrcCode encode of given string
		*/
		CGUIStringEx(const CGUIString& rString, const char* pSrcCode = NULL );

		/**
		* @brief destructor
		*/
		~CGUIStringEx();

		/**
		* @brief set string default info
		*/
		void	SetDefaultInfo(const CGUIStringExInfo* pInfo);

		/**
		* @brief get string default info
		*/
		const CGUIStringExInfo&	GetDefaultInfo() const;

		/**
		* @brief set string content
		*/
		void	SetContent(const wchar_t* pContent);

		/**
		* @brief get string content
		*/
		const wchar_t*	GetContent( ) const;

		/**
		* @brief is this a rendable string, a rendable should have a default info
		*/
		bool	IsRendable();

		/**
		*@brief Returns the size of the CGUIStringEx
		*@return Number of characters currently in the CGUIStringEx
		*/
		uint32		Size(void) const;

		/**
		*@brief get a character by index
		*/
		wchar_t		GetCharacter(uint32 idx) const;

		/**
		*@brief get a character by index
		*/
		const CGUIStringExInfo&		GetInfo(uint32 idx) const;

		/**
		* @brief return true if string is empty
		*/
		bool		Empty() const;

		/**
		* @brief clear all
		*/
		void		Clear();

		/**
		* @brief insert a character
		*/
		void		Insert( uint32 idx, wchar_t cChar,uint32 nCount, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief insert a wchar_t array
		*/
		void		Insert( uint32 idx, const wchar_t* pChar, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief insert a wchar_t array
		*/
		void		Insert( uint32 idx, const wchar_t* pChar,uint32 nCount, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief insert a string
		*/
		void		Insert(uint32 idx, const CGUIStringEx& rString );

		/**
		* @brief insert a string
		* @param pInfo if it is NULL, it will use default info
		*/
		void		Insert(uint32 idx, const CGUIString& rString, const CGUIStringExInfo* pInfo = NULL, const char* szCode = NULL );

		/**
		* @brief replace character
		*/
		void		Replace(uint32 idx, wchar_t cChar,uint32 nCount, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief erase string
		*/
		void		Erase(uint32 beginIdx, uint32 endIdx);


		/**
		* @brief append character
		*/
		void		Append(const wchar_t cChar,uint32 nCount, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief add a wchar_t array
		*/
		void		Append(const wchar_t* pChar, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief add a wchar_t array
		*/
		void		Append(const wchar_t* pChar, uint32 nCount, const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief assign
		*/
		void		Assign(const CGUIStringEx& rString);

		/**
		* @brief assign
		*/
		void		Assign(const wchar_t* pChar,const CGUIStringExInfo* pInfo = NULL);

		/**
		* @brief add a wchar_t array
		*/
		const CGUIStringEx&	operator+=(const wchar_t* pChar);

		/**
		* @brief compare function
		*/
		bool	operator<(const CGUIStringEx& rStr) const;

		/**
		* @brief compare function
		*/
		bool	operator>(const CGUIStringEx& rStr) const;

		/**
		* @brief compare function
		*/
		bool	operator==(const CGUIStringEx& rStr) const;

		/**
		* @brief assign function
		*/
		const CGUIStringEx&	operator=(const CGUIStringEx& rStr);

	private:

	protected:
		/**
		* @brief insert string info, must be called after insert string content.
		* @param pInfo if it is NULL, means insert a default string info
		*/
		void		InsertStringInfo(uint32 idx, uint32 count, const CGUIStringExInfo* pInfo);

		/**
		* @brief replace string info
		* @param pInfo if it is NULL, means not replace this info
		*/
		void		ReplaceStringInfo(uint32 idx, uint32 count, const CGUIStringExInfo* pInfo);

		/**
		* @brief erase string info
		*/
		void		EraseStringInfo(uint32 beginIdx, uint32 endIdx);

	protected:
		CGUIStringExInfo							m_aDefaultInfo;	///< the default info of string, 
																	///< if it's null, means this string only content character info, 
																	///< and can't be
		typedef	std::vector<CGUIStringExInfo>		TVecStringInfo;
		TVecStringInfo								m_vecInfo;		///< the detail info of each character
		std::wstring								m_strContent;	///< container which hold string.
	};

}//namespace guiex


#endif //__GUI_STRING_EX_20060409_H__




