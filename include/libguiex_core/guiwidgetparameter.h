/** 
* @file guiwidgetparameter.h
* @brief for accelerate the speed of ui system, 
* make a fast way to get parameter of widget
* @author ken
* @date 2007-05-10
*/

#ifndef __GUI_WIDGET_20070510_H__
#define __GUI_WIDGET_20070510_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** 
	 * @class CGUIWidgetParamBase
	 * @brief a quick way to get widget's parameter,
	 * <the final value> equal <base value> * <parent's final value>
	 */
	template <typename T>
	class	CGUIWidgetParamBase
	{
	public:
		CGUIWidgetParamBase(const T& aSelfValue);
		virtual ~CGUIWidgetParamBase();

		void				SetSelfValue( const T& aSelfValue);
		const T &			GetSelfValue( ) const;
		virtual const T &	GetFinalValue( ) = 0;

		void				SetDirtyFlag();

		void				AddChild(CGUIWidgetParamBase<T>* pParam);
		void				RemoveChild(CGUIWidgetParamBase<T>* pParam);

	protected:
		void				ResetDirtyFlag()
		{
			m_bDirtyFlag = false;
		}

	protected:
		T			m_aSelfValue;		//base value of this parameter
		T			m_aFinalValue;		//final value of this parameter
		bool		m_bDirtyFlag;		//dirty flag

		CGUIWidgetParamBase<T>*	m_pParent;
		CGUIWidgetParamBase<T>*	m_pChild;
		CGUIWidgetParamBase<T>*	m_pNextSibling;
	};



	/** 
	* @class CGUIWidgetParamMultiply
	* @brief a quick way to get widget's parameter,
	* the operator is multiply,
	* <the final value> equal <base value> * <parent's final value>
	*/
	template <typename T>
	class	CGUIWidgetParamMultiply : public CGUIWidgetParamBase<T>
	{
	public:
		CGUIWidgetParamMultiply(const T& aSelfValue);

		virtual const T &	GetFinalValue( );
	};



	/** 
	* @class CGUIWidgetParamOR
	* @brief a quick way to get widget's parameter,
	* the operator is OR,
	* <the final value> equal <base value> * <parent's final value>
	*/
	template <typename T>
	class	CGUIWidgetParamOR : public CGUIWidgetParamBase<T>
	{
	public:
		CGUIWidgetParamOR(const T& aSelfValue);

		virtual const T &	GetFinalValue( );
	};


	/** 
	* @class CGUIWidgetParamAND
	* @brief a quick way to get widget's parameter,
	* the operator is AND,
	* <the final value> equal <base value> * <parent's final value>
	*/
	template <typename T>
	class	CGUIWidgetParamAND : public CGUIWidgetParamBase<T>
	{
	public:
		CGUIWidgetParamAND(const T& aSelfValue);

		virtual const T &	GetFinalValue( );
	};
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	template <typename T>
	inline CGUIWidgetParamBase<T>::CGUIWidgetParamBase(const T& aSelfValue)
	:m_pParent(NULL)
	,m_pChild(NULL)
	,m_pNextSibling(NULL)
	,m_bDirtyFlag(true)
	,m_aSelfValue(aSelfValue)
	{

	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline CGUIWidgetParamBase<T>::~CGUIWidgetParamBase()
	{

	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline void		CGUIWidgetParamBase<T>::SetSelfValue( const T& aSelfValue)
	{
		m_aSelfValue = aSelfValue;
		SetDirtyFlag();
	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline const T &	CGUIWidgetParamBase<T>::GetSelfValue( ) const
	{
		return m_aSelfValue;
	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline void		CGUIWidgetParamBase<T>::SetDirtyFlag()
	{
		if( !m_bDirtyFlag)
		{
			m_bDirtyFlag = true;

			CGUIWidgetParamBase<T>* pParam = m_pChild;
			while( pParam )
			{
				pParam->SetDirtyFlag();
				pParam = pParam->m_pNextSibling;
			}
		}
	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline void		CGUIWidgetParamBase<T>::AddChild(CGUIWidgetParamBase<T>* pChild)
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->m_pParent == NULL, "the child has haven a parent!");

		pChild->m_pNextSibling = m_pChild;
		m_pChild = pChild;

		pChild->m_pParent = this;
		pChild->SetDirtyFlag();
	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline void		CGUIWidgetParamBase<T>::RemoveChild(CGUIWidgetParamBase<T>* pChild)
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->m_pParent == this, "the child's parent isn't this!");

		if( pChild == m_pChild )
		{
			m_pChild = m_pChild->m_pNextSibling;
		}
		else
		{
			bool bFind = false;
			CGUIWidgetParamBase<T>* pParam = m_pChild;
			while(pParam && pParam->m_pNextSibling)
			{
				if( pParam->m_pNextSibling == pChild )
				{
					pParam->m_pNextSibling = pParam->m_pNextSibling->m_pNextSibling;
					bFind = true;
					break;
				}
				else
				{
					pParam = pParam->m_pNextSibling;
				}
			}
			GUI_ASSERT(bFind, "failed to find this child");
		}


		pChild->m_pParent = NULL;
		pChild->m_pNextSibling = NULL;
		pChild->SetDirtyFlag();
	}
	//------------------------------------------------------------------------------





	//------------------------------------------------------------------------------
	template <typename T>
	inline CGUIWidgetParamMultiply<T>::CGUIWidgetParamMultiply(const T& aSelfValue)
		:CGUIWidgetParamBase<T>(aSelfValue)
	{

	}
	//------------------------------------------------------------------------------
	template <typename T>
	inline const T &	CGUIWidgetParamMultiply<T>::GetFinalValue( )
	{
		if( this->m_bDirtyFlag)
		{
			//dirty
			if( this->m_pParent)
			{
				this->m_aFinalValue = this->m_pParent->GetFinalValue() * this->m_aSelfValue;
			}
			else
			{
				this->m_aFinalValue = this->m_aSelfValue;
			}
			this->ResetDirtyFlag();
		}
		return this->m_aFinalValue;
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	template <class T>
	inline CGUIWidgetParamOR<T>::CGUIWidgetParamOR(const T& aSelfValue)
		:CGUIWidgetParamBase<T>(aSelfValue)
	{

	}
	//------------------------------------------------------------------------------
	template <class T>
	inline const T &	CGUIWidgetParamOR<T>::GetFinalValue( )
	{
		if( this->m_bDirtyFlag)
		{
			//dirty
			if( this->m_pParent)
			{
				this->m_aFinalValue = (this->m_pParent->GetFinalValue() || this->m_aSelfValue);
			}
			else
			{
				this->m_aFinalValue = this->m_aSelfValue;
			}
			this->ResetDirtyFlag();
		}
		return this->m_aFinalValue;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template <class T>
	inline CGUIWidgetParamAND<T>::CGUIWidgetParamAND(const T& aSelfValue)
		:CGUIWidgetParamBase<T>(aSelfValue)
	{

	}
	//------------------------------------------------------------------------------
	template <class T>
	inline const T &	CGUIWidgetParamAND<T>::GetFinalValue( )
	{
		if( this->m_bDirtyFlag)
		{
			//dirty
			if( this->m_pParent)
			{
				this->m_aFinalValue = (this->m_pParent->GetFinalValue() && this->m_aSelfValue);
			}
			else
			{
				this->m_aFinalValue = this->m_aSelfValue;
			}
			this->ResetDirtyFlag();
		}
		return this->m_aFinalValue;
	}
	//------------------------------------------------------------------------------
}

#endif
