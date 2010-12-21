/** 
* @file guistringconv_iconv.cpp
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/stringconv_iconv/guistringconv_iconv.h>
#include <libguiex_core/guiexception.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_iconv);
	//------------------------------------------------------------------------------
	const char* IGUIStringConv_iconv::StaticGetModuleName()
	{
		return "IGUIStringConv_iconv";
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_iconv::IGUIStringConv_iconv()
	:IGUIInterfaceStringConv(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_iconv::~IGUIStringConv_iconv()
	{
	}
	
	//------------------------------------------------------------------------------
	int	IGUIStringConv_iconv::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_iconv::DoDestroy()
	{
		
	}	
	//------------------------------------------------------------------------------
	int IGUIStringConv_iconv::Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		if( rSrc.empty())
		{
			rDst.clear();
			return 0;
		}

		//open iconv
		iconv_t cd = iconv_open ("UTF-16LE", "UTF-8");
		if( cd == (iconv_t)-1 )
		{
			throw CGUIException(
				"[MultiByteToWideChar]: failed to open iconv, errno is %d",
				errno);
			return -1;
		}

		//convert
		size_t	buf_size = rSrc.size()+4;
		size_t	inbytesleft = rSrc.size();
		size_t	outbytesleft = buf_size;
		char*	dst = (char*)(new wchar_t[buf_size]);
		char*	pOutBuf = NULL;
		char*	pInBuf = (char*)(rSrc.c_str());

		bool	bError = false;

		while(inbytesleft > 0) 
		{
			pOutBuf = dst;
			outbytesleft = buf_size*sizeof(wchar_t);

			size_t retbytes = iconv(cd, &pInBuf, &inbytesleft, &pOutBuf, &outbytesleft);

			if (dst != pOutBuf)  
			{
				// we have something to write
				rDst.append((wchar_t*)dst, (pOutBuf-dst)/sizeof(wchar_t));
			} 

			//check ret
			if( retbytes == size_t(-1) )
			{
				if( errno == E2BIG )
				{
					continue;
				}
				else
				{
					bError = true;
					break;
				}
			}
			else
			{
				//success
				break;
			}
		}

		delete[] dst;
		if( bError)
		{
			switch(errno)
			{
			case EILSEQ:
				throw CGUIException(
					"[MultiByteToWideChar]: failed to iconv, errno is EILSEQ");
				return -1;
			case EINVAL:
				throw CGUIException(
					"[MultiByteToWideChar]: failed to iconv, errno is EINVAL");
				return -1;
			default:
				throw CGUIException(
					"[MultiByteToWideChar]: failed to iconv, errno is %d",
					errno);
				return -1;
			}
		}

		//close iconv
		int ret = iconv_close(cd);
		if( ret == -1 )
		{
			throw CGUIException(
				"[MultiByteToWideChar]: failed to close iconv, errno is %d",
				errno);
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_iconv::Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		if( rSrc.empty())
		{
			rDst.clear();
			return 0;
		}

		//open iconv
		iconv_t cd = iconv_open ("UTF-8", "UTF-16LE");
		if( cd == (iconv_t)-1 )
		{
			throw CGUIException(
				"[Utf16ToUtf8]: failed to open iconv, errno is %d",
				errno);
			return -1;
		}

		//convert
		size_t	buf_size = rSrc.size()*4+1;
		size_t	inbytesleft = rSrc.size()*2;
		size_t	outbytesleft = buf_size;
		char*	dst = (char*)(new char[buf_size]);
		char*	pOutBuf = NULL;
		char*	pInBuf = (char*)rSrc.c_str();

		bool	bError = false;

		while(inbytesleft > 0) 
		{
			pOutBuf = dst;
			outbytesleft = buf_size;

			size_t retbytes = iconv(cd, &pInBuf, &inbytesleft, &pOutBuf, &outbytesleft);
			//int errno_save = errno;

			if (dst != pOutBuf)  
			{
				// we have something to write
				rDst.append(dst, (pOutBuf-dst));
			} 

			//check ret
			if( retbytes == size_t(-1) )
			{
				if( errno == E2BIG )
				{
					continue;
				}
				else
				{
					bError = true;
					break;
				}
			}
			else
			{
				//success
				break;
			}
		}

		delete[] dst;
		if( bError)
		{
			switch(errno)
			{
			case EILSEQ:
				throw CGUIException(
					"[Utf16ToUtf8]: failed to iconv, errno is EILSEQ");
				return -1;
			case EINVAL:
				throw CGUIException(
					"[Utf16ToUtf8]: failed to iconv, errno is EINVAL");
				return -1;
			default:
				throw CGUIException(
					"[Utf16ToUtf8]: failed to iconv, errno is %d",
					errno);
				return -1;
			}
		}

		//close iconv
		int ret = iconv_close(cd);
		if( ret == -1 )
		{
			throw CGUIException(
				"[Utf16ToUtf8]: failed to close iconv, errno is %d",
				errno);
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIStringConv_iconv::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------	
}
