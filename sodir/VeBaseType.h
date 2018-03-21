/*
* 版权所有(c) 2016 - 2021 北京卓视智通科技有限责任公司
* 
*   版权所有 (c) 未经允许，不得擅自引用、更改、分发本文件。
*
**/

/*
* 文件名称：VeBaseType.h
* 摘    要：基本数据结构
* 
* 当前版本：1.0
* 作    者：吴柯维 <67904662@qq.com>
* 完成日期：2016.10.3
*/

#ifndef VISIONEBASETYPE_H_
#define VISIONEBASETYPE_H_

#include "VeStdInt.h"

#ifdef WIN32
#pragma pack (4)
#endif

#define VE_MAX_POLYGON_POINT_NUM			50		 //多边形最大点数
#define VE_MAX_TRACK_POINT_NUM				25*60*10  //目标最多跟踪10分钟

#define VE_MAX_BLOB_NUMBER					64		 //最多团块个数
#define VE_MAX_EVENTREPORT_SIZE				10		 //事件报告的最大个数
#define VE_MAX_NAME_STRING_NUM				16		 //名字最大字节数
#define VE_MAX_INFO_STRING_NUM				64		 //报警信息最大字节数

#define VE_MAX_COORDINATION_LINE_NUM		5	//标定线每个方向最大个数

#ifdef WIN32
	typedef struct{
		veu32   nYear;			//年
		veu8    nMonth;			//月 	
		veu8    nDay;			//日 
		veu8    nHour;			//时
		veu8    nMinute;		//分 
		veu16   nSecond;		//秒
		veu16   nMillSecond;	//毫秒
		}VE_HIC_TIME, *LPVE_HIC_TIME;
#else
	#ifdef linux
		typedef struct{
			veu32   nYear;			//年
			veu8    nMonth;			//月 	
			veu8    nDay;			//日 
			veu8    nHour;			//时
			veu8    nMinute;		//分 
			veu16   nSecond;		//秒
			veu16   nMillSecond;	//毫秒
			}VE_HIC_TIME, *LPVE_HIC_TIME;
	#else
		typedef struct{
			veu8    nYear;			//年
			veu8    nMonth;			//月 	
			veu8    nDay;			//日
			veu8    nWeek;			//星期 
			veu8    nHour;			//时
			veu8    nMinute;		//分 
			veu8   	nSecond;		//秒
			veu8   	nMillSecond;	//毫秒
			}VE_HIC_TIME, *LPVE_HIC_TIME;
	#endif
#endif

//内部使用点结构
typedef struct _VE_POINT_
{
    ves32 x;
    ves32 y;
}VE_POINT, VE_Point;

inline  VE_Point  Ve_Point(int x, int y)
{
	VE_Point p;

	p.x = x;
	p.y = y;

	return p;
}
typedef struct _VE_FPOINT_
{
	vef32 x;
	vef32 y;
}VE_FPOINT, VE_FPoint;

//尺寸
typedef struct _VE_Size_
{
    ves32 width;
    ves32 height;
}VE_SIZE, VE_Size;

//内部使用线结构
typedef struct _VE_LINE_
{
    VE_POINT start;
    VE_POINT end;
	veu32	 dir;    //定义线的方向
}VE_LINE, VE_Line;

typedef struct _VE_FLINE_
{
	VE_FPOINT start;
	VE_FPOINT end;
}VE_FLINE, VE_FLine;

//摄像机标定用线
typedef struct _VE_CalibLine
{
    VE_POINT                start;  //线在图像中的起点
    VE_POINT                end;    //线在图像中的终点
    int                     nLen;   //线在世界坐标下的长度(厘米)
}VE_CalibLine, VeCalibLine;

//矩形结构, 推荐使用, 兼容opencv
typedef struct _VE_Rect_
{
    ves32 x;      //左
    ves32 y;      //上
    ves32 width;  //宽
    ves32 height; //高
} VE_RECT, VE_Rect;

//四边形区域结构
typedef struct _VE_QUADRANGLE_
{
    VE_POINT point[4];
}VE_QUADRANGLE, VE_Quadrangle,VE_Quad;

//内部使用多边形区域结构
typedef struct _VE_POLYGON_
{
    veu8        pointNum;
    VE_POINT    point[VE_MAX_POLYGON_POINT_NUM];
} VE_POLYGON, VE_Polygon, VE_CURVES, VE_Curves;

//内部使用折线结构, 末尾不闭合
typedef VE_Polygon VE_Polyline;

//有向多边形区域结构
typedef struct _VE_POLYGON_DIR_
{
    VE_Line    line;     //方向标定线
    VE_Polygon polygon;  //多边形区域
}VE_POLYGON_DIR, VE_PolygonDir;


//像素类型
typedef enum _VE_PixelFormat
{
    PIXEL_FORMAT_YUV422_PLAIN = 0x1,  //y[w x h], u[w x h / 2], v[w x h / 2]
    PIXEL_FORMAT_YUV420_PACK  = 0x2,  //y[w x h], uv[w x h / 2]
    PIXEL_FORMAT_RGB24        = 0x3, 
    PIXEL_FORMAT_YUV420_PLAIN = 0x4,  //y[w x h], u[w x h / 4], v[w x h / 4]
    PIXEL_FORMAT_GRAY8 
}VE_PixelFormat;


//目标位置
typedef struct _MyRECT {
	ves32 xmin;
	ves32 ymin;
	ves32 xmax;
	ves32 ymax;
} OBJRECT, *POBJRECT;

//简化版目标信息
typedef struct _TinyObject {
	ves8 type;
	ves8 pose;
	ves8 truncated;
	ves8 score;
	ves8 label;
	ves8 name[32];
	OBJRECT  rect;
} TinyObject, *PTinyObject;

//人类信息
typedef struct _human
{
	ves8 age;
	ves8 sex;
	ves8 togcolor;
	ves8 score;
	OBJRECT  rect;
}Human;

//内部使用标定网格
typedef struct _VE_GRID_
{
	// 	_Visione_GRID_()
	// 	{
	// 		memset(verticalLine,0,sizeof(verticalLine));
	// 		memset(alongLine,0,sizeof(alongLine));
	// 		memset(vRealDistance,0,sizeof(vRealDistance));
	// 		memset(aRealDistance,0,sizeof(aRealDistance));
	// 		memset(verticalLine,0,sizeof(verticalLine));
	// 		memset(bVertValid,0,sizeof(bVertValid));
	// 	}
	VE_LINE			verticalLine[VE_MAX_COORDINATION_LINE_NUM];	//同道路垂直
	VE_LINE			alongLine[VE_MAX_COORDINATION_LINE_NUM];		//沿着道路

	vef32			vRealDistance[VE_MAX_COORDINATION_LINE_NUM];	//同第一条线段的距离
	vef32			aRealDistance[VE_MAX_COORDINATION_LINE_NUM];	//同第一条线段的距离

	vebool		    bVertValid[VE_MAX_COORDINATION_LINE_NUM];		//沿着道路
	vebool		    bAlongValid[VE_MAX_COORDINATION_LINE_NUM];		//沿着道路

}VE_GRID,Visione_GRID;

//初始化点
ve_inline VE_POINT vePoint(ves32 x, ves32 y)
{
    VE_POINT pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

//初始化矩形
ve_inline VE_Rect veRect(ves32 x, ves32 y, ves32 width, ves32 height)
{
    VE_Rect rect;
    rect.x     = x;
    rect.y     = y;
    rect.width = width;
    rect.height= height;
    return rect;
}

ve_inline VE_SIZE veSize(int width, int height)
{
    VE_SIZE size;
    size.height = height;
    size.width  = width;
    return size;
}

#ifdef WIN32
#pragma pack ()
#else
/*
#pragma STRUCT_ALIGN(_VE_POINT_,4)
#pragma STRUCT_ALIGN(_VE_POLYGON_,4)
#pragma STRUCT_ALIGN(_VE_QUADRANGLE_,4)
#pragma STRUCT_ALIGN(_VE_LINE_,4)
#pragma STRUCT_ALIGN(_VE_RECT_,4)
*/
#endif
#endif //VISIONEBASETYPE_H_
