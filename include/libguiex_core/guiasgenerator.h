/** 
* @file guiasgenerator.h
* @brief used to generate as
* @author ken
* @date 2007-11-21
*/

#ifndef __GUI_AS_GENERATOR_20071121_H__
#define __GUI_AS_GENERATOR_20071121_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"



//============================================================================//
// define
//============================================================================// 
#define GUI_AS_GENERATOR_DECLARE(asType)		\
public:	\
	friend class asType##_Generator;			\
	class asType##_Generator : public ::guiex::CGUIAsGenerator	\
	{														\
	public:													\
	virtual ::guiex::CGUIAs*	GenerateAs(const CGUIString& rAsName, const CGUIString& rSceneName) const;			\
	virtual void DestroyAs(const ::guiex::CGUIAs* pAS) const;\
	static asType##_Generator* GenerateSelf();					\
	virtual void DestroySelf();							\
	protected:												\
	asType##_Generator();										\
	};														


#define GUI_AS_GENERATOR_IMPLEMENT(asType)	\
	asType::asType##_Generator::asType##_Generator()									\
	:CGUIAsGenerator(#asType)							\
	{														\
	}														\
	::guiex::CGUIAs* asType::asType##_Generator::GenerateAs( const CGUIString& rAsName, const CGUIString& rSceneName ) const			\
	{														\
	return new asType(rAsName, rSceneName);									\
	}														\
	void asType::asType##_Generator::DestroyAs(const ::guiex::CGUIAs* pAS) const\
	{														\
	delete pAS;										\
	}														\
	asType::asType##_Generator* asType::asType##_Generator::GenerateSelf()					\
	{														\
	return new asType::asType##_Generator;								\
	}														\
	void asType::asType##_Generator::DestroySelf()							\
	{														\
	delete this;									\
	}												



//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIAs;
	class CGUIWidget;
}//namespace guiex



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIAsGenerator
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsGenerator(const CGUIString& rString);

		/**
		* @brief destructor
		*/
		virtual ~CGUIAsGenerator();

		/** 
		* @brief generate a as
		* @return pointer of the as, return NULL for failed to generate it
		*/
		virtual CGUIAs*	GenerateAs( const CGUIString& rAsName, const CGUIString& rSceneName ) const=0;

		/**
		* @brief destroy as
		*/
		virtual void DestroyAs(const CGUIAs* pAS) const= 0;

		/**
		* @brief destroy generator itself
		*/
		virtual void DestroySelf() = 0;

		/**
		* @brief get type of the as
		*/
		virtual const CGUIString& GetAsType() const;

	protected:
		CGUIString	m_strType;
	};


}//namespace guiex

#endif //__GUI_AS_GENERATOR_20071121_H__
