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
#include "guicolor.h"

#include "guiasgenerator.h"
#include "guiasfactory.h"

#include "guiresource.h"

#include "guiproperty.h"
#include "guipropertyconvertor.h"

#include "guitimer.h"
#include "guisize.h"
#include "guimath.h"
#include "guiinterpolation.h"

#include <list>
#include <vector>


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
	class GUIEXPORT CGUIAs  : public CGUIResource
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUIAs();

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		/**
		* @brief get as type
		*/
		const CGUIString& GetType() const;

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
		virtual void SetReceiver(CGUIWidget* pReceiver);

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
		* @brief set delay time
		*/
		void SetElapsedTime( real fElapsedTime );

		/**
		* @brief get elapsed time
		*/
		real GetElapsedTime( ) const;

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
		* @return the left time that is not processed
		*/
		virtual real Update( real fDeltaTime );

		///is this as created by CGUIAsFactory
		bool IsCreateByFactory() const
		{
			return m_pAsGenerator!=NULL;
		}

	protected:
		/**
		* @brief constructor
		*/
		CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );
	
	private:
		friend class CGUIAsManager;
		//disable =
		CGUIAs( const CGUIAs&  );
		const CGUIAs& operator=(const CGUIAs& );

	protected:
		///widget generator
		friend class CGUIAsFactory;
		//!<set generator
		void SetGenerator( const CGUIAsGenerator* pGenerator);
		//!<get generator
		const CGUIAsGenerator* GetGenerator() const;

		virtual int32	DoLoad() const;
		virtual void	DoUnload();

	private:
		real m_fTotalTime;			//!<delay-time, to control when to process this as, millisecond
		real m_fElapsedTime;			

	private:		
		CGUIString m_strAsType;			//!<type of this as
		bool m_bRetired;				//!<should this as be retired
		bool m_bLooping;
		CGUIWidget*	m_pReceiver;		//!<receiver
		
		typedef std::list<CGUIAs*> TListSuccessor;
		TListSuccessor m_listSuccessor;		//!<successor

	private:
		const CGUIAsGenerator* m_pAsGenerator;	//!<generator which used to create as
	};




	template< class T >
	class CGUIInterpolationBase : public CGUIAs
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIInterpolationBase( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
			:CGUIAs( rAsType,  rAsType, rSceneName )
			,m_eInterpolationType( eInterpolationType_Linear )
		{
		}

	public:
		EInterpolationType GetInterpolationType( ) const
		{
			return m_eInterpolationType;
		}
		void SetInterpolationType( EInterpolationType eType )
		{
			m_eInterpolationType = eType;
		}

		void SetInterpolationValue( const T& rBeginValue, const T& rEndValue, real fTotalTime )
		{
			m_aBeginValue = rBeginValue;
			m_aEndValue = rEndValue;
			m_aCurValue = m_aBeginValue;
			SetTotalTime( fTotalTime );
		}

		virtual real Update( real fDeltaTime )
		{
			real fLeftTime = CGUIAs::Update( fDeltaTime );
			m_aCurValue = LinearTween( GetElapsedTime() / GetTotalTime(), m_aBeginValue, m_aEndValue );
			return fLeftTime;
		}

		void SetBeginValue( const T& rValue )
		{
			m_aBeginValue = rValue;
		}

		const T& GetBeginValue() const
		{
			return m_aBeginValue;
		}

		void SetEndValue( const T& rValue )
		{
			m_aEndValue = rValue;
		}

		const T& GetEndValue() const
		{
			return m_aEndValue;
		}

		const T& GetCurrentValue() const
		{
			return m_aCurValue;
		}

		virtual int32 ProcessProperty( const CGUIProperty& rProperty )
		{
			/*
			*<property name="asname" type="CGUIAsColor">
			*		<property name="loop" type="bool" value="false"/>
			*		<property name="total_time" type="real" value="10" />
			*		<property name="elapsed_time" type="real" value="5" />
			*		<property name="interpolation" type="EInterpolationType" value="0,0,0" />
			*		<property name="begin_value" type="CGUIVector3" value="0,0,0" />
			*		<property name="end_value" type="CGUIVector3" value="0,0,0" />
			*</property>
			*/
			int32 ret = CGUIAs::ProcessProperty( rProperty );
			if( ret != 0 )
			{
				return ret;
			}

			{
				EInterpolationType eValue = eInterpolationType_Linear;
				const CGUIProperty* pPPtInterpolationType = rProperty.GetProperty("interpolation");
				if( pPPtInterpolationType )
				{
					PropertyToValue( *pPPtInterpolationType, eValue );
				}
				SetInterpolationType( eValue );
			}

			{
				const CGUIProperty* pPPtBeginValue = rProperty.GetProperty("begin_value");
				if( !pPPtBeginValue )
				{
					throw CGUIException(
						"[CGUIInterpolationBase::ProcessProperty]: invalid property: <%s> <%s>", 
						rProperty.GetName().c_str(), 
						rProperty.GetTypeAsString().c_str());
					return -1;
				}
				T aBeginValue;
				PropertyToValue( *pPPtBeginValue, aBeginValue );
				SetBeginValue( aBeginValue );
			}

			{
				const CGUIProperty* pPPtEndValue = rProperty.GetProperty("end_value");
				if( !pPPtEndValue )
				{
					throw CGUIException(
						"[CGUIInterpolationBase::ProcessProperty]: invalid property: <%s> <%s>", 
						rProperty.GetName().c_str(), 
						rProperty.GetTypeAsString().c_str());
					return -1;
				}
				T aEndValue;
				PropertyToValue( *pPPtEndValue, aEndValue );
				SetEndValue( aEndValue );
			}

			return 0;
		}

		virtual int32 GenerateProperty( CGUIProperty& rProperty )
		{
			/*
			*<property name="asname" type="CGUIAsColor">
			*		<property name="loop" type="bool" value="false"/>
			*		<property name="total_time" type="real" value="10" />
			*		<property name="elapsed_time" type="real" value="5" />
			*
			*		<property name="interpolation" type="EInterpolationType" value="0,0,0" />
			*		<property name="begin_value" type="CGUIVector3" value="0,0,0" />
			*		<property name="end_value" type="CGUIVector3" value="0,0,0" />
			*</property>
			*/
			int32 ret = CGUIAs::GenerateProperty( rProperty );
			if( ret != 0 )
			{
				return ret;
			}

			{
				CGUIProperty aProperty( "interpolation", "EInterpolationType" );
				ValueToProperty( GetInterpolationType(), aProperty );
				rProperty.AddProperty( aProperty );
			}

			{
				CGUIProperty aProperty( "begin_value", GetValueType<T>() );
				ValueToProperty( GetBeginValue(), aProperty );
				rProperty.AddProperty( aProperty );
			}

			{
				CGUIProperty aProperty( "end_value", GetValueType<T>() );
				ValueToProperty( GetEndValue(), aProperty );
				rProperty.AddProperty( aProperty );
			}
			return 0;
		}

	private:
		EInterpolationType	m_eInterpolationType;

		T m_aBeginValue;
		T m_aEndValue;

		T m_aCurValue;
	};


	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	/**
	* @class CGUIAsAlpha
	* @brief the as, change alpha of widget
	*/
	class GUIEXPORT CGUIAsAlpha : public CGUIInterpolationBase<real>
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsAlpha(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
		/**
		* @brief update the event.
		*/
		virtual real Update( real fDeltaTime );

		GUI_AS_GENERATOR_DECLARE( CGUIAsAlpha);
	};




	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	/**
	* @class CGUIAsScale
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsScale : public CGUIInterpolationBase<CGUISize>
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsScale(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
		/**
		* @brief Update the event.
		*/
		virtual real Update( real fDeltaTime );

		GUI_AS_GENERATOR_DECLARE( CGUIAsScale);
	};


	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	/**
	* @class CGUIAsRotation
	* @brief the as, change rotation of widget
	*/
	class GUIEXPORT CGUIAsRotation : public CGUIInterpolationBase<CGUIVector3>
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsRotation(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
		/**
		* @brief Update the event.
		*/
		virtual real Update( real fDeltaTime );

		GUI_AS_GENERATOR_DECLARE( CGUIAsRotation);
	};

	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	/**
	* @class CGUIAsPosition
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsPosition : public CGUIInterpolationBase<CGUIVector2>
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsPosition(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
		/**
		* @brief Update the event.
		*/
		virtual real Update( real fDeltaTime );

		GUI_AS_GENERATOR_DECLARE( CGUIAsPosition);
	};


	//*****************************************************************************
	//	CGUIAsColor
	//*****************************************************************************
	/**
	* @class CGUIAsColor
	* @brief the as, change color of widget
	*/
	class GUIEXPORT CGUIAsColor : public CGUIInterpolationBase<CGUIColor>
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsColor(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
		/**
		* @brief Update the event.
		*/
		virtual real Update( real fDeltaTime );

		GUI_AS_GENERATOR_DECLARE( CGUIAsColor);
	};



	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	
	class CGUIAsContainItemInfo
	{
	public:
		CGUIAs* m_pAs;
		real m_fBeginTime;

		CGUIAsContainItemInfo()
			:m_pAs( NULL )
			,m_fBeginTime( 0.0f )
		{

		}
	};
	/**
	* @class CGUIAsContainer
	* @brief maintain several as.
	*/
	class GUIEXPORT CGUIAsContainer : public CGUIAs
	{
	protected:
		/**
		* @brief constructor
		*/
		CGUIAsContainer(const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsContainer( );

	public:
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		/**
		* @brief Update the event.
		*/
		virtual real Update( real fDeltaTime );

		virtual void SetReceiver(CGUIWidget* pReceiver);

		void AddItem( CGUIAsContainItemInfo& rItemInfo );
		void AddItem( CGUIAs* pAs, real fBeginTime );
		
	protected:
		typedef std::vector<CGUIAsContainItemInfo> TAsList;
		TAsList m_vAsList;//the as in this list should be sorted by begin time
		
		GUI_AS_GENERATOR_DECLARE( CGUIAsContainer);
	};

}//namespace guiex

#endif //__GUI_AS_20071121_H__


