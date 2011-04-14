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

#include "guiresource.h"

#include "guiproperty.h"
#include "guipropertyconvertor.h"

#include "guitimer.h"
#include "guisize.h"
#include "guimath.h"
#include "guiinterpolation.h"

#include <vector>

//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIWidget;

	typedef void (*FunOnAsCallback)(CGUIAs*);
}

#define GUI_AS_GENERATOR_DECLARE(asType) \
public:	\
	friend class CGUIAsManager; \
	static CGUIAs* GenerateAs( const CGUIString& rAsName, const CGUIString& rSceneName ) \
	{ \
		return new asType( rAsName, rSceneName ); \
	} \
	static const char* StaticGetType() \
	{ \
		return #asType; \
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

		const CGUIString& GetType() const;

		virtual void Reset( );

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

		real GetPercent() const;

		void AddSuccessor( CGUIAs* pAs);
		uint32 GetSuccessorNum( ) const;
		CGUIAs*	GetSuccessor( uint32 nIndex );
		void RemoveSuccessor( uint32 nIndex );
		void ClearSuccessor( );

		virtual real Update( real fDeltaTime );

	protected:
		friend class CGUIAsManager;
		CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );
		
		virtual void OnUpdate();
		virtual void OnRetired();
		virtual void OnDestory();

	private:
		//disable =
		CGUIAs( const CGUIAs& );
		const CGUIAs& operator=(const CGUIAs& );

	protected:
		virtual int32 DoLoad();
		virtual void DoUnload();

	private:
		real m_fTotalTime; //!<delay-time, to control when to process this as, millisecond
		real m_fElapsedTime;			
		bool m_bLooping;
		bool m_bRetired; //!<should this as be retired
		CGUIWidget*	m_pReceiver; //!<receiver

		typedef std::vector<CGUIAs*> TArraySuccessor;
		TArraySuccessor m_arraySuccessor; //!<successor

		CGUIString m_strAsType; //!<type of this as
	};


	//*****************************************************************************
	//	CGUICustomAs
	//*****************************************************************************
	class CGUICustomAs : public CGUIAs
	{
	public:
		virtual void RefRelease();

	protected:
		CGUICustomAs( const CGUIString& rAsType );
		virtual void DestroySelf();
	};

	//*****************************************************************************
	//	CGUIAsInterpolation
	//*****************************************************************************
	template< class T >
	class CGUIAsInterpolation : public CGUIAs
	{
	protected:
		CGUIAsInterpolation( const CGUIString& rAsName, const CGUIString& rSceneName );
		CGUIAsInterpolation( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );

	public:
		virtual void Reset( );

		EInterpolationType GetInterpolationType( ) const;
		void SetInterpolationType( EInterpolationType eType );

		void SetInterpolationValue( const T& rBeginValue, const T& rEndValue, real fTotalTime );

		void SetBeginValue( const T& rValue );
		const T& GetBeginValue() const;
		void SetEndValue( const T& rValue );
		const T& GetEndValue() const;
		void SetCurrentValue( const T& rValue );
		const T& GetCurrentValue() const;
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

	protected:
		virtual void OnUpdate();

	private:
		EInterpolationType	m_eInterpolationType;

		T m_aBeginValue;
		T m_aEndValue;

		T m_aCurValue;

		GUI_AS_GENERATOR_DECLARE( CGUIAsInterpolation );
	};

	//*****************************************************************************
	//	CGUIAsInterpolationQueue
	//*****************************************************************************
	template< class T >
	class CGUIAsInterpolationQueue : public CGUIAs
	{
	protected:
		CGUIAsInterpolationQueue(const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsInterpolationQueue( );

	public:
		virtual void Reset( );

		void AddItem( CGUIAsInterpolation<T>* pAs );
		const T& GetCurrentValue() const;

	protected:
		virtual void OnUpdate();

	private:
		class CGUIQueueItemInfo
		{
		public:
			CGUIAsInterpolation<T>* m_pAs;
			real m_fBeginTime;
			CGUIQueueItemInfo()
				:m_fBeginTime( 0.0f )
				,m_pAs(NULL)
			{
			}
		};
		typedef std::vector<CGUIQueueItemInfo> TAsQueue;
		TAsQueue m_vAsQueue;

		T m_aCurValue;

		GUI_AS_GENERATOR_DECLARE( CGUIAsInterpolationQueue );
	};


	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	/**
	* @class CGUIAsAlpha
	* @brief the as, change alpha of widget
	*/
	class GUIEXPORT CGUIAsAlpha : public CGUIAsInterpolation<real>
	{
	protected:
		CGUIAsAlpha(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnUpdate();

		GUI_AS_GENERATOR_DECLARE( CGUIAsAlpha);
	};


	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	/**
	* @class CGUIAsScale
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsScale : public CGUIAsInterpolation<CGUISize>
	{
	protected:
		CGUIAsScale(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnUpdate();

		GUI_AS_GENERATOR_DECLARE( CGUIAsScale);
	};


	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	/**
	* @class CGUIAsRotation
	* @brief the as, change rotation of widget
	*/
	class GUIEXPORT CGUIAsRotation : public CGUIAsInterpolation<CGUIVector3>
	{
	protected:
		CGUIAsRotation(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnUpdate();

		GUI_AS_GENERATOR_DECLARE( CGUIAsRotation);
	};


	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	/**
	* @class CGUIAsPosition
	* @brief the as, change scale of widget
	*/
	class GUIEXPORT CGUIAsPosition : public CGUIAsInterpolation<CGUIVector2>
	{
	protected:
		CGUIAsPosition(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnUpdate();

		GUI_AS_GENERATOR_DECLARE( CGUIAsPosition);
	};


	//*****************************************************************************
	//	CGUIAsColor
	//*****************************************************************************
	/**
	* @class CGUIAsColor
	* @brief the as, change color of widget
	*/
	class GUIEXPORT CGUIAsColor : public CGUIAsInterpolation<CGUIColor>
	{
	protected:
		CGUIAsColor(const CGUIString& rAsName, const CGUIString& rSceneName);

	protected:
		virtual void OnUpdate();

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
		virtual void Reset( );

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		virtual void SetReceiver(CGUIWidget* pReceiver);

		void AddItem( CGUIAsContainItemInfo& rItemInfo );
		void AddItem( CGUIAs* pAs, real fBeginTime );

	protected:
		virtual void OnUpdate();

	private:
		typedef std::vector<CGUIAsContainItemInfo> TAsList;
		TAsList m_vAsList;//the as in this list should be sorted by begin time

		GUI_AS_GENERATOR_DECLARE( CGUIAsContainer);
	};

}//namespace guiex




//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CGUIAsInterpolation
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolation<T>::CGUIAsInterpolation( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAs( "CGUIAsInterpolation",  rAsName, rSceneName )
		,m_eInterpolationType( eInterpolationType_Linear )
	{

	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolation<T>::CGUIAsInterpolation( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAs( rAsType,  rAsName, rSceneName )
		,m_eInterpolationType( eInterpolationType_Linear )
	{
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::Reset( )
	{
		CGUIAs::Reset();

		m_aCurValue = m_aBeginValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline EInterpolationType CGUIAsInterpolation<T>::GetInterpolationType( ) const
	{
		return m_eInterpolationType;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::SetInterpolationType( EInterpolationType eType )
	{
		m_eInterpolationType = eType;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::SetInterpolationValue( const T& rBeginValue, const T& rEndValue, real fTotalTime )
	{
		m_aBeginValue = rBeginValue;
		m_aEndValue = rEndValue;
		m_aCurValue = m_aBeginValue;
		SetTotalTime( fTotalTime );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::OnUpdate( )
	{
		CGUIAs::OnUpdate( );
		m_aCurValue = LinearTween( GetElapsedTime() / GetTotalTime(), m_aBeginValue, m_aEndValue );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::SetBeginValue( const T& rValue )
	{
		m_aBeginValue = rValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsInterpolation<T>::GetBeginValue() const
	{
		return m_aBeginValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::SetEndValue( const T& rValue )
	{
		m_aEndValue = rValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsInterpolation<T>::GetEndValue() const
	{
		return m_aEndValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolation<T>::SetCurrentValue( const T& rValue )
	{
		m_aCurValue = rValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsInterpolation<T>::GetCurrentValue() const
	{
		return m_aCurValue;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline int32 CGUIAsInterpolation<T>::ProcessProperty( const CGUIProperty& rProperty )
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
			const CGUIProperty* pPPtInterpolationType = rProperty.GetProperty("interpolation", "EInterpolationType" );
			if( pPPtInterpolationType )
			{
				PropertyToValue( *pPPtInterpolationType, eValue );
			}
			SetInterpolationType( eValue );
		}

		{
			const CGUIProperty* pPPtBeginValue = rProperty.GetProperty("begin_value", "");
			if( !pPPtBeginValue )
			{
				throw CGUIException(
					"[CGUIAsInterpolation::ProcessProperty]: invalid property: <%s> <%s>", 
					rProperty.GetName().c_str(), 
					rProperty.GetTypeAsString().c_str());
				return -1;
			}
			T aBeginValue;
			PropertyToValue( *pPPtBeginValue, aBeginValue );
			SetBeginValue( aBeginValue );
		}

		{
			const CGUIProperty* pPPtEndValue = rProperty.GetProperty("end_value","");
			if( !pPPtEndValue )
			{
				throw CGUIException(
					"[CGUIAsInterpolation::ProcessProperty]: invalid property: <%s> <%s>", 
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

	//------------------------------------------------------------------------------
	template< class T >
	inline int32 CGUIAsInterpolation<T>::GenerateProperty( CGUIProperty& rProperty )
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
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CGUIAsInterpolationQueue
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolationQueue<T>::CGUIAsInterpolationQueue( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAs("CGUIAsInterpolationQueue", rAsName, rSceneName)
	{
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolationQueue<T>::~CGUIAsInterpolationQueue( )
	{
		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			(*itor).m_pAs->RefRelease();
		}
		m_vAsQueue.clear();
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::Reset( )
	{
		CGUIAs::Reset();
		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			(*itor).m_pAs->Reset();
		}
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::OnUpdate( )
	{
		CGUIAs::OnUpdate( );

		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			CGUIQueueItemInfo &rInfo = *itor;
			if( rInfo.m_fBeginTime > GetElapsedTime() )
			{
				//update done
				break;
			}

			if( rInfo.m_fBeginTime + rInfo.m_pAs->GetTotalTime() < GetElapsedTime() )
			{
				continue;
			}

			rInfo.m_pAs->SetElapsedTime( GetElapsedTime() - rInfo.m_fBeginTime );
			rInfo.m_pAs->Update( 0.0f );
			m_aCurValue = rInfo.m_pAs->GetCurrentValue();
		}
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::AddItem( CGUIAsInterpolation<T>* pAs )
	{
		CGUIQueueItemInfo aNewInfo;
		aNewInfo.m_pAs = pAs;

		//retain
		aNewInfo.m_pAs->RefRetain();

		if( m_vAsQueue.empty() )
		{
			aNewInfo.m_fBeginTime = 0.0f;
			SetTotalTime( pAs->GetTotalTime() );
		}
		else
		{
			aNewInfo.m_fBeginTime = GetTotalTime();
			SetTotalTime( GetTotalTime() + pAs->GetTotalTime() );
		}

		//insert
		m_vAsQueue.push_back( aNewInfo );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsInterpolationQueue<T>::GetCurrentValue() const
	{
		return m_aCurValue;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsCallFunc
	//*****************************************************************************
	/**
	* @class CGUIAsCallFunc
	*/
	class GUIEXPORT CGUIAsCallFunc : public CGUIAs
	{
	public:
		void SetFuncCallback( FunOnAsCallback funCallback );

		virtual real Update( real fDeltaTime );

	protected:
		CGUIAsCallFunc(const CGUIString& rAsName, const CGUIString& rSceneName);

	private:
		FunOnAsCallback m_funCallback;

		GUI_AS_GENERATOR_DECLARE( CGUIAsCallFunc);
	};

	//*****************************************************************************
	//	CGUIAsMoveTo
	//*****************************************************************************
	/**
	* @class CGUIAsMoveTo
	* @brief move widget to destination with given velocity.
	*/
	class GUIEXPORT CGUIAsMoveTo : public CGUIAs
	{
	public:
		void SetVelocity( real fVelocity );
		real GetVelocity( ) const;
		void SetDestination( const CGUIVector2& rDestination );
		const CGUIVector2& GetDestination( ) const;

		virtual real Update( real fDeltaTime );

	protected:
		CGUIAsMoveTo(const CGUIString& rAsName, const CGUIString& rSceneName);

	private:
		real m_fVelocity;
		CGUIVector2 m_aDestination;

		GUI_AS_GENERATOR_DECLARE( CGUIAsMoveTo);
	};
}

#endif //__GUI_AS_20071121_H__


