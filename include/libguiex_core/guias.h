/** 
* @file guias.h
* @brief widget action sequence
* @author ken
* @date 2007-11-21
*/

#ifndef __GUI_AS_20071121_H__
#define __GUI_AS_20071121_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guivector2.h"
#include "guivector3.h"

#include "guiasgenerator.h"
#include "guiasfactory.h"

#include "guitimer.h"
#include "guisize.h"
#include <list>


//============================================================================//
// define
//============================================================================//


//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIWidget;
}


//============================================================================//
// function
//============================================================================//
namespace guiex
{
	/**
	* @brief register all as
	* called by CGUIWidgetSystem
	*/
	extern int32 GUIRegisterAllAs();
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAs
	//*****************************************************************************
	/**
	* @class CGUIAs
	* @brief base class of widget as.
	*/
	class GUIEXPORT CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAs( const char* pAsName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIAs();

		/**
		* @brief get as name
		*/
		const CGUIString& GetAsName() const;

		/**
		* @brief set the flag after this as has been processed.
		*/
		void Retire( bool bRetired );

		/**
		* @brief whether the as is retired.
		*/
		bool IsRetired();

		void SetLooping( bool bLooping );
		bool IsLooping();

		/**
		* @brief set widget which receives this as
		*/
		void SetReceiver(CGUIWidget* pReceiver);

		/**
		* @brief Get widget which receives this as
		*/
		CGUIWidget*	GetReceiver();

		/**
		* @brief set delay time
		*/
		void SetTotalTime( real fTotalTime );

		/**
		* @brief get total time
		*/
		real GetTotalTime( ) const;

		/**
		* @brief add a successor as
		*/
		void PushSuccessor( CGUIAs* pAs);

		/**
		* @brief get successor, 
		* @return NULL if there isn't any successor
		*/
		CGUIAs*	PopSuccessor();

		/**
		* @brief Update the as
		*/
		virtual void Update( real fDeltaTime );

		///is this as created by CGUIAsFactory
		bool IsCreateByFactory() const
		{
			return m_pAsGenerator!=NULL;
		}

	protected:
		///widget generator
		friend class CGUIAsFactory;
		//!<set generator
		void SetGenerator( const CGUIAsGenerator* pGenerator);
		//!<get generator
		const CGUIAsGenerator* GetGenerator() const;

	protected:
		real m_fTotalTime;			//!<delay-time, to control when to process this as, millisecond
		real m_fElapsedTime;			

	private:		
		CGUIString m_strAsName;			//!<name of this as
		bool m_bRetired;				//!<should this as be retired
		bool m_bLooping;
		CGUIWidget*	m_pReceiver;		//!<receiver
		
		typedef std::list<CGUIAs*> TListSuccessor;
		TListSuccessor m_listSuccessor;		//!<successor

	private:
		const CGUIAsGenerator* m_pAsGenerator;	//!<generator which used to create as
	};


	template< class T >
	class CGUIAsLinearBase : public CGUIAs
	{
		/**
		* @brief constructor
		*/
		CGUIAsLinearBase( const char* pAsName )
			:CGUIAs( pAsName )
			,m_eLinearType( eLinearType_Normal )
		{
		}

		ELinearType GetLinearType( ) const
		{
			return m_eLinearType;
		}
		void GetLinearType( ELinearType eType ) const
		{
			m_eLinearType = eType;
		}

	protected:
		ELinearType	m_eLinearType;
	};


	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	/**
	* @class CGUIAsAlpha
	* @brief the as, change alpha of widget
	*/
	class GUIEXPORT CGUIAsAlpha : public CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsAlpha();

		/** 
		 * @brief set the parameter of action sequence
		 */
		void SetAlphaSequence( real fBeginValue, real fEndValue, real fTotalTime );

		/**
		* @brief update the event.
		*/
		virtual void Update( real fDeltaTime );

	protected:
		real m_fBeginValue;
		real m_fEndValue;
	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsAlpha);




	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	/**
	* @class CGUIAsScale
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsScale : public CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsScale();

		/** 
		* @brief set the parameter of action sequence
		*/
		void SetScaleSequence(const CGUISize& aBeginValue, const CGUISize& aEndValue, real fTotalTime );

		/**
		* @brief Update the event.
		*/
		virtual void Update( real fDeltaTime );

	protected:
		CGUISize m_aBeginValue;
		CGUISize m_aEndValue;
	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsScale);


	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	/**
	* @class CGUIAsRotation
	* @brief the as, change rotation of widget
	*/
	class GUIEXPORT CGUIAsRotation : public CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsRotation();

		/** 
		* @brief set the parameter of action sequence
		*/
		void SetRotationSequence(const CGUIVector3& rBeginValue, const CGUIVector3& rEndValue, real fTotalTime);

		/**
		* @brief Update the event.
		*/
		virtual void Update( real fDeltaTime );

	protected:
		CGUIVector3	m_vBeginValue;
		CGUIVector3	m_vEndValue;
	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsRotation);

	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	/**
	* @class CGUIAsPosition
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsPosition : public CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsPosition();

		/** 
		* @brief set the parameter of action sequence
		*/
		void SetPositionSequence(const CGUIVector2& aBeginValue, const CGUIVector2& aEndValue, real fTotalTime );

		/**
		* @brief Update the event.
		*/
		virtual void Update( real fDeltaTime );

	protected:
		CGUIVector2	m_aBeginValue;
		CGUIVector2	m_aEndValue;

	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsPosition);

}//namespace guiex

#endif //__GUI_AS_20071121_H__


