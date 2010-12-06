
namespace guiex
{
	typedef short			int16;
	typedef int				int32;
	typedef char			int8;
	typedef unsigned short	uint16;
	typedef unsigned int	uint32;
	typedef unsigned char	uint8;
	typedef float			real;
 
	enum EScreenValue
	{
		eScreenValue_Pixel = 0,
		eScreenValue_Percentage,
	};

	enum EInterpolationType
	{
		eInterpolationType_Linear = 0,
		eInterpolationType_EaseIn,
		eInterpolationType_EaseInOut,
	};
}
