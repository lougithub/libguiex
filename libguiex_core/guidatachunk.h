/** 
 * @file guidatachunk.h
 * @brief data chunk
 * @author ken
 * @date 2006-07-06
 */


#ifndef __GUI_DATA_CHUNK_H_20060706__
#define __GUI_DATA_CHUNK_H_20060706__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


namespace guiex
{
//============================================================================//
// class
//============================================================================// 
/**
 * @class CGUIDataChunk
 * @brief data chunk, the allocadated data can be released automatically
 */
class GUIEXPORT CGUIDataChunk
{
public:
	/**
 	 * @brief Constructor for CGUIDataChunk class
	 */
    CGUIDataChunk()
      : m_pData(NULL),
        m_nSize(0)
    {
    }

	/**
 	 * @brief Destructor for CGUIDataChunk class
	 */
	virtual ~CGUIDataChunk(void) 
    { 
        Release();
    }

	/** 
	 * @brief Allocates the passed number of bytes.
	 * @return NULL for failed
	 */ 
	uint8 * Allocate( size_t size )
	{
		Release();
		m_pData = new uint8[size];
		m_nSize = size;
		return m_pData;
	}

	/**
	 * @brief Return a pointer to the external data
	 */
    uint8* GetDataPtr(void) { return m_pData; }

	/**
	 * @brief Return a pointer to the external data
	 */
    const uint8* GetDataPtr(void) const { return m_pData; }

	/**
	 * @brief Set the size of the external data.  
	 */
    void SetSize(size_t size) { m_nSize = size; }

	/**
	 * @brief Get the size of the external data.  
	 */
    size_t GetSize(void) const { return m_nSize; }

	/**
	 * @brief Release supplied data.
	 */
    virtual void Release(void)
    {
        if(m_pData)
        {
            delete[] m_pData;
            m_pData = 0;
        }
    }


protected:
    uint8*	m_pData;
    size_t		m_nSize;
};

}//namespace guiex

#endif //__GUI_DATA_CHUNK_H_20060706__
