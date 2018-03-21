/*
* ��Ȩ����(c) 2016 - 2021 ����׿����ͨ�Ƽ��������ι�˾
* 
*   ��Ȩ���� (c) δ�����������������á����ġ��ַ����ļ���
*
**/

/*
* �ļ����ƣ�VeBaseType.h
* ժ    Ҫ���������ݽṹ
* 
* ��ǰ�汾��1.0
* ��    �ߣ����ά <67904662@qq.com>
* ������ڣ�2016.10.3
*/

#ifndef VISIONEBASETYPE_H_
#define VISIONEBASETYPE_H_

#include "VeStdInt.h"

#ifdef WIN32
#pragma pack (4)
#endif

#define VE_MAX_POLYGON_POINT_NUM			50		 //�����������
#define VE_MAX_TRACK_POINT_NUM				25*60*10  //Ŀ��������10����

#define VE_MAX_BLOB_NUMBER					64		 //����ſ����
#define VE_MAX_EVENTREPORT_SIZE				10		 //�¼������������
#define VE_MAX_NAME_STRING_NUM				16		 //��������ֽ���
#define VE_MAX_INFO_STRING_NUM				64		 //������Ϣ����ֽ���

#define VE_MAX_COORDINATION_LINE_NUM		5	//�궨��ÿ������������

#ifdef WIN32
	typedef struct{
		veu32   nYear;			//��
		veu8    nMonth;			//�� 	
		veu8    nDay;			//�� 
		veu8    nHour;			//ʱ
		veu8    nMinute;		//�� 
		veu16   nSecond;		//��
		veu16   nMillSecond;	//����
		}VE_HIC_TIME, *LPVE_HIC_TIME;
#else
	#ifdef linux
		typedef struct{
			veu32   nYear;			//��
			veu8    nMonth;			//�� 	
			veu8    nDay;			//�� 
			veu8    nHour;			//ʱ
			veu8    nMinute;		//�� 
			veu16   nSecond;		//��
			veu16   nMillSecond;	//����
			}VE_HIC_TIME, *LPVE_HIC_TIME;
	#else
		typedef struct{
			veu8    nYear;			//��
			veu8    nMonth;			//�� 	
			veu8    nDay;			//��
			veu8    nWeek;			//���� 
			veu8    nHour;			//ʱ
			veu8    nMinute;		//�� 
			veu8   	nSecond;		//��
			veu8   	nMillSecond;	//����
			}VE_HIC_TIME, *LPVE_HIC_TIME;
	#endif
#endif

//�ڲ�ʹ�õ�ṹ
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

//�ߴ�
typedef struct _VE_Size_
{
    ves32 width;
    ves32 height;
}VE_SIZE, VE_Size;

//�ڲ�ʹ���߽ṹ
typedef struct _VE_LINE_
{
    VE_POINT start;
    VE_POINT end;
	veu32	 dir;    //�����ߵķ���
}VE_LINE, VE_Line;

typedef struct _VE_FLINE_
{
	VE_FPOINT start;
	VE_FPOINT end;
}VE_FLINE, VE_FLine;

//������궨����
typedef struct _VE_CalibLine
{
    VE_POINT                start;  //����ͼ���е����
    VE_POINT                end;    //����ͼ���е��յ�
    int                     nLen;   //�������������µĳ���(����)
}VE_CalibLine, VeCalibLine;

//���νṹ, �Ƽ�ʹ��, ����opencv
typedef struct _VE_Rect_
{
    ves32 x;      //��
    ves32 y;      //��
    ves32 width;  //��
    ves32 height; //��
} VE_RECT, VE_Rect;

//�ı�������ṹ
typedef struct _VE_QUADRANGLE_
{
    VE_POINT point[4];
}VE_QUADRANGLE, VE_Quadrangle,VE_Quad;

//�ڲ�ʹ�ö��������ṹ
typedef struct _VE_POLYGON_
{
    veu8        pointNum;
    VE_POINT    point[VE_MAX_POLYGON_POINT_NUM];
} VE_POLYGON, VE_Polygon, VE_CURVES, VE_Curves;

//�ڲ�ʹ�����߽ṹ, ĩβ���պ�
typedef VE_Polygon VE_Polyline;

//������������ṹ
typedef struct _VE_POLYGON_DIR_
{
    VE_Line    line;     //����궨��
    VE_Polygon polygon;  //���������
}VE_POLYGON_DIR, VE_PolygonDir;


//��������
typedef enum _VE_PixelFormat
{
    PIXEL_FORMAT_YUV422_PLAIN = 0x1,  //y[w x h], u[w x h / 2], v[w x h / 2]
    PIXEL_FORMAT_YUV420_PACK  = 0x2,  //y[w x h], uv[w x h / 2]
    PIXEL_FORMAT_RGB24        = 0x3, 
    PIXEL_FORMAT_YUV420_PLAIN = 0x4,  //y[w x h], u[w x h / 4], v[w x h / 4]
    PIXEL_FORMAT_GRAY8 
}VE_PixelFormat;


//Ŀ��λ��
typedef struct _MyRECT {
	ves32 xmin;
	ves32 ymin;
	ves32 xmax;
	ves32 ymax;
} OBJRECT, *POBJRECT;

//�򻯰�Ŀ����Ϣ
typedef struct _TinyObject {
	ves8 type;
	ves8 pose;
	ves8 truncated;
	ves8 score;
	ves8 label;
	ves8 name[32];
	OBJRECT  rect;
} TinyObject, *PTinyObject;

//������Ϣ
typedef struct _human
{
	ves8 age;
	ves8 sex;
	ves8 togcolor;
	ves8 score;
	OBJRECT  rect;
}Human;

//�ڲ�ʹ�ñ궨����
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
	VE_LINE			verticalLine[VE_MAX_COORDINATION_LINE_NUM];	//ͬ��·��ֱ
	VE_LINE			alongLine[VE_MAX_COORDINATION_LINE_NUM];		//���ŵ�·

	vef32			vRealDistance[VE_MAX_COORDINATION_LINE_NUM];	//ͬ��һ���߶εľ���
	vef32			aRealDistance[VE_MAX_COORDINATION_LINE_NUM];	//ͬ��һ���߶εľ���

	vebool		    bVertValid[VE_MAX_COORDINATION_LINE_NUM];		//���ŵ�·
	vebool		    bAlongValid[VE_MAX_COORDINATION_LINE_NUM];		//���ŵ�·

}VE_GRID,Visione_GRID;

//��ʼ����
ve_inline VE_POINT vePoint(ves32 x, ves32 y)
{
    VE_POINT pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

//��ʼ������
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
