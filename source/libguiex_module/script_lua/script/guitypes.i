
namespace guiex
{

	/***********************************************************
	types
	***********************************************************/
	typedef short			int16;
	typedef int				int32;
	typedef char			int8;

	typedef unsigned short	uint16;
	typedef unsigned int	uint32;
	typedef unsigned char	uint8;

	typedef float			real;
 

	enum EImageOperation
	{
		IMAGE_NONE,				///!< none operation
		IMAGE_ROTATE90CW,		///!< rotate image for 90 CW
		IMAGE_ROTATE90CCW,		///!< rotate image for 90 CCW
		IMAGE_FLIPHORIZON,		///!< flip image horizon
		IMAGE_FLIPVERTICAL,		///!< flip image vertical
	};

}
