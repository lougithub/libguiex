/** 
* @file guiimageloader_devil.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module\imageloader_devil\guiimageloader_devil.h>
#include <libguiex_module\imageloader_devil\guiimagedata_devil.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacefilesys.h>


#include <IL\il.h>
#include <IL\ilu.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIImageLoader_devil);
	//------------------------------------------------------------------------------
	IGUIImageLoader_devil::IGUIImageLoader_devil()
	{
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_devil::~IGUIImageLoader_devil()
	{
	}
	//------------------------------------------------------------------------------
	int		IGUIImageLoader_devil::DoInitialize(void* )
	{
		// Initialize DevIL.
		ilInit();
		iluInit();
		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_devil::DoDestroy()
	{
		//shutdown DevIL
		ilShutDown();
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_devil::LoadFromFile( const CGUIString& rFileName  )
	{
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();;
		CGUIDataChunk aDataChunk;
		if( 0 == pFileSys->ReadFile( rFileName, aDataChunk ))
		{
			return LoadFromMemory(aDataChunk.GetDataPtr(), aDataChunk.GetSize());
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_devil::LoadFromMemory( uint8* pFileData, size_t nSize )
	{
		if( !pFileData || nSize <= 0 )
		{
			return NULL;
		}

		ILuint imgName;
		ilGenImages(1, &imgName);
		ilBindImage(imgName);

		if (ilLoadL(IL_TYPE_UNKNOWN, pFileData, (ILuint)nSize) != IL_FALSE)
		{
			//create image data
			CGUIImageData_devil * pImageData = new CGUIImageData_devil(this);

			// flip the image
			//iluFlipImage();

			// get details about size of loaded image
			ILinfo imgInfo;
			iluGetImageInfo(&imgInfo);

			// allocate temp buffer to receive image data
			uint8* tmpBuff = pImageData->SetImageData(imgInfo.Width, imgInfo.Height);
			pImageData->SetPixelFormat(GUI_PF_RGBA_32);

			// get image data in required format
			ilCopyPixels(0, 0, 0, imgInfo.Width, imgInfo.Height, 1, IL_RGBA, IL_UNSIGNED_BYTE, (void*)tmpBuff);

			// delete DevIL image
			ilDeleteImages(1, &imgName);

			return pImageData;
		}
		// failed to load image properly.
		else
		{
			// delete DevIL image
			ilDeleteImages(1, &imgName);

			throw CGUIException("[IGUIImageLoader_devil::LoadFromMemory] - Failed to load image by DevIL!");
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_devil::DestroyImageData(CGUIImageData* pImageData)
	{
		delete pImageData;
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_devil::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------


}//namespace guiex

