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
	* called by CGUISystem
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
		virtual ~CGUIAs();

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		virtual CGUIAs* Clone( ) const;

		const CGUIString& GetType() const;

		void Retire( bool bRetired );
		bool IsRetired();

		void SetLooping( bool bLooping );
		bool IsLooping();

		virtual void SetReceiver(CGUIWidget* pReceiver);
		CGUIWidget*	GetReceiver();

		void SetTotalTime( real fTotalTime );
		real GetTotalTime( ) const;

		void SetElapsedTime( real fElapsedTime );
		real GetElapsedTime( ) const;

		void PushSuccessor( CGUIAs* pAs);
		CGUIAs*	PopSuccessor();

		virtual real Update( real fDeltaTime );

		///is this as created by CGUIAsFactory
		bool IsCreateByFactory() const
		{
			return m_pAsGenerator!=NULL;
		}

	protected:
		CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );

	private:
		friend class CGUIAsManager;
		//disable =
		CGUIAs( const CGUIAs& );
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
		real m_fTotalTime; //!<delay-time, to control when to process this as, millisecond
		real m_fElapsedTime;			
		bool m_bLooping;
		bool m_bRetired; //!<should this as be retired
		CGUIWidget*	m_pReceiver; //!<receiver

		typedef std::list<CGUIAs*> TListSuccessor;
		TListSuccessor m_listSuccessor; //!<successor

		CGUIString m_strAsType; //!<type of this as
	private:
		const CGUIAsGenerator* m_pAsGenerator;	//!<generator which used to create as
	};


	//*****************************************************************************
	//	CGUIInterpolationBase
	//*****************************************************************************
	template< class T >
	class CGUIInterpolationBase : public CGUIAs
	{
	protected:
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

		void SetCurrentValue( const T& rValue )
		{
			m_aCurValue = rValue;
		}

		const T& GetCurrentValue() const
		{
			return m_aCurValue;
		}

		virtual CGUIAs* Clone( ) const
		{
			CGUIInterpolationBase<T>* pCloneAs = static_cast< CGUIInterpolationBase<T>* >(CGUIAs::Clone());
			pCloneAs->SetBeginValue( m_aBeginValue );
			pCloneAs->SetEndValue( m_aEndValue );
			pCloneAs->SetCurrentValue( m_aCurValue );

			return pCloneAs;
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
		CGUIAsAlpha(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
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
		CGUIAsScale(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
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
		CGUIAsRotation(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
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
		CGUIAsPosition(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
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
		CGUIAsColor(const CGUIString& rAsName, const CGUIString& rSceneName);

	public:
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


	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	class GUIEXPORT CGUIAsContainer : public CGUIAs
	{
	protected:

		CGUIAsContainer(const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsContainer( );

	public:
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual CGUIAs* Clone( ) const;

		virtual real Update( real fDeltaTime );

		virtual void SetReceiver(CGUIWidget* pReceiver);

		void AddItem( CGUIAsContainItemInfo& rItemInfo );
		void AddItem( CGUIAs* pAs, real fBeginTime );

	private:
		typedef std::vector<CGUIAsContainItemInfo> TAsList;
		TAsList m_vAsList;//the as in this list should be sorted by begin time

		GUI_AS_GENERATOR_DECLARE( CGUIAsContainer);
	};

}//namespace guiex

#endif //__GUI_AS_20071121_H__


