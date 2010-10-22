/** 
* @file guistringconvertor.cpp
* @brief convert string from other value or
* convert other value to string
* @author ken
* @date 2006-08-30
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>
#include <sstream>

#if GUI_STRING_CONV_USE_ICONV
#	include <iconv.h>
#	pragma comment( lib, "libiconv.lib" )
#elif  GUI_STRING_CONV_USE_MSC
#else
#	error "no support for MultiByteToWideChar() function"
#endif

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	bool CGUIStringConvertor::StringToBool( const CGUIString& rStringValue )
	{
		if( rStringValue == "true")
		{
			return true;
		}
		else if (  rStringValue == "false")
		{
			return false;
		}
		else
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToBool]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIStringConvertor::BoolToString( bool bValue )
	{
		return bValue?"true":"false";
	}
	//------------------------------------------------------------------------------
	uint32 CGUIStringConvertor::StringToUInt( const CGUIString& rStringValue )
	{
		return static_cast<uint32>(strtoul(rStringValue.c_str(), 0, 10));
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIStringConvertor::UIntToString( uint32 nValue  )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << nValue;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	uint32 CGUIStringConvertor::StringToInt( const CGUIString& rStringValue )
	{
		return static_cast<uint32>(strtol(rStringValue.c_str(), 0, 10));
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIStringConvertor::IntToString( int32 nValue )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << nValue;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	real CGUIStringConvertor::StringToReal( const CGUIString& rStringValue )
	{
		return static_cast<real>(atof(rStringValue.c_str()));
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIStringConvertor::RealToString( real fValue )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << fValue;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	static std::vector<CGUIString>	StringToVector(const CGUIString& rStringValue )
	{
		std::vector<CGUIString> aListString;
		CGUIString::size_type idx = 0;

		while( idx < rStringValue.size())
		{
			CGUIString::size_type ret = rStringValue.find_first_of(",", idx);
			aListString.push_back(rStringValue.substr(idx, ret));
			if( ret == CGUIString::npos )
			{
				break;
			}
			else
			{
				idx = ret+1;
			}
		}
		return aListString;
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIStringConvertor::StringToRect( const CGUIString& rStringValue )
	{
		//string should have format as "left, top, right,top"
		std::vector<CGUIString> aListString= StringToVector(rStringValue);
		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToRect]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}

		return CGUIRect( 
			StringToReal(aListString[0]),
			StringToReal(aListString[1]),
			StringToReal(aListString[2]),
			StringToReal(aListString[3]));
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::RectToString( const CGUIRect& rRect)
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rRect.m_fLeft<<','<<rRect.m_fTop<<','<<rRect.m_fRight<<','<<rRect.m_fBottom;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	CGUISize CGUIStringConvertor::StringToSize( const CGUIString& rStringValue )
	{
		//string should have format as "width,height"
		std::vector<CGUIString> aListString= StringToVector(rStringValue);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToSize]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}

		return CGUISize( 
			StringToReal(aListString[0]),
			StringToReal(aListString[1]));
	}
	//------------------------------------------------------------------------------
	void CGUIStringConvertor::StringToSize( const CGUIString& rStringValue, CGUISize& rSize )
	{
		//string should have format as "width,height"
		std::vector<CGUIString> aListString= StringToVector(rStringValue);
		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToSize]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}

		rSize.SetWidth( StringToReal(aListString[0]) );
		rSize.SetHeight( StringToReal(aListString[1]) );
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::SizeToString( const CGUISize& rSize)
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rSize.m_fWidth<<','<<rSize.m_fHeight;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIStringConvertor::StringToVector2( const CGUIString& rStringValue )
	{
		//string should have format as "x,y"
		std::vector<CGUIString> aListString= StringToVector(rStringValue);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToVector2]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}

		return CGUIVector2( 
			StringToReal(aListString[0]),
			StringToReal(aListString[1]));
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::Vector2ToString( const CGUIVector2& rVector2)
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rVector2.x<<','<<rVector2.y;
		return stream.str();
	}
	//------------------------------------------------------------------------------
	CGUIColor CGUIStringConvertor::StringToColor( const CGUIString& rStringValue )
	{
		//string should have format as "r,g,b,a"
		std::vector<CGUIString> aListString= StringToVector(rStringValue);

		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToColor]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}

		return CGUIColor( 
			StringToReal(aListString[0]),
			StringToReal(aListString[1]),
			StringToReal(aListString[2]),
			StringToReal(aListString[3]));
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::ColorToString( const CGUIColor& rColor)
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rColor.GetRed()<<','<<rColor.GetGreen()<<','<<rColor.GetBlue()<<','<<rColor.GetAlpha();
		return stream.str();
	}
	//------------------------------------------------------------------------------
	EImageOperation CGUIStringConvertor::StringToImageOperation( const CGUIString& rStringValue )
	{
		if( rStringValue == "IMAGE_NONE" )
		{
			return IMAGE_NONE;
		}
		else if( rStringValue == "IMAGE_ROTATE90CW" )
		{
			return IMAGE_ROTATE90CW;
		}
		else if( rStringValue == "IMAGE_ROTATE90CCW" )
		{
			return IMAGE_ROTATE90CCW;
		}
		else if( rStringValue == "IMAGE_FLIPHORIZON" )
		{
			return IMAGE_FLIPHORIZON;
		}
		else if( rStringValue == "IMAGE_FLIPVERTICAL" )
		{
			return IMAGE_FLIPVERTICAL;
		}
		else
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToImageOperation]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::ImageOperationToString( EImageOperation eOperation)
	{
		switch(eOperation)
		{
		case IMAGE_NONE:
			return CGUIString("IMAGE_NONE");
		case IMAGE_ROTATE90CW:
			return CGUIString("IMAGE_ROTATE90CW");
		case IMAGE_ROTATE90CCW:
			return CGUIString("IMAGE_ROTATE90CCW");
		case IMAGE_FLIPHORIZON:
			return CGUIString("IMAGE_FLIPHORIZON");
		case IMAGE_FLIPVERTICAL:
			return CGUIString("IMAGE_FLIPVERTICAL");
		default:
			throw CGUIException(
				"[CGUIStringConvertor::ImageOperationToString]: unknown image operation <%d>",
				eOperation);
		}
	}
	//------------------------------------------------------------------------------
	EAlignMode CGUIStringConvertor::StringToAlignMode( const CGUIString& rStringValue )
	{
		if( rStringValue == "eAbsolute" )
		{
			return eAbsolute;
		}
		else if( rStringValue == "eRelative" )
		{
			return eRelative;
		}
		else if( rStringValue == "eRelativePos_AbsoluteSize" )
		{
			return eRelativePos_AbsoluteSize;
		}
		else
		{
			throw CGUIException(
				"[CGUIStringConvertor::StringToImageOperation]: string value format is wrong! <%s>",
				rStringValue.c_str());
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIStringConvertor::AlignModeToString( EAlignMode eAlignMode)
	{
		switch(eAlignMode)
		{
		case eAbsolute:
			return CGUIString("eAbsolute");
		case eRelative:
			return CGUIString("eRelative");
		case eRelativePos_AbsoluteSize:
			return CGUIString("eRelativePos_AbsoluteSize");
		default:
			throw CGUIException(
				"[CGUIStringConvertor::ImageOperationToString]: unknown image operation <%d>",
				eAlignMode);
		}
	}
	//------------------------------------------------------------------------------
	int			CGUIStringConvertor::WideByteToMultiChar( const CGUIStringEx& rSrc, CGUIString& rDst,  const char* szFromCode )
	{
		if( rSrc.Empty())
		{
			return 0;
		}

#if GUI_STRING_CONV_USE_ICONV
		//open iconv
		iconv_t cd = iconv_open (szFromCode?szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode(), "UTF-16LE");
		if( cd == (iconv_t)-1 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::WideByteToMultiChar]: failed to open iconv, errno is %d",
				errno);
			return -1;
		}

		//convert
		size_t	buf_size = rSrc.Size()*4+1;
		size_t	inbytesleft = rSrc.Size()*2;
		size_t	outbytesleft = buf_size;
		char*	dst = (char*)(new char[buf_size]);
		char*	pOutBuf = NULL;
		const char*	pInBuf = (const char*)rSrc.GetContent();

		bool	bError = false;

		while(inbytesleft > 0) 
		{
			pOutBuf = dst;
			outbytesleft = buf_size;

			size_t retbytes = iconv(cd, &pInBuf, &inbytesleft, &pOutBuf, &outbytesleft);
			int errno_save = errno;

			if (dst != pOutBuf)  
			{
				// we have something to write
				rDst.append(dst, (pOutBuf-dst));
			} 

			//check ret
			if( retbytes == -1 )
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
					"[CGUIStringConvertor::WideByteToMultiChar]: failed to iconv, errno is EILSEQ");
				return -1;
			case EINVAL:
				throw CGUIException(
					"[CGUIStringConvertor::WideByteToMultiChar]: failed to iconv, errno is EINVAL");
				return -1;
			default:
				throw CGUIException(
					"[CGUIStringConvertor::WideByteToMultiChar]: failed to iconv, errno is %d",
					errno);
				return -1;
			}
		}

		//close iconv
		int ret = iconv_close(cd);
		if( ret == -1 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::WideByteToMultiChar]: failed to close iconv, errno is %d",
				errno);
			return -1;
		}

#elif GUI_STRING_CONV_USE_MSC
		const char* pMultiByteCode = szFromCode ? szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode();
		if( strcmp( pMultiByteCode, "UTF-8" ) == 0 )
		{
			//utf8
			size_t	buf_size = (rSrc.Size()+1)*4;
			char*	dst = (char*)(new char[buf_size]);
			::WideCharToMultiByte(CP_UTF8,0,rSrc.GetContent(),-1,dst,buf_size, NULL, NULL );
			rDst.append(dst);
			delete[] dst;
		}
		else
		{
			throw CGUIException(
				"[CGUIStringConvertor::WideByteToMultiChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}
#else
#	error "no support for MultiByteToWideChar() function"
#endif
		return 0;
	}
	//------------------------------------------------------------------------------
	int		CGUIStringConvertor::MultiByteToWideChar( const CGUIString& rSrc, CGUIStringEx& rDst, const char* szFromCode)
	{
		if( rSrc.empty())
		{
			return 0;
		}

#if GUI_STRING_CONV_USE_ICONV
		//open iconv
		iconv_t cd = iconv_open ("UTF-16LE", szFromCode?szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode());
		if( cd == (iconv_t)-1 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::MultiByteToWideChar]: failed to open iconv, errno is %d",
				errno);
			return -1;
		}

		//convert
		size_t	buf_size = rSrc.size()+4;
		size_t	inbytesleft = rSrc.size();
		size_t	outbytesleft = buf_size;
		char*	dst = (char*)(new wchar_t[buf_size]);
		char*	pOutBuf = NULL;
		const char*	pInBuf = rSrc.c_str();

		bool	bError = false;

		while(inbytesleft > 0) 
		{
			pOutBuf = dst;
			outbytesleft = buf_size*sizeof(wchar_t);
			
			size_t retbytes = iconv(cd, &pInBuf, &inbytesleft, &pOutBuf, &outbytesleft);
			int errno_save = errno;

			if (dst != pOutBuf)  
			{
				// we have something to write
				rDst.Append((wchar_t*)dst, (pOutBuf-dst)/sizeof(wchar_t));
			} 

			//check ret
			if( retbytes == -1 )
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
					"[CGUIStringConvertor::MultiByteToWideChar]: failed to iconv, errno is EILSEQ");
				return -1;
			case EINVAL:
				throw CGUIException(
					"[CGUIStringConvertor::MultiByteToWideChar]: failed to iconv, errno is EINVAL");
				return -1;
			default:
				throw CGUIException(
					"[CGUIStringConvertor::MultiByteToWideChar]: failed to iconv, errno is %d",
					errno);
				return -1;
			}
		}

		//close iconv
		int ret = iconv_close(cd);
		if( ret == -1 )
		{
			throw CGUIException(
				"[CGUIStringConvertor::MultiByteToWideChar]: failed to close iconv, errno is %d",
				errno);
			return -1;
		}
#elif GUI_STRING_CONV_USE_MSC
		const char* pMultiByteCode = szFromCode ? szFromCode:CGUIWidgetSystem::Instance()->GetDefaultCode();
		if( strcmp( pMultiByteCode, "UTF-8" ) == 0 )
		{
			//utf8
			size_t	buf_size = rSrc.size()+1;
			wchar_t*	dst = new wchar_t[buf_size];
			::MultiByteToWideChar(CP_UTF8,0,rSrc.c_str(),-1,dst,buf_size);
			rDst.Append(dst);
			delete[] dst;
		}
		else
		{
			throw CGUIException(
				"[CGUIStringConvertor::MultiByteToWideChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}

#else
#	error "no support for MultiByteToWideChar() function"
#endif
		return 0;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
 