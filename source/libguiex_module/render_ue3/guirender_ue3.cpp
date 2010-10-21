/** 
* @file guirender_dx9.cpp
* @brief use directx 9 to render gui
* @author ken
* @date 2006-07-06
*/

//============================================================================//
// include
//============================================================================// 
#include "EnginePrivate.h"

#include <module\render_ue3\guirender_ue3.h>
#include <module\render_ue3\guitexture_ue3.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guicolorrect.h>
#include <libguiex_core\guiperfmonitor.h>



namespace guiex
{
	//============================================================================//
	// function
	//============================================================================// 
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_UE3);
	//------------------------------------------------------------------------------
	IGUIRender_UE3::IGUIRender_UE3()
		:m_pUE3Canvas(NULL)
		,m_bQueueing(true)
	{

	}
	//------------------------------------------------------------------------------
	IGUIRender_UE3::~IGUIRender_UE3()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIRender_UE3::DoInitialize(void* pData )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_UE3::DoDestroy()
	{
		DestroyAllTexture();
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::SetWireFrame( bool bWireFrame)
	{
		checkMsg(0, TEXT("not implemented"));
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::EnableScissor( bool bEnable )
	{
		checkMsg(0, TEXT("not implemented"));
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::AddRenderTexture(
		const CGUIRect& rDestRect, 
		real z, 
		const CGUITextureImp* pTexture, 
		const CGUIRect& rTextureRect, 
		EImageOperation eImageOperation, 										
		GUIARGB rColor_topleft,
		GUIARGB rColor_topright,
		GUIARGB rColor_bottomleft,
		GUIARGB rColor_bottomright)
	{
		SCOPE_CYCLE_COUNTER(STAT_Canvas_DrawTextureTileTime);

		check( m_pUE3Canvas );
		const FTexture* FinalTexture = (FTexture*)(((CGUITexture_UE3*)pTexture)->GetUE3Texture());
		FBatchedElements* BatchedElements = m_pUE3Canvas->GetBatchedElements(FCanvas::ET_Triangle, FinalTexture, SE_BLEND_Translucent);	
		FHitProxyId HitProxyId = m_pUE3Canvas->GetHitProxyId();

		FLinearColor ActualColor;
		CGUIColor aGUIColor;

		//position
		FLOAT X = rDestRect.m_fLeft;		
		FLOAT Y = rDestRect.m_fTop;	
		FLOAT SizeX = rDestRect.GetWidth();
		FLOAT SizeY = rDestRect.GetHeight();

		//UV
		FLOAT U=0.f,V=0.f,SizeU=0.f,SizeV=0.f;
		switch(eImageOperation)
		{
		case IMAGE_NONE:				///!< none operation
			U = rTextureRect.m_fLeft;
			V = rTextureRect.m_fTop;
			SizeU = rTextureRect.GetWidth();
			SizeV = rTextureRect.GetHeight();
			break;
		case IMAGE_ROTATE90CW:		///!< rotate image for 90 CW
			U = rTextureRect.m_fLeft;
			V = rTextureRect.m_fBottom;
			SizeU = rTextureRect.GetWidth();
			SizeV = -rTextureRect.GetHeight();
			break;
		case IMAGE_ROTATE90CCW:		///!< rotate image for 90 CCW
			U = rTextureRect.m_fRight;
			V = rTextureRect.m_fTop;
			SizeU = -rTextureRect.GetWidth();
			SizeV = -rTextureRect.GetHeight();
			break;
		case IMAGE_FLIPHORIZON:		///!< flip image horizon
			U = rTextureRect.m_fRight;
			V = rTextureRect.m_fTop;
			SizeU = -rTextureRect.GetWidth();
			SizeV = rTextureRect.GetHeight();
			break;
		case IMAGE_FLIPVERTICAL:		///!< flip image vertical
			U = rTextureRect.m_fLeft;
			V = rTextureRect.m_fBottom;
			SizeU = rTextureRect.GetWidth();
			SizeV = -rTextureRect.GetHeight();
			break;
		default:
			throw CGUIException("[IGUIRender_UE3::AddRenderTexture]: unknown ImageOperation format;");
			break;
		}

		//top left
		aGUIColor.SetARGB( rColor_topleft );
		ActualColor.A = aGUIColor.GetAlpha() * m_pUE3Canvas->AlphaModulate;
		ActualColor.R = aGUIColor.GetRed();
		ActualColor.G = aGUIColor.GetGreen();
		ActualColor.B = aGUIColor.GetBlue();
		INT V00 = BatchedElements->AddVertex(FVector4(X,		Y,			0,1),FVector2D(U,			V),			ActualColor,HitProxyId);
		
		//top right
		aGUIColor.SetARGB( rColor_topright );
		ActualColor.A = aGUIColor.GetAlpha() * m_pUE3Canvas->AlphaModulate;
		ActualColor.R = aGUIColor.GetRed();
		ActualColor.G = aGUIColor.GetGreen();
		ActualColor.B = aGUIColor.GetBlue();
		INT V10 = BatchedElements->AddVertex(FVector4(X + SizeX,Y,			0,1),FVector2D(U + SizeU,	V),			ActualColor,HitProxyId);
		
		//bottom left
		aGUIColor.SetARGB( rColor_bottomleft );
		ActualColor.A = aGUIColor.GetAlpha() * m_pUE3Canvas->AlphaModulate;
		ActualColor.R = aGUIColor.GetRed();
		ActualColor.G = aGUIColor.GetGreen();
		ActualColor.B = aGUIColor.GetBlue();
		INT V01 = BatchedElements->AddVertex(FVector4(X,		Y + SizeY,	0,1),FVector2D(U,			V + SizeV),	ActualColor,HitProxyId);
		
		//bottom right
		aGUIColor.SetARGB( rColor_bottomright );
		ActualColor.A = aGUIColor.GetAlpha() * m_pUE3Canvas->AlphaModulate;
		ActualColor.R = aGUIColor.GetRed();
		ActualColor.G = aGUIColor.GetGreen();
		ActualColor.B = aGUIColor.GetBlue();
		INT V11 = BatchedElements->AddVertex(FVector4(X + SizeX,Y + SizeY,	0,1),FVector2D(U + SizeU,	V + SizeV),	ActualColor,HitProxyId);

		BatchedElements->AddTriangle(V00,V10,V11,FinalTexture,SE_BLEND_Translucent);
		BatchedElements->AddTriangle(V00,V11,V01,FinalTexture,SE_BLEND_Translucent);
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::AddScissor( const CGUIRect& rClipRect)
	{
		check( m_pUE3Canvas );
		//m_pUE3Canvas->ReplaceMaskRegion(rClipRect.m_fLeft, rClipRect.m_fTop, rClipRect.GetWidth(), rClipRect.GetHeight());
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::DoRender(void)
	{
		check( m_pUE3Canvas );
		m_pUE3Canvas->Flush();
	}

	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::ClearRenderList(void)
	{
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::EnableRenderQueue(bool bEnable)
	{
		m_bQueueing = bEnable;
	}
	//------------------------------------------------------------------------------
	bool	IGUIRender_UE3::IsRenderQueueEnabled(void) const
	{
		return m_bQueueing;
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_UE3::CreateTexture(void)
	{
		CGUITexture_UE3* pTexture = new CGUITexture_UE3(this);
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_UE3::CreateTexture(const CGUIString& filename)
	{
		CGUITexture_UE3* pTexture = new CGUITexture_UE3(this);
		if( pTexture->LoadFromFile(filename) != 0 )
		{
			//failed
			delete pTexture;
			return NULL;
		}
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_UE3::CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		CGUITexture_UE3* pTexture = new CGUITexture_UE3(this);
		pTexture->SetUE3TextureSize(nWidth,nHeight,ePixelFormat );
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	void		IGUIRender_UE3::DestroyTexture(CGUITextureImp* texture)
	{
		if (texture != NULL)
		{
			TSetTexture::iterator itor = m_setTexture.find(texture);
			GUI_ASSERT( itor!= m_setTexture.end(),"failed to destroy texture");
			m_setTexture.erase(itor);

			delete texture;
		}
	}
	//------------------------------------------------------------------------------
	void		IGUIRender_UE3::DestroyAllTexture()
	{
		while( m_setTexture.empty() == false)
		{
			DestroyTexture(*m_setTexture.begin());
		}
	}
	//------------------------------------------------------------------------------
	uint16	IGUIRender_UE3::GetWidth(void) const
	{
		checkMsg(0, TEXT("not implemented"));
		return 0;
	}
	//------------------------------------------------------------------------------
	uint16	IGUIRender_UE3::GetHeight(void) const
	{
		checkMsg(0, TEXT("not implemented"));
		return 0;
	}
	//------------------------------------------------------------------------------
	uint32	IGUIRender_UE3::GetMaxTextureSize(void) const
	{
		return appRoundUpToPowerOfTwo(GMaxTextureMipCount);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void	IGUIRender_UE3::SetCanvas( FCanvas* pCanvas )
	{
		assert( pCanvas );
		m_pUE3Canvas = pCanvas;
	}
	//------------------------------------------------------------------------------


}//namespace guiex

