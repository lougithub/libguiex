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
#define GUI_AS_GENERATOR_DECLARE(asname)		\
class GUIEXPORT asname##_Generator : public ::guiex::CGUIAsGenerator	\
	{														\
	public:													\
	virtual ::guiex::CGUIAs*	GenerateAs(CGUIWidget* pReceiver) const;			\
	virtual void DestroyAs(const ::guiex::CGUIAs* pAS) const;\
	static asname##_Generator* GenerateSelf();					\
	virtual void DestroySelf();							\
	protected:												\
	asname##_Generator();										\
	};														\
	GUIEXPORT	asname*	CreateAs_##asname(CGUIWidget* pReceiver);


#define GUI_AS_GENERATOR_IMPLEMENT(asname)	\
	asname##_Generator::asname##_Generator()									\
	:CGUIAsGenerator(#asname)							\
	{														\
	}														\
	::guiex::CGUIAs* asname##_Generator::GenerateAs(CGUIWidget* pReceiver) const			\
	{														\
	return new asname(pReceiver);									\
	}														\
	void asname##_Generator::DestroyAs(const ::guiex::CGUIAs* pAS) const\
	{														\
	delete pAS;										\
	}														\
	asname##_Generator* asname##_Generator::GenerateSelf()					\
	{														\
	return new asname##_Generator;								\
	}														\
	void asname##_Generator::DestroySelf()							\
	{														\
	delete this;									\
	}												\
	asname*	CreateAs_##asname(CGUIWidget* pReceiver)\
	{												\
		return static_cast< ::guiex::asname* >( ::guiex::CGUIAsFactory::Instance()->GenerateAs(#asname, pReceiver));\
	}

#define GUI_USER_AS_GENERATOR_DECLARE(asname)		\
class asname##_Generator : public ::guiex::CGUIAsGenerator	\
	{														\
	public:													\
	virtual ::guiex::CGUIAs*	GenerateAs(CGUIWidget* pReceiver) const;			\
	virtual void DestroyAs(const ::guiex::CGUIAs* pAS) const;\
	static asname##_Generator* GenerateSelf();					\
	virtual void DestroySelf();							\
	protected:												\
	asname##_Generator();										\
	};														

#define GUI_USER_AS_GENERATOR_IMPLEMENT(asname)	\
	asname##_Generator::asname##_Generator()									\
	:CGUIAsGenerator(#asname)							\
	{														\
	}														\
	::guiex::CGUIAs* asname##_Generator::GenerateAs(CGUIWidget* pReceiver) const			\
	{														\
	return new asname(pReceiver);									\
	}														\
	void asname##_Generator::DestroyAs(const ::guiex::CGUIAs* pAS) const\
	{														\
	delete pAS;										\
	}														\
	asname##_Generator* asname##_Generator::GenerateSelf()					\
	{														\
	return new asname##_Generator;								\
	}														\
	void asname##_Generator::DestroySelf()							\
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
		virtual CGUIAs*	GenerateAs(CGUIWidget* pReceiver) const=0;

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
