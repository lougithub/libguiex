/** 
* @file PerfMonitor.cpp
* @brief thanks for yu jian rong
* @author ken
* @date 2004-10-28
*/

/*******************************************
* PerfMon
*   Performance monitor
*******************************************/


//====================================================================================//
// include
//====================================================================================// 
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guitimer.h>
#include <string>


#if GUI_PERFORMANCE_ON

//====================================================================================//
// class
//====================================================================================// 
namespace guiex
{
	class CPerfMon_impl
	{
	public:
		CPerfMon_impl( int32 nFrameCnt, int32 nSectionNum );

		~CPerfMon_impl();

		void FrameUpdate();

		bool IsUpdated()
		{
			return m_frameindex == 0;
		}

		real GetFPS()
		{
			return m_fRecentFPS;
		}

		void BeginSection(int32 nSectionNo,const char* pName)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			m_pSectionBegin[nSectionNo].UpdateTime();
			++ m_pSectionTimes[nSectionNo];
			if( pName )
			{
				strncpy( m_pSectionName[nSectionNo], pName, 11 );
			}
		}

		void EndSection(int32 nSectionNo)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			m_pGlobalTimer->UpdateTime();

			m_pSectionCnt[nSectionNo] += (*m_pGlobalTimer - m_pSectionBegin[nSectionNo]);
		}

		real GetRate(int32 nSectionNo)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			return m_pSectionRate[nSectionNo];
		}

		unsigned GetMillionsec(int32 nSectionNo)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			return m_pSectionMillionsec[nSectionNo];
		}

		unsigned GetTimes(int32 nSectionNo)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			return m_pSectionLastTimes[nSectionNo];
		}

		const char* GetName(int32 nSectionNo)
		{
			GUI_ASSERT( nSectionNo<m_nSectionNum, "wrong section number");

			return m_pSectionName[nSectionNo];
		}

		const int32		GetSectionNum() const
		{
			return m_nSectionNum;
		}


	private:
		unsigned*			m_pSectionCnt;
		CGUITimer*		m_pSectionBegin;
		CGUITimer*		m_pLastCount;
		CGUITimer*		m_pGlobalTimer;

		char**			m_pSectionName;
		real*			m_pSectionRate;
		unsigned*			m_pSectionMillionsec;
		unsigned*			m_pSectionTimes;
		unsigned*			m_pSectionLastTimes;
		int32				m_frameindex;
		real			m_fRecentFPS;

		int32				m_nFrameCnt;			///< frame count
		int32				m_nSectionNum;			///< section number
	};
	//--------------------------------------------------------------------------------------
	CPerfMon_impl::CPerfMon_impl( int32 nFrameCnt, int32 nSectionNum )
	{
		m_fRecentFPS = 0.0f;
		m_frameindex = 0;
		m_nSectionNum = nSectionNum;
		m_nFrameCnt = nFrameCnt;

		GUI_ASSERT( nFrameCnt > 0, "error" );
		GUI_ASSERT( nSectionNum > 0, "error" );

		//section beginer
		m_pSectionBegin = new CGUITimer[nSectionNum];
		GUI_ASSERT( m_pSectionBegin, "error" );

		//last count
		m_pLastCount = new CGUITimer;
		GUI_ASSERT( m_pLastCount, "error" );		

		//timer
		m_pGlobalTimer = new CGUITimer;
		GUI_ASSERT( m_pGlobalTimer, "error" );		

		//section count
		m_pSectionCnt = new unsigned[nSectionNum];
		GUI_ASSERT( m_pSectionCnt, "error" );
		memset( m_pSectionCnt, 0, nSectionNum*sizeof(unsigned));

		//section rate
		m_pSectionRate = new real[nSectionNum];
		GUI_ASSERT( m_pSectionRate, "error" );
		memset( m_pSectionRate, 0, nSectionNum*sizeof(real));

		//section times
		m_pSectionTimes = new unsigned[nSectionNum];
		GUI_ASSERT( m_pSectionTimes, "error" );
		memset( m_pSectionTimes, 0, nSectionNum*sizeof(unsigned));

		//section million second
		m_pSectionMillionsec = new unsigned[nSectionNum];
		GUI_ASSERT( m_pSectionMillionsec, "error" );
		memset(m_pSectionMillionsec , 0 , m_nSectionNum * sizeof(unsigned));

		//section last time
		m_pSectionLastTimes = new unsigned[nSectionNum];
		GUI_ASSERT( m_pSectionLastTimes, "error" );
		memset(m_pSectionLastTimes , 0 , m_nSectionNum * sizeof(unsigned));

		//section name
		m_pSectionName = new char*[nSectionNum];
		for( int32 i = 0; i<nSectionNum; ++i)
		{
			m_pSectionName[i] = new char[12];
			memset(m_pSectionName[i], 0, 12);
			char buf[20];
			sprintf( buf,"section %d", i );
			strncpy( m_pSectionName[i], buf, 12);
		}
	}
	//--------------------------------------------------------------------------------------
	CPerfMon_impl::~CPerfMon_impl()
	{

		//section beginer
		delete[] m_pSectionBegin;

		//last count
		delete m_pLastCount;

		//timer
		delete m_pGlobalTimer;

		//section count
		delete[] m_pSectionCnt;

		//section rate
		delete[] m_pSectionRate;

		//section times
		delete[] m_pSectionTimes;

		//section million second
		delete[] m_pSectionMillionsec;

		//section last time
		delete[] m_pSectionLastTimes;

		//section name
		for( int32 i = 0; i<m_nSectionNum; ++i)
		{
			delete[] m_pSectionName[i];
		}
		delete[] m_pSectionName;
	}
	//--------------------------------------------------------------------------------------
	void CPerfMon_impl::FrameUpdate()
	{
		++m_frameindex;


		if (m_frameindex == m_nFrameCnt)
		{
			m_pGlobalTimer->UpdateTime();

			unsigned u32Elapse = *m_pGlobalTimer - *m_pLastCount;

			//get fps
			m_fRecentFPS = (real)m_nFrameCnt *1000 / (real)u32Elapse;

			//update section information
			for (int32 i=0;i<m_nSectionNum;++i)
			{
				m_pSectionRate[i] = (real)(m_pSectionCnt[i]) / (real)(u32Elapse);
				m_pSectionMillionsec[i] = (unsigned)(m_pSectionCnt[i]);
				m_pSectionLastTimes[i] = m_pSectionTimes[i];
			}

			m_frameindex = 0;
			memset (m_pSectionCnt , 0 , sizeof(unsigned)*m_nSectionNum);
			memset(m_pSectionTimes , 0 , sizeof(unsigned)*m_nSectionNum);			

			*m_pLastCount = *m_pGlobalTimer;
		}
	}
	//--------------------------------------------------------------------------------------
}//namespace guiex

//====================================================================================//
// function
//====================================================================================// 
namespace guiex
{
	//--------------------------------------------------------------------------------------
	CPerfMonitor* CPerfMonitor::ms_perfMon = NULL;
	//--------------------------------------------------------------------------------------
	CPerfMonitor::CPerfMonitor()
	{
		m_impl = NULL;
	}
	//--------------------------------------------------------------------------------------
	CPerfMonitor::~CPerfMonitor()
	{
		Release();

	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::Init( int32 nFrameCnt, int32 nSectionNum)
	{
		GUI_ASSERT(ms_perfMon == 0, "CPerfMonitor has been initialed!");

		ms_perfMon = new CPerfMonitor;
		ms_perfMon->Initialize(nFrameCnt, nSectionNum);
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::Exit( )
	{
		GUI_ASSERT(ms_perfMon != 0, "CPerfMonitor has been exited!");
		ms_perfMon->Release();
		delete ms_perfMon;
		ms_perfMon = 0;
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::Initialize( int32 nFrameCnt, int32 nSectionNum )
	{
		GUI_ASSERT( m_impl == 0, "CPerfMonitor has been initialed!");
		m_impl = new CPerfMon_impl( nFrameCnt,nSectionNum );
		return;
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::Release( )
	{
		if( m_impl )
		{
			delete m_impl;
			m_impl = NULL;
		}
	}
	//--------------------------------------------------------------------------------------
	CPerfMonitor* CPerfMonitor::GetIt()
	{
		return ms_perfMon;
	}
	//--------------------------------------------------------------------------------------
	bool	CPerfMonitor::IsUpdated()
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->IsUpdated();
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::FrameUpdate()
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		m_impl->FrameUpdate();
	}
	//--------------------------------------------------------------------------------------
	real CPerfMonitor::GetFPS()
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetFPS();
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::BeginSection(int32 nSectionNo, const char* pName )
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		m_impl->BeginSection(nSectionNo, pName);
	}
	//--------------------------------------------------------------------------------------
	void CPerfMonitor::EndSection(int32 nSectionNo)
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		m_impl->EndSection(nSectionNo);
	}
	//--------------------------------------------------------------------------------------
	real CPerfMonitor::GetRate(int32 nSectionNo)
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetRate(nSectionNo);
	}
	//--------------------------------------------------------------------------------------
	unsigned CPerfMonitor::GetMillionsec(int32 nSectionNo)
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetMillionsec(nSectionNo);
	}
	//--------------------------------------------------------------------------------------
	unsigned CPerfMonitor::GetTimes(int32 nSectionNo)
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetTimes(nSectionNo);
	}
	//--------------------------------------------------------------------------------------
	const char* CPerfMonitor::GetName(int32 nSectionNo) const 
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetName(nSectionNo);
	}	
	//--------------------------------------------------------------------------------------
	const int32		CPerfMonitor::GetSectionNum() const
	{
		GUI_ASSERT(m_impl,"CPerfMonitor hasn't been initialized");
		return m_impl->GetSectionNum();
	}
	//--------------------------------------------------------------------------------------

}//namespace guiex

#endif	//GUI_PERFORMANCE_ON


