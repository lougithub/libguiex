/** 
 * @file guistringconv_internal.cpp
 * @brief used to convert string between different code
 * @author ken
 * @date 2011-04-29
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/stringconv_internal/guistringconv_internal.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_internal);
	//------------------------------------------------------------------------------
	const char* IGUIStringConv_internal::StaticGetModuleName()
	{
		return "IGUIStringConv_internal";
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_internal::IGUIStringConv_internal()
		:IGUIInterfaceStringConv(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_internal::~IGUIStringConv_internal()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIStringConv_internal::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_internal::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_internal::Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		if( rSrc.empty())
		{
			rDst.clear();
			return 0;
		}
		const unsigned char* utf8 = (const unsigned char*)rSrc.c_str();
	
		size_t	buf_size = rSrc.size()+1;
		wchar_t* dst = new wchar_t[buf_size];

		uint32 nUtf16Count = 0;
		for(uint32 count=0; count<rSrc.size(); )
		{    
			wchar_t integer = 0;
			if( utf8[count] < 0x80 )
			{
				// <0x80
				integer = utf8[count];            
				count++;
			}
			else if( (utf8[count] < 0xDF) && (utf8[count]>=0x80))
			{
				integer = utf8[count] & 0x1F;
				integer = integer << 6;
				integer += utf8[count+1] &0x3F;
				count+=2;
			}
			else if( (utf8[count] <= 0xEF) && (utf8[count]>=0xDF))
			{
				integer = utf8[count] & 0x0F;
				integer = integer << 6;
				integer += utf8[count+1] &0x3F;
				integer = integer << 6;
				integer += utf8[count+2] &0x3F;
				count+=3;
			}
			else
			{
				delete dst;
				CGUIException::ThrowException( "[IGUIStringConv_internal::Utf8ToUtf16]: error" );
				return -1;
			}
			dst[nUtf16Count] = integer;
			++nUtf16Count;
		}
		dst[nUtf16Count] = 0;
		rDst = dst;
		delete[] dst;
		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_internal::Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		if( rSrc.empty())
		{
			return 0;
		}
		const wchar_t* utf16 = rSrc.c_str();

		size_t buf_size = (rSrc.size()*4)+1;
		char* utf8 = (char*)(new char[buf_size]);

		uint32 count = 0;
		for( uint32 i=0; i<rSrc.size(); ++i )
		{    
			wchar_t integer = utf16[i];

			if( integer<0x80)
			{            
				utf8[count] = utf16[i] & 0x7f;
				count++;
			}
			else if( integer>=0x80 && integer<0x07ff)
			{
				utf8[count] = 0xC0 | (0x1F & integer>>6);
				utf8[count+1] = 0x80 | (0x3F & integer);
				count+=2;
			}
			else if( integer>=0x0800 )
			{            
				utf8[count] = 0xE0 | (0x0F & integer>>12);
				utf8[count+1] = 0x80 | ((0x0FC0 & integer)>>6);
				utf8[count+2] = 0x80 | (0x003F & integer);
				count += 3;
			}    
			else 
			{
				delete utf8;
				CGUIException::ThrowException( "[IGUIStringConv_internal::Utf16ToUtf8]: error" );
				return -1;
			}
		}

		utf8[count] = 0;
		rDst = utf8;
		delete[] utf8;

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_internal::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
}
