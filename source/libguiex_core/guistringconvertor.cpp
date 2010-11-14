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
#include <libguiex_core/guistringconvertor.h>

#if GUI_STRING_CONV_USE_ICONV
#	include <iconv.h>
#	include <errno.h>
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
	std::vector<CGUIString>	StringToVector(const CGUIString& rString )
	{
		std::vector<CGUIString> aListString;
		CGUIString::size_type idx = 0;

		while( idx < rString.size())
		{
			CGUIString::size_type ret = rString.find_first_of(",", idx);
			aListString.push_back(rString.substr(idx, ret));
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




	//------------------------------------------------------------------------------
	int WideByteToMultiChar( const CGUIStringEx& rSrc, CGUIString& rDst,  const char* szFromCode )
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
				"[WideByteToMultiChar]: failed to open iconv, errno is %d",
				errno);
			return -1;
		}

		//convert
		size_t	buf_size = rSrc.Size()*4+1;
		size_t	inbytesleft = rSrc.Size()*2;
		size_t	outbytesleft = buf_size;
		char*	dst = (char*)(new char[buf_size]);
		char*	pOutBuf = NULL;
		char*	pInBuf = (char*)rSrc.GetContent();

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
					"[WideByteToMultiChar]: failed to iconv, errno is EILSEQ");
				return -1;
			case EINVAL:
				throw CGUIException(
					"[WideByteToMultiChar]: failed to iconv, errno is EINVAL");
				return -1;
			default:
				throw CGUIException(
					"[WideByteToMultiChar]: failed to iconv, errno is %d",
					errno);
				return -1;
			}
		}

		//close iconv
		int ret = iconv_close(cd);
		if( ret == -1 )
		{
			throw CGUIException(
				"[WideByteToMultiChar]: failed to close iconv, errno is %d",
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
				"[WideByteToMultiChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}
#else
#	error "no support for MultiByteToWideChar() function"
#endif
		return 0;
	}
	//------------------------------------------------------------------------------
	int MultiByteToWideChar( const CGUIString& rSrc, CGUIStringEx& rDst, const char* szFromCode)
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
			//int errno_save = errno;

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
				"[MultiByteToWideChar]: not supported multi byte code <%s>",
				pMultiByteCode );
			return -1;
		}

#else
#	error "no support for MultiByteToWideChar() function"
#endif
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for bool
	template< >
	int32 StringToValue<bool>( const CGUIString& rString, bool& rValue)
	{
		if( rString == "true")
		{
			rValue = true;
			return 0;
		}
		else if (  rString == "false")
		{
			rValue = false;
			return 0;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[bool]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
	}

	template< >
	int32 ValueToString<bool>( const bool& rValue, CGUIString& rString)
	{
		rString = rValue?"true":"false";
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for uint32
	template< >
	int32 StringToValue<uint32>( const CGUIString& rString, uint32& rValue)
	{
		rValue = static_cast<uint32>(strtoul(rString.c_str(), 0, 10));

		return 0;
	}

	template< >
	int32 ValueToString<uint32>( const uint32& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for int32
	template<  >
	int32 StringToValue<int32>( const CGUIString& rString, int32& rValue)
	{
		rValue = static_cast<int32>(strtol(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<int32>( const int32& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for uint16
	template<  >
	int32 StringToValue<uint16>( const CGUIString& rString, uint16& rValue)
	{
		rValue = static_cast<uint16>(strtoul(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<uint16>( const uint16& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for int16
	template<  >
	int32 StringToValue<int16>( const CGUIString& rString, int16& rValue)
	{
		rValue = static_cast<int16>(strtol(rString.c_str(), 0, 10));

		return 0;
	}

	template<  >
	int32 ValueToString<int16>( const int16& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for real
	template<  >
	int32 StringToValue<real>( const CGUIString& rString, real& rValue)
	{
		rValue = static_cast<real>(strtod(rString.c_str(), 0));

		return 0;
	}

	template<  >
	int32 ValueToString<real>( const real& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue;
		rString = stream.str();

		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIRect
	template<  >
	int32 StringToValue<CGUIRect>( const CGUIString& rString, CGUIRect& rValue)
	{
		//string should have format as "left, top, right,bottom"
		std::vector<CGUIString> aListString= StringToVector(rString);
		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[StringToValue[CGUIRect]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue( aListString[0], rValue.m_fLeft );
		StringToValue( aListString[1], rValue.m_fTop );
		StringToValue( aListString[2], rValue.m_fRight );
		StringToValue( aListString[3], rValue.m_fBottom );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIRect>( const CGUIRect& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.m_fLeft<<','<<rValue.m_fTop<<','<<rValue.m_fRight<<','<<rValue.m_fBottom;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUISize
	template<  >
	int32 StringToValue<CGUISize>( const CGUIString& rString, CGUISize& rValue)
	{
		//string should have format as "width,height"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[StringToValue[CGUISize]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue( aListString[0], rValue.m_fWidth );
		StringToValue( aListString[1], rValue.m_fHeight );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUISize>( const CGUISize& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.m_fWidth<<','<<rValue.m_fHeight;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIVector2
	template<  >
	int32 StringToValue<CGUIVector2>( const CGUIString& rString, CGUIVector2& rValue)
	{
		//string should have format as "x,y"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 2 )
		{
			throw CGUIException(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue(aListString[0], rValue.x);
		StringToValue(aListString[1], rValue.y);
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIVector2>( const CGUIVector2& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.x<<','<<rValue.y;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIVector3
	template<  >
	int32 StringToValue<CGUIVector3>( const CGUIString& rString, CGUIVector3& rValue)
	{
		//string should have format as "x,y,z"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 3 )
		{
			throw CGUIException(
				"[StringToValue[StringToValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		StringToValue(aListString[0], rValue.x);
		StringToValue(aListString[1], rValue.y);
		StringToValue(aListString[2], rValue.z);
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIVector3>( const CGUIVector3& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.x<<','<<rValue.y<<','<<rValue.z;
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for CGUIColor
	template<  >
	int32 StringToValue<CGUIColor>( const CGUIString& rString, CGUIColor& rValue)
	{
		//string should have format as "r,g,b,a"
		std::vector<CGUIString> aListString= StringToVector(rString);

		if( aListString.size() != 4 )
		{
			throw CGUIException(
				"[StringToValue[CGUIColor]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}

		real fColor;
		StringToValue(aListString[0], fColor );
		rValue.SetRed( fColor );
		StringToValue(aListString[1], fColor );
		rValue.SetGreen( fColor );
		StringToValue(aListString[2], fColor );
		rValue.SetBlue( fColor );
		StringToValue(aListString[3], fColor );
		rValue.SetAlpha( fColor );
		return 0;
	}

	template<  >
	int32 ValueToString<CGUIColor>( const CGUIColor& rValue, CGUIString& rString )
	{
		std::stringstream stream;
		stream.width(0);
		stream.fill(' ');
		stream << rValue.GetRed()<<','<<rValue.GetGreen()<<','<<rValue.GetBlue()<<','<<rValue.GetAlpha();
		rString = stream.str();
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EImageOperation
	template<  >
	int32 StringToValue<EImageOperation>( const CGUIString& rString, EImageOperation& rValue)
	{
		if( rString == "IMAGE_NONE" )
		{
			rValue = IMAGE_NONE;
		}
		else if( rString == "IMAGE_ROTATE90CW" )
		{
			rValue = IMAGE_ROTATE90CW;
		}
		else if( rString == "IMAGE_ROTATE90CCW" )
		{
			rValue = IMAGE_ROTATE90CCW;
		}
		else if( rString == "IMAGE_FLIPHORIZON" )
		{
			rValue = IMAGE_FLIPHORIZON;
		}
		else if( rString == "IMAGE_FLIPVERTICAL" )
		{
			rValue = IMAGE_FLIPVERTICAL;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[EImageOperation]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EImageOperation>( const EImageOperation& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case IMAGE_NONE:
			rString = CGUIString("IMAGE_NONE");
			break;
		case IMAGE_ROTATE90CW:
			rString =  CGUIString("IMAGE_ROTATE90CW");
			break;
		case IMAGE_ROTATE90CCW:
			rString =  CGUIString("IMAGE_ROTATE90CCW");
			break;
		case IMAGE_FLIPHORIZON:
			rString =  CGUIString("IMAGE_FLIPHORIZON");
			break;
		case IMAGE_FLIPVERTICAL:
			rString =  CGUIString("IMAGE_FLIPVERTICAL");
			break;
		default:
			throw CGUIException(
				"[ValueToString[EImageOperation]]: unknown image operation <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for EScreenValue
	template<  >
	int32 StringToValue<EScreenValue>( const CGUIString& rString, EScreenValue& rValue)
	{
		if( rString == "eScreenValue_Pixel" )
		{
			rValue = eScreenValue_Pixel;
		}
		else if( rString == "eScreenValue_Percentage" )
		{
			rValue = eScreenValue_Percentage;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[EScreenValue]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<EScreenValue>( const EScreenValue& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eScreenValue_Pixel:
			rString = CGUIString("eScreenValue_Pixel");
			break;
		case eScreenValue_Percentage:
			rString = CGUIString("eScreenValue_Percentage");
			break;
		default:
			throw CGUIException(
				"[ValueToString[EScreenValue]]: unknown image operation <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentHorz
	template<  >
	int32 StringToValue<ETextAlignmentHorz>( const CGUIString& rString, ETextAlignmentHorz& rValue)
	{
		if( rString == "eTextAlignment_Horz_Left" )
		{
			rValue = eTextAlignment_Horz_Left;
		}
		else if( rString == "eTextAlignment_Horz_Right" )
		{
			rValue = eTextAlignment_Horz_Right;
		}
		else if( rString == "eTextAlignment_Horz_Center" )
		{
			rValue = eTextAlignment_Horz_Center;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[ETextAlignmentHorz]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<ETextAlignmentHorz>( const ETextAlignmentHorz& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eTextAlignment_Horz_Left:
			rString = CGUIString("eTextAlignment_Horz_Left");
			break;
		case eTextAlignment_Horz_Right:
			rString = CGUIString("eTextAlignment_Horz_Right");
			break;
		case eTextAlignment_Horz_Center:
			rString = CGUIString("eTextAlignment_Horz_Center");
			break;
		default:
			throw CGUIException(
				"[ValueToString[ETextAlignmentHorz]]: unknown ETextAlignmentHorz enum <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	//convert for ETextAlignmentVert
	template<  >
	int32 StringToValue<ETextAlignmentVert>( const CGUIString& rString, ETextAlignmentVert& rValue)
	{
		if( rString == "eTextAlignment_Vert_Up" )
		{
			rValue = eTextAlignment_Vert_Up;
		}
		else if( rString == "eTextAlignment_Vert_Down" )
		{
			rValue = eTextAlignment_Vert_Down;
		}
		else if( rString == "eTextAlignment_Vert_Center" )
		{
			rValue = eTextAlignment_Vert_Center;
		}
		else
		{
			throw CGUIException(
				"[StringToValue[ETextAlignmentVert]]: string value format is wrong! <%s>",
				rString.c_str());
			return -1;
		}
		return 0;
	}

	template<  >
	int32 ValueToString<ETextAlignmentVert>( const ETextAlignmentVert& rValue, CGUIString& rString )
	{
		switch(rValue)
		{
		case eTextAlignment_Vert_Up:
			rString = CGUIString("eTextAlignment_Vert_Up");
			break;
		case eTextAlignment_Vert_Down:
			rString = CGUIString("eTextAlignment_Vert_Down");
			break;
		case eTextAlignment_Vert_Center:
			rString = CGUIString("eTextAlignment_Vert_Center");
			break;
		default:
			throw CGUIException(
				"[ValueToString[ETextAlignmentVert]]: unknown ETextAlignmentVert enum <%d>",
				rValue);
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
 