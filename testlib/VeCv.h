/*************************************************************************
* @file VeCv.h  本文件包含了跟Opencv有关的应用，以便其他位置不再出现Opencv的引用
*
* 版权所有(c) 2016 - 2021 北京卓视智通科技有限责任公司
*
*
* @author kovu <67904662@qq.com>
* @version 1.0
* @date 2016.10.3
*************************************************************************/


#ifndef VISIONE_CV_H
#define VISIONE_CV_H


//#ifdef __cplusplus
//extern "C" {
//#endif

#define  WIN32_VERSION
#ifdef WIN32
    #pragma warning(disable:4996)
	#pragma warning(disable:4244)
#endif

#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/gpu/gpu.hpp"
#include "opencv2/video/background_segm.hpp"
//#include "opencv2/contrib/contrib.hpp"


#if 0
#pragma comment(lib,"opencv_calib3d2412.lib")
#pragma comment(lib,"opencv_contrib2412.lib") 
#pragma comment(lib,"opencv_core2412.lib") 
#pragma comment(lib,"opencv_features2d2412.lib")  
#pragma comment(lib,"opencv_flann2412.lib") 
#pragma comment(lib,"opencv_gpu2412.lib") 
#pragma comment(lib,"opencv_highgui2412.lib")  
#pragma comment(lib,"opencv_imgproc2412.lib")  
#pragma comment(lib,"opencv_legacy2412.lib")  
#pragma comment(lib,"opencv_ml2412.lib") 
#pragma comment(lib,"opencv_nonfree2412.lib")  
#pragma comment(lib,"opencv_objdetect2412.lib")  
#pragma comment(lib,"opencv_ocl2412.lib") 
#pragma comment(lib,"opencv_photo2412.lib")  
#pragma comment(lib,"opencv_stitching2412.lib")  
#pragma comment(lib,"opencv_superres2412.lib")  
#pragma comment(lib,"opencv_ts2412.lib") 
#pragma comment(lib,"opencv_video2412.lib")  
#pragma comment(lib,"opencv_videostab2412.lib") 

#endif

#define __VE_BEGIN__        // {CV_FUNCNAME(__FUNCTION__)
#define __VE_END__          // goto exit; exit: ; }
#define __VE_EXIT__			// goto exit;
#define VE_ASSERT            assert

//时间测量函数
#define TIME_BEGIN()                                                        \
	{ \
	double dTick1 = (double)cvGetTickCount();			                    \
	double dTick2, dFreq;								                    \
	unsigned int nId = GetCurrentThreadId();                                \
	char outStrxxx[1024];                                                   \

#define TIME_END(_name)    							                            \
	dFreq = (double)cvGetTickFrequency();				                    \
	dTick2 = (double)cvGetTickCount();					                    \
	dTick1 = dTick2 - dTick1;   						                    \
	dTick1 = dTick1 / dFreq;							                    \
	sprintf(outStrxxx, "ThreadID--%d, %s time %3.1f \n", nId, _name, dTick1 / 1000); \
	OutputDebugStringA(outStrxxx);                                          \
}

//安全创建和释放图像
#define SAFECREATE_IMAGE(x,y,z) cvCreateImage((x),(y),(z))
#define SAFECREATE_IMAGEHEADER(x,y,z) cvCreateImageHeader((x),(y),(z))
#define SAFERELEASE_IMAGEHEADER(x) if( (x)!= NULL) { cvReleaseImageHeader(&x); x=NULL; }
#define SAFERELEASE_IMAGE(x) if( (x)!= NULL) { cvReleaseImage(&x); x=NULL; }

//申请和释放
#define MEM_alloc(x,y,z)  malloc(y);
#define MEM_free(x, y, z) free(y);

extern int EXTERNALHEAP;

#define SAFEDELETE_EX(x,y) if ((x)!= NULL){ MEM_free(EXTERNALHEAP,x,sizeof(y));/*delete (x);*/ x=NULL;}
#define SAFEDELETE_ARRAY_EX(x,y,z) if ((x)!= NULL){ MEM_free(EXTERNALHEAP,x,sizeof(y)*z)/*delete (x);*/; (x)=NULL;}

	//#endif
#define ASSERT_CHECK_ZEROIMAGE(x,y)  VE_ASSERT(x!=NULL);CHECKVALID_RX(x,y);cvZero(x);

#define SAFERELEASE_MAT(x) if( (x)!= NULL) { cvReleaseMat(&x); x=NULL; }
#define SAFERELEASE_SEQ(x)if( (x)!= NULL) { cvReleaseMemStorage(&x->storage); }

#define RGB2CVRGB(x) CV_RGB((x)&0x000000ff,((x)&0x0000ff00)>>8,((x)&0x00ff0000)>>16)
#define SAFERESET_IMAGE(x) if ( (x)!= NULL) { cvZero(x);} 

#define TRY_AND_CVTGRAY(src,dst) if((src)->nChannels == 3) { cvCvtColor(src,dst,CV_BGR2GRAY);} \
								 else if ((src)->nChannels == 1) { cvCopy(src, dst, NULL); }
#define RGB2CVRGB(x) CV_RGB((x)&0x000000ff,((x)&0x0000ff00)>>8,((x)&0x00ff0000)>>16)
#define SAFEDELETE(x) if ((x)!= NULL){ delete (x); x=NULL;}
#define SAFEDELETE_ARRAY(x) if ((x)!= NULL){ delete [] (x); x=NULL;}

#define cvSafeCloneImage(src,dst)   if(src == NULL) return vefalse;\
	if (dst == NULL) { dst = cvCreateImage(cvGetSize(src), (src)->depth, (src)->nChannels); cvCopyImage(src, dst); }

#define CHECKVALID_RF(x)	if ((x)==NULL || (x) == INVALID_HANDLE_VALUE){ return vefalse; }
#define CHECKVALID_RT(x)	if ((x)==NULL)	{ return ; }
#define CHECKVALID_RX(x,y)	if ((x)==NULL)	{ return (y); }
#define CHECKTRUE_RF(x)		if ((x)==vefalse)	{ return vefalse; }
#define CHECKTRUE_RT(x)		if ((x)==vefalse)	{ return ; }
#define CHECKTRUE_RX(x,y)	if ((x)==vefalse)	{ return (y); }

#define PI					3.1415926


#define VE_IS_RECT_IN_IMAGE(r, img) \
	(r.x >= 0 && r.y >= 0 && r.width > 0 && r.height > 0 && r.x + r.width <= img.width && r.y + r.height <= img.height)


inline void cvCopyImageEx(IplImage* src, IplImage*& dst)
{
	CHECKVALID_RT(src);
	if (dst == NULL)
	{
		dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	}
	cvCopyImage(src, dst);
}


								
//#ifdef __cplusplus
//}
//#endif

#endif
