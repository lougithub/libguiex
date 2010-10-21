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
		CGUIAs(const char* pAsName, CGUIWidget* pReceiver);

		/**
		* @brief destructor
		*/
		virtual ~CGUIAs();

		/**
		* @brief get as name
		*/
		const CGUIString&		GetAsName() const;

		/**
		* @brief set the flag after this as has been processed.
		*/
		void	Retire( bool bRetired );

		/**
		* @brief whether the as is retired.
		*/
		bool	IsRetired();

		/**
		* @brief set widget which receives this as
		*/
		void	SetReceiver(CGUIWidget* pReceiver);

		/**
		* @brief Get widget which receives this as
		*/
		CGUIWidget*	GetReceiver();

		/**
		* @brief set delay time
		*/
		void	SetDelayTime(uint32 nDelayTime);

		/**
		* @brief get delay time
		*/
		uint32	GetDelayTime( ) const;

		/**
		* @brief update timer
		*/
		void	UpdateTimer();

		/**
		* @brief get timer
		*/
		const CGUITimer & GetTimer() const;


		/**
		* @brief add a successor as
		*/
		void		PushSuccessor( CGUIAs* pAs);

		/**
		* @brief get successor, 
		* @return NULL if there isn't any successor
		*/
		CGUIAs*	PopSuccessor();

		/**
		* @brief process the as
		*/
		virtual uint32	Process() = 0;

		///is this as created by CGUIAsFactory
		bool	IsCreateByFactory() const
		{
			return m_pAsGenerator!=NULL;
		}

	protected:
		///widget generator
		friend class CGUIAsFactory;
		//!<set generator
		void	SetGenerator( const CGUIAsGenerator* pGenerator);
		//!<get generator
		const CGUIAsGenerator* GetGenerator() const;

	private:
		
		CGUIString	m_strAsName;			//!<name of this as
		uint32		m_nDelayTime;			//!<delay-time, to control when to process this as, millisecond
		CGUITimer	m_aTimer;				//!<timer, used for delay time
		bool		m_bRetired;				//!<should this as be retired
		CGUIWidget*	m_pReceiver;			//!<receiver
		
		typedef std::list<CGUIAs*> TListSuccessor;
		TListSuccessor m_listSuccessor;		//!<successor

	private:
		const CGUIAsGenerator*	m_pAsGenerator;	//!<generator which used to create as
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
		CGUIAsAlpha(CGUIWidget* pReceiver);

		/** 
		 * @brief set the parameter of action sequence
		 */
		void	SetAlphaSequence(real fStartValue, real fEndValue, uint32 nFrames, uint32 nDelayTime);

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

	protected:
		real	m_fStepValue;
		real	m_fCurValue;

		uint32	m_nFrames;
		uint32	m_nCurFrames;
		uint32  m_nDelaytime;
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
		CGUIAsScale(CGUIWidget* pReceiver);

		/** 
		* @brief set the parameter of action sequence
		*/
		void	SetScaleSequence(const CGUISize& fStartScale, const CGUISize& fEndScale, uint32 nFrames, uint32 nDelayTime);

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

	protected:
		CGUISize	m_fStepValue;
		CGUISize	m_fCurValue;

		uint32	m_nFrames;
		uint32	m_nCurFrames;
		uint32  m_nDelaytime;
	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsScale);



	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	/**
	* @class CGUIAsScale
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsPosition : public CGUIAs
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIAsPosition(CGUIWidget* pReceiver);

		/** 
		* @brief set the parameter of action sequence
		*/
		void	SetPositionSequence(const CGUIVector2& fStartPos, const CGUIVector2& fEndPos, uint32 nFrames, uint32 nDelayTime);

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

	protected:
		CGUIVector2	m_fStepValue;
		CGUIVector2	m_fCurValue;

		uint32	m_nFrames;
		uint32	m_nCurFrames;
		uint32  m_nDelaytime;
	};
	GUI_AS_GENERATOR_DECLARE( CGUIAsPosition);

}//namespace guiex






#endif //__GUI_AS_20071121_H__


