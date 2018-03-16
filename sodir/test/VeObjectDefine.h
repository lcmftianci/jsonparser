/**
* @file VeObjectDefine.h  定义了目标有关的基本数据结构
*
* 版权所有(c) 2016 - 2021 北京卓视智通科技有限责任公司
*
*
* @author 吴柯维 <67904662@qq.com>
* @version 1.0
* @date 2016.10.3
*/


#ifndef _VISIONE_OBJECT_DEFINE_H_
#define _VISIONE_OBJECT_DEFINE_H_

#include "VeStdInt.h"
#include "VeCv.h"
#include "VeBaseType.h"

#define VE_MAX_OBJECT_NUMBER				100       //最多目标个数
#define VE_MAX_OBJTRK_NUM		VE_MAX_OBJECT_NUMBER  //最多跟踪目标格式 
#define VE_MAX_TRAJECTORY_LEN				256       //最多轨迹长度
#define VE_MAX_OBJSUBIMAGE_LEN				5		  //目标切图最多临时缓存数量

///////////////// 目标OBJ//////////////////////////

//目标状态
typedef enum _VE_ObjectStatus_
{
	VE_OBJ_STATUS_DELETE = 0x0001,					// 删除目标
	VE_OBJ_STATUS_INIT = 0x0002,					// 初始化目标
	VE_OBJ_STATUS_MACHED = 0x0004,					// 匹配目标
	VE_OBJ_STATUS_PREDICTED = 0x0008,				// 预测目标
	VE_OBJ_STATUS_STABLE = 0x0010,					// 稳定目标
	VE_OBJ_STATUS_ABNML = 0x0020,					// 异常目标
	VE_OBJ_STATUS_COLLISION = 0x0040,				// 碰撞目标
	VE_OBJ_STATUS_RELIABLE = 0x0080,				// 可信属性		
	VE_OBJ_STATUS_VANISH = 0x0100					// 消失状态
} VE_ObjectStatus;

//目标类型
typedef enum _VE_ObjectType_
{
	VE_OBJ_TYPE_UNKNOWN = 0x0000,					// 未知类型
	VE_OBJ_TYPE_HUMAN = 0x0001,						// 人
	VE_OBJ_TYPE_VERHICLE = 0x0002,					// 车
	VE_OBJ_TYPE_NOCAR = 0x0004,						// 非机动车
	VE_OBJ_TYPE_FACE = 0x0008,						// 人脸
	VE_OBJ_TYPE_WHEEL = 0x00016,					// 车轮
	VE_OBJ_TYPE_OTHER = 0x0032,						// 其他自然界物体
} VE_ObjectType;



//目标精确类型
typedef enum _VE_ObjectTypeEx_
{
	ve_obj_typeex_unkown = 0x0000,					// 未知类型
	ve_obj_typeex_bicycle = 0x0001,
	ve_obj_typeex_bus = 0x0002,
	ve_obj_typeex_car,
	ve_obj_typeex_motorbike,
	ve_obj_typeex_person,
	ve_obj_typeex_truck,
	ve_obj_typeex_tricar,
	ve_obj_typeex_backmirror,
	ve_obj_typeex_paperbox,
	ve_obj_typeex_lightcover,
	ve_obj_typeex_windglass,
	ve_obj_typeex_hungs,
	ve_obj_typeex_anusigns,
	ve_obj_typeex_entrylisence,
	ve_obj_typeex_safebelt,
	ve_obj_typeex_plate,
	ve_obj_typeex_carlight,
	ve_obj_typeex_cartopwindow,
	ve_obj_typeex_carrier,
	ve_obj_typeex_newersign,
	ve_obj_typeex_wheel,
	ve_obj_typeex_layon,
	ve_obj_typeex_max,
}VE_ObjectTypeEx;

static char VE_ObjectTypeName[ve_obj_typeex_max][32] = {
	"background",
	"bicycle",
	"bus",
	"car",
	"motorbike",
	"person",
	"truck",
	"tricar",
	"backmirror",
	"paperbox",
	"lightcover",
	"windglass",
	"hungs",
	"anusigns",
	"entrylisence",
	"safebelt",
	"plate",
	"carlight",
	"cartopwindow",
	"carrier",
	"newersign",
	"wheel",
	"layon",
};

static char VE_ObjectTypeChineseName[ve_obj_typeex_max][32] = {
	"背景",
	"自行车",
	"公共汽车",
	"轿车",
	"摩托车",
	"人",
	"货车",
	"三轮车",
	"后视镜",
	"抽纸盒",
	"遮阳板",
	"挡风玻璃",
	"挂件",
	"年检标",
	"出入证",
	"安全带",
	"车牌",
	"车灯",
	"天窗",
	"行李架",
	"新手标",
	"车轮",
	"摆件",
};


typedef struct _VE_TinyFeature_
{
	ves8  cFeature[64];
	veu32 nFeatureIndex;
	veu8  nFeatureScore;
}VE_TinyFeature;

typedef struct _VE_ChannelDescript_
{
	ves8  cChannelDescript[64];
	veu32 nChannelIndex;
	ves8  cChannelReserve[32];
}VE_ChannelDescript;

#define MAX_HUMANFEATURE_NUMBER 10
//特征类型
typedef enum _VE_HumanFeatureType_
{
	VE_HUMFEA_AGETYPE = 0x0000,					//年龄类型 未成年;青年;中年;老年;未知;
	VE_HUMFEA_BAGTYPE = 0x0001,					//背包类型 未背包;单肩包;双肩包;手提包;未知;
	VE_HUMFEA_CLOUSECOLOR = 0x0002,				//上衣颜色 白;黑;红;黄;蓝;绿;紫;棕;灰;橙;未知;
	VE_HUMFEA_CLOUSETYPE = 0x0003,				//上衣类型 长袖;短袖;未知;
	VE_HUMFEA_HAIRTYPE = 0x0004,				//发型     长发;短发;未知;
	VE_HUMFEA_HAPTYPE = 0x0005,					//帽子类型 未戴帽;鸭舌帽;其他帽;未知;
	VE_HUMFEA_RACETYPE = 0x0006,				//种族类型 黄种人;白人;黑人;未知;
	VE_HUMFEA_SEXTYPE = 0x0007,					//年龄类型 未成年;青年;中年;老年;未知;
	VE_HUMFEA_TROUSECOLOR = 0x0008,				//裤子颜色 白;黑;红;黄;蓝;绿;紫;棕;灰;橙;未知;
	VE_HUMFEA_TROUSETYPE = 0x0009,				//裤子类型 长裤;短裤;未知;
	VE_HUMFEA_MAX = 0x0010,
} VE_HumanFeatureType;

typedef struct _VE_FeatureInfo_
{
	//车牌
	ves8  cPlate[16];

	//车型
	VE_TinyFeature tfBrand;
	VE_TinyFeature tfColor;
	VE_TinyFeature tfType;

	VE_TinyFeature tfHumanFea[MAX_HUMANFEATURE_NUMBER];				//行人特征

	ves8  reseverd[10];						//预留//  

}VE_FeatureInfo;

//目标 基本轨迹信息
typedef struct _VE_ObjectInfo_
{
	veu64			m_uiTimeStamp;					// 时间戳
	VE_ObjectType	m_uiObjType;					// 目标类型
	VE_ObjectTypeEx	m_uiObjTypeEx;					// 目标精确类型
	VE_ObjectStatus m_uiObjStatus;	    			// 目标状态
	VE_Point		m_vObjCenter;					// 中心坐标				
	VE_Rect			m_vObjRect;						// 目标外接矩形

	VE_FeatureInfo	m_vObjFea;						// 目标特性
}VE_ObjectInfo;

//目标 轨迹信息
typedef struct _VE_ObjectTrajectory_
{
	veu32 m_uiNum;									// 轨迹数量
	VE_ObjectInfo m_vObjInfo[VE_MAX_TRAJECTORY_LEN];// 轨迹内容

	veu32 m_uiImageNum;
	veu8  m_uiImageMaxIndex;
	IplImage *m_pObjSubImage[VE_MAX_OBJSUBIMAGE_LEN];//目标子图缓存
	IplImage *m_pObjSubImageEx[VE_MAX_OBJSUBIMAGE_LEN];//目标子图缓存扩大版
} VE_ObjectTrajectory;


//车牌有关信息
typedef struct _VE_PLATEINFO_
{
	ves8			szPlate[32];					// 号牌
	ves8			szColor[32];					// 颜色
	veu32			lPositon[4];					// 位置：left, top,right, bottomz
	veu64			lType;							// 车牌类型
	vef32			fMatScore;						// 定位准确性分值
	vef32			fRecScore;						// 识别准确性分值
	veu64			lTimeMs;
	veu32			charNum;						//车牌字符个数 
	veu32			charPositon[10][4];				//车牌每个字符位置
	ves8*			pGrayPlate;
	veu64			nReserved[10];
}VE_PLATEINFO,VE_PlateInfo;

//车辆有关属性
typedef struct _VE_VehicleInfo_
{
	VE_PlateInfo plateinfo;						//车牌号码//
	ves8		cVehicleColor[16];				//车辆颜色//
	veu8		nVehicleColorScore;				//车辆颜色,满分100分//
	veu8		nVehicleColorIndex;				//车辆颜色索引

	ves8		cVehicleColorDarkOrLight[10];	//车辆深浅色//

	veu8		nVehicleTypeScore;				//车辆品牌（子品牌）得分,满分100分//
	veu32		nVehicleTypeIndex;				//车辆品牌索引
	ves8		cVehicleType[16];				//车型//
	ves8		cVehicleBrand[50];				//车辆品牌//
	veu8		nVehicleBrandScore;				//车辆品牌（子品牌）得分,满分100分//
	veu32		nVehicleBrandIndex;				//车辆品牌索引
	ves8		cVehicleSubBrand[50];			//车辆子品牌//
	veu16		nVehicleFeatureNum;				//特征维度//
	ves8		fVehicleCarFeature[4096];		//车辆子图像特征//
	ves8		fVehicleImgFeature[4096];		//挡风玻璃子特征//
	veu8        nVehicleDirection;				//车辆方向  == 0? "车头":"车尾"

	VE_Rect		rcVehicleChouzhihe[4];			//抽纸盒位置
	veu8		nVehicleChouzhiheNum;			//抽纸盒个数

	VE_Rect		rcVehicleDiaozhui;				//吊坠位置
	veu8		nVehicleDiaozhuiNum;			//吊坠个数

	VE_Rect		rcVehicleZheyangban[2];			//遮阳板位置
	veu8		nVehicleZheyangbanNum;			//遮阳板个数

	VE_Rect		rcVehicleInspec[2];				//年检标位置
	veu8		nVehicleInspecNum;				//年检标个数

	VE_Rect		rcVehicleTopWnd;				//天窗位置
	veu8		nVehicleTopWndNum;				//天窗个数

	VE_Rect		rcVehicleTopCarrier[2];			//行李架位置
	veu8		nVehicleTopCarrierNum;			//行李架个数

	VE_Rect		rcVehicleLight[2];				//车灯位置
	veu8		nVehicleLightNum;				//车灯个数

	VE_Rect		rcVehicleWnd[2];				//挡风玻璃位置
	veu8		nVehicleWndNum;					//挡风玻璃个数

	VE_Rect		rcVehicleLayon;					//摆件位置
	veu8		nVehicleLayonNum;				//摆件个数

	VE_Rect		rcHuman[2];						//前排人员位置
	veu8		nHumanNum;						//前排人员个数

	VE_Rect		rcPassPaper;					//出入证位置
	veu8		nPassPaperNum;					//出入证个数

	veu8		nVehicleSafeBeltyMainScore;		//主驾驶系安全带得分 -1表示没有检测到，0表示没系安全带，100表示肯定系了安全带
	veu8		nVehicleSafeBeltyCopilotScore;	//副驾驶系安全带得分 -1表示没有检测到，0表示没系安全带，100表示肯定系了安全带
	veu8		nVehicleDriverOnCallScore;		//主驾驶开车打电话得分 -1表示没有检测到，0表示没有打电话，100表示肯定在打电话

	ves8		cVehicleOcr[32];				//车上用OCR识别出来的一些字
	ves8		reseverd[10];					//预留//
}VE_VehicleInfo;

//行人有关属性
typedef struct _VE_HumanInfo_
{
	VE_TinyFeature tfHumanFea[MAX_HUMANFEATURE_NUMBER];				//行人特征

	//0:上衣颜色 白;黑;红;黄;蓝;绿;紫;棕;灰;橙;未知;
	//1:裤子颜色 白;黑;红;黄;蓝;绿;紫;棕;灰;橙;未知;
	//2:发型     长发;短发;未知;
	//3:上衣类型 长袖;短袖;未知;
	//4:裤子类型 长裤;短裤;未知;
	//5:背包类型 未背包;单肩包;双肩包;手提包;未知;
	//6:年龄类型 未成年;青年;中年;老年;未知;
	//7:性别     男人;女人;未知;
	//8:种族类型 黄种人;白人;黑人;未知;
	//9:帽子类型 未戴帽;鸭舌帽;其他帽;未知;

	ves8		reseverd[10];					//预留//
}VE_HumanInfo;

//自然界物体属性
typedef struct _VE_NatureInfo_
{
	ves8 name[32];								//物体名称
	veu8 type;									//类别
	veu8 pose;									//姿态
	veu8 truncated;								//是否裁切
	veu8 score;									//分值

	ves8		reseverd[10];					//预留//
}VE_NatureInfo;

//目标描述
typedef struct _VE_Object_
{
	veu32					m_uiObjId;				// 目标ID
	veu32                   m_nTrackNum;            // 跟踪帧数
	veu32					m_nMissNum;				// 匹配不上的帧数
	veu32					m_nMatchNum;			// 检测到的帧数
	vebool					m_bOccupied;			// 目标是否被占用
	vebool					m_bCaptured;			// 目标是否被抓拍
	VE_ObjectType 			m_uiObjType;			// 目标类型
	VE_ObjectTypeEx			m_uiObjTypeEx;			// 目标精确类型
	VE_ObjectStatus			m_uiObjStatus;			// 目标状态

	VE_Point			    m_vObjCenter;			// 中心坐标
	VE_Point				m_vObjTrackCenterOffset;// 跟踪角点的位置便宜
	VE_Point				m_vObjTrackCenter;		// 跟踪角点的位置

	VE_Point				m_vObjRealCenter;		// 真实中心坐标	
	VE_Rect					m_vObjRect;				// 目标外接矩形		
	veu32 					m_uiBlobIndex;			// 匹配的团块索引				
	vef32 					m_fRealWidth;			// 实际宽度
	veu64                   m_uiTimeStamp;			// 时间戳

	VE_ObjectTrajectory		m_vObjTrajectory;		// 目标轨迹信息

	VE_VehicleInfo			m_vVehicleInfo;			// 车辆属性
	VE_HumanInfo			m_vHumanInfo;			// 行人属性
	VE_NatureInfo			m_vNatureInfo;			// 自然物体属性
	vebool					m_bRecoInfo;
}VE_Object;

//目标列表
typedef struct _VE_ObjectList_
{
	veu32		m_uiObjNum;							//有效目标个数							
	veu32		m_uiFrameNum;						//帧号
	veu64       m_uTimeStamp;						//时间戳
	VE_Object	m_Object[VE_MAX_OBJECT_NUMBER];		//目标列表
}VE_ObjectList;

//目标识别任务
typedef struct _VE_ObjectRecoTask_
{
	VE_Object			m_vObjInfo;					//目标信息
	VE_ChannelDescript	m_vVideoChannelInfo;		//视频通道信息
	IplImage			*m_pObjImage;				//目标图像
	IplImage			*m_pObjImageEx;				//目标图像扩大版
}VE_ObjectRecoTask;
//车辆方向
typedef enum _VE_VehicleDirect_
{
	VE_VehicleDirectHead = 0,						// 车头方向
	VE_VehicleDirectTail = 1,						// 车尾方向
	VE_VehicleDirectHeadAndTail = 2,				// 车头和车尾方向
	VE_VehicleDirectFromLeft = 4,					// 左侧行驶往右侧
	VE_VehicleDirectFromRight = 5,					// 右侧行驶往左侧
	VE_VehicleDirectFromLeftAndRight = 6			// 左右行驶
}VE_VehicleDirect;


#define VE_CD_TABLE_LEN 2000						// 标定表的维数
typedef struct _VE_Coordinator_
{
	ves32 bCoordOnOff;								// 启用标定开关
	vef32 fRealWidth;								// 实际宽度
	vef32 pixTable[VE_CD_TABLE_LEN];				// 一个像素的大地距离
}VE_Coordinator;

typedef enum _VE_WorkMode_
{
	VE_WORK_MODE_KAKOU = 0x0000,					// 卡口
	VE_WORK_MODE_DZJC  = 0x0001,					// 电子警察
	VE_WORK_MODE_EVENT = 0x0002,					// 事件检测
	VE_WORK_MODE_SUV =	 0x0003,					// 监控场景
} VE_WorkMode;

typedef struct  _VE_DTCordParam_
{
	ves32 m_nWidth;
	ves32 m_nHeight;
	ves32 *m_mCoorTableX;
	ves32 *m_mCoorTableY;
	ves32 *m_mCoorTableDis2Pix;
	vef32 *m_mCoorTablePix2Dis;
	vef32 m_fTranferVector[8];
}VE_DTCordParam;

typedef struct  _VE_DTAdvParam_
{
	ves32 m_nDayDiffThreth;							// 白天差分阈值 (如配置为0 则使用自适应方式)
	ves32 m_nNightDiffThreth;						// 夜间差分阈值 (如配置为0 则使用自适应方式)
	vef32 m_nLearnRate;								// 背景学习速率 (0-1023 越大背景更新速率越快)
	vef32 m_nBgLearnFrames;							// 背景学习帧数 (1-360 帧)
}VE_DTAdvParam;

#define N_MAX_DETECT_REGION 10						// 最大检测区域
#define N_MAX_INVALID_RGNS  10						// 最大无效检测区域

typedef struct  _VE_DTParam_
{
	VE_Coordinator	vCoordinator;					// 标定参数

	ves32			nMaxTrackFrame;					// 最大跟踪帧数(默认：1000 范围：100 - 5000)
	ves32			bEdgeEnable;                    // 边界起效(0表示开启边界消除，1表示关闭边界消除)
	ves32			nBoderEdge[4];                  // 目标的四个边界  左上右下(四个边界的位置)
	vef32			fMinCarWidth;                   // 最小车辆宽度(最小车辆宽度，当vCoordinator配置了之后生效)
	vef32			fMaxCarWidth;                   // 最大车辆宽度(最小车辆宽度，当vCoordinator配置了之后生效)

	ves32			bVehicleValidPolygonOnOff;		// 启用车辆有效多边形区域开关
	VE_POLYGON		vVehicleValidPolygon;			// 车辆有效多边形区域（默认：图像下四分之三）
	VE_GRID			vGrid;							// 标定网格（用于图像标定的网格线，也可以不用配置）
	VE_LINE			vTrigerLine;					// 触发线（用于抓拍的触发线，实际中根据需要使用，也可以不采用抓拍线进行抓拍）

	ves32			nInValidPolygonNum;				// 屏蔽区域个数，最多十个
	VE_POLYGON		vInValidPolygon[10];			// 屏蔽区域
}VE_DTParam;


typedef struct  _VE_AjustPictureParam_
{
	vef32 fdist_vertical;							//两条折线垂直方向实际距离
	vef32 fdist_horizon;							//两条折线水平方向实际距离
	vef32 fCameraHeight;							//摄像机高度 单位 米                          //新加 2.160527  cqs
	vef32 fleftRatio;								//摄像机左半部分视野占图像宽度比例 范围0~1    //新加 2.160527  cqs
	VE_CURVES   vAdjustcurves[2];					//矫正折线
}VE_AjustPictParam;

typedef enum _VE_VDMethod_
{
	VE_VDMethodPlate = 0,							// 基于车牌的算法
	VE_VDMethodModel = 1,							// 基于模型的算法
	VE_VDMethodObject = 2,							// 基于目标的算法
	VE_VDMethodPlateAndModel = 3,					// 基于车牌和模型的算法
	VE_VDMethodPlateAndObject = 4,					// 基于车牌和目标的算法
	VE_VDMethodPlateAndModelAndObject = 5,			// 基于车牌、模型、运动目标三算法同时启用
}VE_VDMethod;


typedef struct _VE_ModelDetectParam_
{
	ves32  method;
}VE_ModelDetectParam;

typedef struct _VEHICLEDETECT_PARAM_
{
	vebool	bVehiclePlate;			//车牌识别是否开启 
	vebool	bVehicleColor;			//车辆颜色识别是否开启
	vebool	bVehicleType;			//车型识别是否开启
	vebool	bVehicleBrand;			//车辆品牌识别是否开启
	vebool	bVehicle360Brand;		//车辆品牌识别是否开启
	vebool	bVehicleImgFeature;		//车辆特征是否开启//

	vebool	bUseCnn;				//基于CNN进行识别
	vebool	bUseSVM;				//基于SVM进行识别
	vebool	bUseBoth;				//基于卓视增强合成算法识别
	vebool	bRecoHeadOrTail;		//是否内部自动识别车头和车尾

	vebool	bVehicleInspec;			//年检标个数检测
	vebool	bVehicleMirror;			//驾驶窗检测
	vebool	bVehicleSafeBelty;		//安全带检测
	vebool	bVehicleZheyangban;		//遮阳板检测
	vebool	bVehicleDiaozhui;		//吊坠检测
	vebool	bVehicleChouzhihe;		//抽纸盒检测

	vebool	bNoPlateCarEnable;		//无牌车检测使能
	vef32	fZoomRatio;				//无牌车检测缩放系数
	vef32	fNoPlateDetectScore;	//无牌车检测的检测分值
	vef32	fNoPlateRecoScore;		//无牌车检测的车型分值

	vef32	fSvmCnnThreshofHead;
	vef32	fSvmCnnThreshofTail;

	////////////////////////////////////////////
	ves8	strPlateRecoIniPath[256];		//veplate.ini
	ves8	strCnnModelWithPlatePath[256];  //v.txt,v.mod v.men v.dep
	ves8    strCnnColorPath[256];			//c.txt,c.mod.c.men.c.dep
	ves8    strCnnModelNoPlatePath[256];	//n.txt,n.mod n.men n.dep
	ves8    strCnnModelTypePath[256];		//n.txt,n.mod n.men n.dep
	ves8    strCnnModel360BrandPath[256];	//c360.txt,c360.mod c360.men c360.dep
	ves8    strCnnModel360ColorPath[256];	//color360.txt,color360.mod color360.men color360.dep
	ves8    strCnnModelImgFeaturePath[256];	//imgFeature.txt,imgFeature.mod imgFeature.men imgFeature.dep
	ves8    strFeaModelPath[256];			//f.dep f.mod f.txt 
	veu8	nGPUId;
	veu8	nMethodID;
	ves8	reseverd[10];
}VehicleRecoParam;

typedef struct _MODELDETECT_PARAM_
{
	ves8	nDetectAlg;
	ves8	nThresh;
	ves8	nJumpFrame;
	ves8    strModelDetectPath[256];			//p.txt,p.mod p.men p.dep
	veu8	nGPUId;
}ModelDetectParam;


typedef struct _HUMANRECO_PARAM_
{
	vebool	bHumanReco;						//是否开启行人识别
	vebool	bUseCnn;						//基于CNN进行识别

	////////////////////////////////////////////
	ves8    strCnnModelPath[256];			//p.txt,p.mod p.men p.dep
	veu8	nGPUId;
	veu8	nMethodID;
	ves8	reseverd[10];
}HumanRecoParam;

typedef struct _VE_VTConfigParam_
{
	vef32			fObjectCutRatio;				// 目标切图系数，默认为1，越大切图范围越大
	VE_DTParam		vDetTrkParam;					// 检测跟踪参数
	VE_AjustPictParam vAjustParam;					// 图像矫正参数
	VehicleRecoParam vVehicleRecoParam;				// 车辆识别参数
	HumanRecoParam	 vHumanRecoParam;				// 行人识别参数
	ModelDetectParam vModelDetectParam;				// 检测模型参数

	ves8	reseverd[100];
}VE_VTConfigParam;


#endif
