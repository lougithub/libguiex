/** 
 * @file guiexception.h
 * @brief exception used in this system
 * @author ken
 * @date 2006-05-26
 */

#ifndef __GUI_EXCEPTION_H_20060526__
#define __GUI_EXCEPTION_H_20060526__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include <exception>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{	
/**
 * @class CGUIBaseException
 * @brief base class of exception, used for guiex system..
 */
class GUIEXPORT CGUIBaseException : public std::exception
{
public:
	CGUIBaseException( );
	CGUIBaseException( const CGUIString& rError );
  virtual const char *what( ) const;

protected:
	CGUIString	m_strError;
};

/**
 * @class CGUIException
 * @brief common exception, used for guiex system..
 */
class GUIEXPORT CGUIException : public CGUIBaseException
{
public:
   CGUIException( const CGUIString& rError );

   CGUIException( const char *format, ... );
};

/**
 * @class CGUIException_Script
 * @brief exception caused by parse script
 */
class GUIEXPORT CGUIException_Script : public CGUIBaseException
{
public:
   CGUIException_Script( const CGUIString& rError );

   CGUIException_Script( const char *format, ... );
};

//============================================================================//
// function
//============================================================================// 
/**
 * @brief throw a exception
 */
GUIEXPORT void ThrowException(const CGUIString& rError);



}//namespace guiex

#endif //__GUI_EXCEPTION_H_20060526__

