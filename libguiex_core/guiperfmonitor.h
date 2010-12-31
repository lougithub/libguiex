/** 
 * @file PerfMonitor.h
 * @brief thanks for yu jian rong
 * @author ken
 * @date 2004-10-28
 */
/*******************************************
 * PerfMon
 *   Performance monitor
 *******************************************/



#ifndef _KEN_PERFMONITOR_28102004_H_
#define _KEN_PERFMONITOR_28102004_H_

//====================================================================================//
// include
//====================================================================================//
#include "guibase.h"

//====================================================================================//
// define
//====================================================================================// 


#if GUI_PERFORMANCE_ON
#	define  PERFMON_INIT( nFrameCnt, nSectionNum )	CPerfMonitor::Init(nFrameCnt, nSectionNum)
#	define  PERFMON_EXIT( )							CPerfMonitor::Exit( )

#	define  PERFMON_BEGIN( nSection, name )			CPerfMonitor::GetIt()->BeginSection( nSection, name )
#	define  PERFMON_END( nSection )					CPerfMonitor::GetIt()->EndSection( nSection )
#	define  PERFMON_SCOPE( nSection, name )			CPerfMonObject		gPerfMon_##nSection(nSection, name);
#else	//GUI_PERFORMANCE_ON
#	define  PERFMON_INIT( nFrameCnt, nSectionNum )		
#	define  PERFMON_EXIT( )		

#	define  PERFMON_BEGIN( nSection, name )		
#	define  PERFMON_END( nSection )		
#	define  PERFMON_SCOPE( nSection, name )		
#endif	//GUI_PERFORMANCE_ON


#if GUI_PERFORMANCE_ON

//====================================================================================//
// declare
//====================================================================================// 
namespace guiex
{
	class CPerfMon_impl;
}
	


//====================================================================================//
// class
//====================================================================================// 
namespace guiex
{
class GUIEXPORT CPerfMonitor
{
public:	
	~CPerfMonitor();

	static void Init( int32 nFrameCnt, int32 nSectionNum);
	static void Exit( );
	static CPerfMonitor* GetIt();


	/** 
	* @brief calls this each frame!
	*/
	void FrameUpdate();

	/** 
	* @brief is frame updated this frame
	*/
	bool IsUpdated();

	/** 
	* @brief get frame numbers per second
	*/
	real GetFPS();

	/** 
	* @brief call this in the begin of the section.
	*/
	void BeginSection(int32 nSectionNo, const char* pName);

	/** 
	* @brief call this in the end of the section
	*/
	void EndSection(int32 nSectionNo);

	/** 
	* @brief get the rate of this section.
	*/
	real GetRate(int32 nSectionNo);

	/** 
	* @brief get the millionsecond of the section.
	*/
	unsigned GetMillionsec(int32 nSectionNo);

	/** 
	* @brief get the run timers of the section.
	*/
	unsigned GetTimes(int32 nSectionNo);

	/** 
	* @brief get the name of the section.
	*/
	const char* GetName(int32 nSectionNo) const;

	/** 
	* @brief get number of the section.
	*/
	const int32 GetSectionNum() const;

protected:
	CPerfMonitor();

	void Initialize( int32 nFrameCnt, int32 nSectionNum );
	void Release();

	static CPerfMonitor* ms_perfMon;

private:
	CPerfMon_impl* m_impl;
};


/** 
* @brief object of the performance monitor.
* for scope monitor.
*/
class CPerfMonObject
{
public:
	CPerfMonObject(int32 nSectionNo, const char* pName)
		:m_nSectionNo(nSectionNo)
	{
		CPerfMonitor::GetIt()->BeginSection( nSectionNo, pName );
	}

	~CPerfMonObject()
	{
		CPerfMonitor::GetIt()->EndSection( m_nSectionNo );
	}

protected:
	int32 m_nSectionNo;
};
}//namespace guiex

#endif	//GUI_PERFORMANCE_ON


#endif	//_KEN_PERFMONITOR_28102004_H_

