/**
* @file VeObjectDefine.h  ������Ŀ���йصĻ������ݽṹ
*
* ��Ȩ����(c) 2016 - 2021 ����׿����ͨ�Ƽ��������ι�˾
*
*
* @author ���ά <67904662@qq.com>
* @version 1.0
* @date 2016.10.3
*/


#ifndef _VISIONE_OBJECT_DEFINE_H_
#define _VISIONE_OBJECT_DEFINE_H_

#include "VeStdInt.h"
#include "VeCv.h"
#include "VeBaseType.h"

#define VE_MAX_OBJECT_NUMBER				100       //���Ŀ�����
#define VE_MAX_OBJTRK_NUM		VE_MAX_OBJECT_NUMBER  //������Ŀ���ʽ 
#define VE_MAX_TRAJECTORY_LEN				256       //���켣����
#define VE_MAX_OBJSUBIMAGE_LEN				5		  //Ŀ����ͼ�����ʱ��������

///////////////// Ŀ��OBJ//////////////////////////

//Ŀ��״̬
typedef enum _VE_ObjectStatus_
{
	VE_OBJ_STATUS_DELETE = 0x0001,					// ɾ��Ŀ��
	VE_OBJ_STATUS_INIT = 0x0002,					// ��ʼ��Ŀ��
	VE_OBJ_STATUS_MACHED = 0x0004,					// ƥ��Ŀ��
	VE_OBJ_STATUS_PREDICTED = 0x0008,				// Ԥ��Ŀ��
	VE_OBJ_STATUS_STABLE = 0x0010,					// �ȶ�Ŀ��
	VE_OBJ_STATUS_ABNML = 0x0020,					// �쳣Ŀ��
	VE_OBJ_STATUS_COLLISION = 0x0040,				// ��ײĿ��
	VE_OBJ_STATUS_RELIABLE = 0x0080,				// ��������		
	VE_OBJ_STATUS_VANISH = 0x0100					// ��ʧ״̬
} VE_ObjectStatus;

//Ŀ������
typedef enum _VE_ObjectType_
{
	VE_OBJ_TYPE_UNKNOWN = 0x0000,					// δ֪����
	VE_OBJ_TYPE_HUMAN = 0x0001,						// ��
	VE_OBJ_TYPE_VERHICLE = 0x0002,					// ��
	VE_OBJ_TYPE_NOCAR = 0x0004,						// �ǻ�����
	VE_OBJ_TYPE_FACE = 0x0008,						// ����
	VE_OBJ_TYPE_WHEEL = 0x00016,					// ����
	VE_OBJ_TYPE_OTHER = 0x0032,						// ������Ȼ������
} VE_ObjectType;



//Ŀ�꾫ȷ����
typedef enum _VE_ObjectTypeEx_
{
	ve_obj_typeex_unkown = 0x0000,					// δ֪����
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
	"����",
	"���г�",
	"��������",
	"�γ�",
	"Ħ�г�",
	"��",
	"����",
	"���ֳ�",
	"���Ӿ�",
	"��ֽ��",
	"������",
	"���粣��",
	"�Ҽ�",
	"����",
	"����֤",
	"��ȫ��",
	"����",
	"����",
	"�촰",
	"�����",
	"���ֱ�",
	"����",
	"�ڼ�",
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
//��������
typedef enum _VE_HumanFeatureType_
{
	VE_HUMFEA_AGETYPE = 0x0000,					//�������� δ����;����;����;����;δ֪;
	VE_HUMFEA_BAGTYPE = 0x0001,					//�������� δ����;�����;˫���;�����;δ֪;
	VE_HUMFEA_CLOUSECOLOR = 0x0002,				//������ɫ ��;��;��;��;��;��;��;��;��;��;δ֪;
	VE_HUMFEA_CLOUSETYPE = 0x0003,				//�������� ����;����;δ֪;
	VE_HUMFEA_HAIRTYPE = 0x0004,				//����     ����;�̷�;δ֪;
	VE_HUMFEA_HAPTYPE = 0x0005,					//ñ������ δ��ñ;Ѽ��ñ;����ñ;δ֪;
	VE_HUMFEA_RACETYPE = 0x0006,				//�������� ������;����;����;δ֪;
	VE_HUMFEA_SEXTYPE = 0x0007,					//�������� δ����;����;����;����;δ֪;
	VE_HUMFEA_TROUSECOLOR = 0x0008,				//������ɫ ��;��;��;��;��;��;��;��;��;��;δ֪;
	VE_HUMFEA_TROUSETYPE = 0x0009,				//�������� ����;�̿�;δ֪;
	VE_HUMFEA_MAX = 0x0010,
} VE_HumanFeatureType;

typedef struct _VE_FeatureInfo_
{
	//����
	ves8  cPlate[16];

	//����
	VE_TinyFeature tfBrand;
	VE_TinyFeature tfColor;
	VE_TinyFeature tfType;

	VE_TinyFeature tfHumanFea[MAX_HUMANFEATURE_NUMBER];				//��������

	ves8  reseverd[10];						//Ԥ��//  

}VE_FeatureInfo;

//Ŀ�� �����켣��Ϣ
typedef struct _VE_ObjectInfo_
{
	veu64			m_uiTimeStamp;					// ʱ���
	VE_ObjectType	m_uiObjType;					// Ŀ������
	VE_ObjectTypeEx	m_uiObjTypeEx;					// Ŀ�꾫ȷ����
	VE_ObjectStatus m_uiObjStatus;	    			// Ŀ��״̬
	VE_Point		m_vObjCenter;					// ��������				
	VE_Rect			m_vObjRect;						// Ŀ����Ӿ���

	VE_FeatureInfo	m_vObjFea;						// Ŀ������
}VE_ObjectInfo;

//Ŀ�� �켣��Ϣ
typedef struct _VE_ObjectTrajectory_
{
	veu32 m_uiNum;									// �켣����
	VE_ObjectInfo m_vObjInfo[VE_MAX_TRAJECTORY_LEN];// �켣����

	veu32 m_uiImageNum;
	veu8  m_uiImageMaxIndex;
	IplImage *m_pObjSubImage[VE_MAX_OBJSUBIMAGE_LEN];//Ŀ����ͼ����
	IplImage *m_pObjSubImageEx[VE_MAX_OBJSUBIMAGE_LEN];//Ŀ����ͼ���������
} VE_ObjectTrajectory;


//�����й���Ϣ
typedef struct _VE_PLATEINFO_
{
	ves8			szPlate[32];					// ����
	ves8			szColor[32];					// ��ɫ
	veu32			lPositon[4];					// λ�ã�left, top,right, bottomz
	veu64			lType;							// ��������
	vef32			fMatScore;						// ��λ׼ȷ�Է�ֵ
	vef32			fRecScore;						// ʶ��׼ȷ�Է�ֵ
	veu64			lTimeMs;
	veu32			charNum;						//�����ַ����� 
	veu32			charPositon[10][4];				//����ÿ���ַ�λ��
	ves8*			pGrayPlate;
	veu64			nReserved[10];
}VE_PLATEINFO,VE_PlateInfo;

//�����й�����
typedef struct _VE_VehicleInfo_
{
	VE_PlateInfo plateinfo;						//���ƺ���//
	ves8		cVehicleColor[16];				//������ɫ//
	veu8		nVehicleColorScore;				//������ɫ,����100��//
	veu8		nVehicleColorIndex;				//������ɫ����

	ves8		cVehicleColorDarkOrLight[10];	//������ǳɫ//

	veu8		nVehicleTypeScore;				//����Ʒ�ƣ���Ʒ�ƣ��÷�,����100��//
	veu32		nVehicleTypeIndex;				//����Ʒ������
	ves8		cVehicleType[16];				//����//
	ves8		cVehicleBrand[50];				//����Ʒ��//
	veu8		nVehicleBrandScore;				//����Ʒ�ƣ���Ʒ�ƣ��÷�,����100��//
	veu32		nVehicleBrandIndex;				//����Ʒ������
	ves8		cVehicleSubBrand[50];			//������Ʒ��//
	veu16		nVehicleFeatureNum;				//����ά��//
	ves8		fVehicleCarFeature[4096];		//������ͼ������//
	ves8		fVehicleImgFeature[4096];		//���粣��������//
	veu8        nVehicleDirection;				//��������  == 0? "��ͷ":"��β"

	VE_Rect		rcVehicleChouzhihe[4];			//��ֽ��λ��
	veu8		nVehicleChouzhiheNum;			//��ֽ�и���

	VE_Rect		rcVehicleDiaozhui;				//��׹λ��
	veu8		nVehicleDiaozhuiNum;			//��׹����

	VE_Rect		rcVehicleZheyangban[2];			//������λ��
	veu8		nVehicleZheyangbanNum;			//���������

	VE_Rect		rcVehicleInspec[2];				//����λ��
	veu8		nVehicleInspecNum;				//�������

	VE_Rect		rcVehicleTopWnd;				//�촰λ��
	veu8		nVehicleTopWndNum;				//�촰����

	VE_Rect		rcVehicleTopCarrier[2];			//�����λ��
	veu8		nVehicleTopCarrierNum;			//����ܸ���

	VE_Rect		rcVehicleLight[2];				//����λ��
	veu8		nVehicleLightNum;				//���Ƹ���

	VE_Rect		rcVehicleWnd[2];				//���粣��λ��
	veu8		nVehicleWndNum;					//���粣������

	VE_Rect		rcVehicleLayon;					//�ڼ�λ��
	veu8		nVehicleLayonNum;				//�ڼ�����

	VE_Rect		rcHuman[2];						//ǰ����Աλ��
	veu8		nHumanNum;						//ǰ����Ա����

	VE_Rect		rcPassPaper;					//����֤λ��
	veu8		nPassPaperNum;					//����֤����

	veu8		nVehicleSafeBeltyMainScore;		//����ʻϵ��ȫ���÷� -1��ʾû�м�⵽��0��ʾûϵ��ȫ����100��ʾ�϶�ϵ�˰�ȫ��
	veu8		nVehicleSafeBeltyCopilotScore;	//����ʻϵ��ȫ���÷� -1��ʾû�м�⵽��0��ʾûϵ��ȫ����100��ʾ�϶�ϵ�˰�ȫ��
	veu8		nVehicleDriverOnCallScore;		//����ʻ������绰�÷� -1��ʾû�м�⵽��0��ʾû�д�绰��100��ʾ�϶��ڴ�绰

	ves8		cVehicleOcr[32];				//������OCRʶ�������һЩ��
	ves8		reseverd[10];					//Ԥ��//
}VE_VehicleInfo;

//�����й�����
typedef struct _VE_HumanInfo_
{
	VE_TinyFeature tfHumanFea[MAX_HUMANFEATURE_NUMBER];				//��������

	//0:������ɫ ��;��;��;��;��;��;��;��;��;��;δ֪;
	//1:������ɫ ��;��;��;��;��;��;��;��;��;��;δ֪;
	//2:����     ����;�̷�;δ֪;
	//3:�������� ����;����;δ֪;
	//4:�������� ����;�̿�;δ֪;
	//5:�������� δ����;�����;˫���;�����;δ֪;
	//6:�������� δ����;����;����;����;δ֪;
	//7:�Ա�     ����;Ů��;δ֪;
	//8:�������� ������;����;����;δ֪;
	//9:ñ������ δ��ñ;Ѽ��ñ;����ñ;δ֪;

	ves8		reseverd[10];					//Ԥ��//
}VE_HumanInfo;

//��Ȼ����������
typedef struct _VE_NatureInfo_
{
	ves8 name[32];								//��������
	veu8 type;									//���
	veu8 pose;									//��̬
	veu8 truncated;								//�Ƿ����
	veu8 score;									//��ֵ

	ves8		reseverd[10];					//Ԥ��//
}VE_NatureInfo;

//Ŀ������
typedef struct _VE_Object_
{
	veu32					m_uiObjId;				// Ŀ��ID
	veu32                   m_nTrackNum;            // ����֡��
	veu32					m_nMissNum;				// ƥ�䲻�ϵ�֡��
	veu32					m_nMatchNum;			// ��⵽��֡��
	vebool					m_bOccupied;			// Ŀ���Ƿ�ռ��
	vebool					m_bCaptured;			// Ŀ���Ƿ�ץ��
	VE_ObjectType 			m_uiObjType;			// Ŀ������
	VE_ObjectTypeEx			m_uiObjTypeEx;			// Ŀ�꾫ȷ����
	VE_ObjectStatus			m_uiObjStatus;			// Ŀ��״̬

	VE_Point			    m_vObjCenter;			// ��������
	VE_Point				m_vObjTrackCenterOffset;// ���ٽǵ��λ�ñ���
	VE_Point				m_vObjTrackCenter;		// ���ٽǵ��λ��

	VE_Point				m_vObjRealCenter;		// ��ʵ��������	
	VE_Rect					m_vObjRect;				// Ŀ����Ӿ���		
	veu32 					m_uiBlobIndex;			// ƥ����ſ�����				
	vef32 					m_fRealWidth;			// ʵ�ʿ��
	veu64                   m_uiTimeStamp;			// ʱ���

	VE_ObjectTrajectory		m_vObjTrajectory;		// Ŀ��켣��Ϣ

	VE_VehicleInfo			m_vVehicleInfo;			// ��������
	VE_HumanInfo			m_vHumanInfo;			// ��������
	VE_NatureInfo			m_vNatureInfo;			// ��Ȼ��������
	vebool					m_bRecoInfo;
}VE_Object;

//Ŀ���б�
typedef struct _VE_ObjectList_
{
	veu32		m_uiObjNum;							//��ЧĿ�����							
	veu32		m_uiFrameNum;						//֡��
	veu64       m_uTimeStamp;						//ʱ���
	VE_Object	m_Object[VE_MAX_OBJECT_NUMBER];		//Ŀ���б�
}VE_ObjectList;

//Ŀ��ʶ������
typedef struct _VE_ObjectRecoTask_
{
	VE_Object			m_vObjInfo;					//Ŀ����Ϣ
	VE_ChannelDescript	m_vVideoChannelInfo;		//��Ƶͨ����Ϣ
	IplImage			*m_pObjImage;				//Ŀ��ͼ��
	IplImage			*m_pObjImageEx;				//Ŀ��ͼ�������
}VE_ObjectRecoTask;
//��������
typedef enum _VE_VehicleDirect_
{
	VE_VehicleDirectHead = 0,						// ��ͷ����
	VE_VehicleDirectTail = 1,						// ��β����
	VE_VehicleDirectHeadAndTail = 2,				// ��ͷ�ͳ�β����
	VE_VehicleDirectFromLeft = 4,					// �����ʻ���Ҳ�
	VE_VehicleDirectFromRight = 5,					// �Ҳ���ʻ�����
	VE_VehicleDirectFromLeftAndRight = 6			// ������ʻ
}VE_VehicleDirect;


#define VE_CD_TABLE_LEN 2000						// �궨���ά��
typedef struct _VE_Coordinator_
{
	ves32 bCoordOnOff;								// ���ñ궨����
	vef32 fRealWidth;								// ʵ�ʿ��
	vef32 pixTable[VE_CD_TABLE_LEN];				// һ�����صĴ�ؾ���
}VE_Coordinator;

typedef enum _VE_WorkMode_
{
	VE_WORK_MODE_KAKOU = 0x0000,					// ����
	VE_WORK_MODE_DZJC  = 0x0001,					// ���Ӿ���
	VE_WORK_MODE_EVENT = 0x0002,					// �¼����
	VE_WORK_MODE_SUV =	 0x0003,					// ��س���
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
	ves32 m_nDayDiffThreth;							// ��������ֵ (������Ϊ0 ��ʹ������Ӧ��ʽ)
	ves32 m_nNightDiffThreth;						// ҹ������ֵ (������Ϊ0 ��ʹ������Ӧ��ʽ)
	vef32 m_nLearnRate;								// ����ѧϰ���� (0-1023 Խ�󱳾���������Խ��)
	vef32 m_nBgLearnFrames;							// ����ѧϰ֡�� (1-360 ֡)
}VE_DTAdvParam;

#define N_MAX_DETECT_REGION 10						// ���������
#define N_MAX_INVALID_RGNS  10						// �����Ч�������

typedef struct  _VE_DTParam_
{
	VE_Coordinator	vCoordinator;					// �궨����

	ves32			nMaxTrackFrame;					// ������֡��(Ĭ�ϣ�1000 ��Χ��100 - 5000)
	ves32			bEdgeEnable;                    // �߽���Ч(0��ʾ�����߽�������1��ʾ�رձ߽�����)
	ves32			nBoderEdge[4];                  // Ŀ����ĸ��߽�  ��������(�ĸ��߽��λ��)
	vef32			fMinCarWidth;                   // ��С�������(��С������ȣ���vCoordinator������֮����Ч)
	vef32			fMaxCarWidth;                   // ��������(��С������ȣ���vCoordinator������֮����Ч)

	ves32			bVehicleValidPolygonOnOff;		// ���ó�����Ч��������򿪹�
	VE_POLYGON		vVehicleValidPolygon;			// ������Ч���������Ĭ�ϣ�ͼ�����ķ�֮����
	VE_GRID			vGrid;							// �궨��������ͼ��궨�������ߣ�Ҳ���Բ������ã�
	VE_LINE			vTrigerLine;					// �����ߣ�����ץ�ĵĴ����ߣ�ʵ���и�����Ҫʹ�ã�Ҳ���Բ�����ץ���߽���ץ�ģ�

	ves32			nInValidPolygonNum;				// ����������������ʮ��
	VE_POLYGON		vInValidPolygon[10];			// ��������
}VE_DTParam;


typedef struct  _VE_AjustPictureParam_
{
	vef32 fdist_vertical;							//�������ߴ�ֱ����ʵ�ʾ���
	vef32 fdist_horizon;							//��������ˮƽ����ʵ�ʾ���
	vef32 fCameraHeight;							//������߶� ��λ ��                          //�¼� 2.160527  cqs
	vef32 fleftRatio;								//�������벿����Ұռͼ���ȱ��� ��Χ0~1    //�¼� 2.160527  cqs
	VE_CURVES   vAdjustcurves[2];					//��������
}VE_AjustPictParam;

typedef enum _VE_VDMethod_
{
	VE_VDMethodPlate = 0,							// ���ڳ��Ƶ��㷨
	VE_VDMethodModel = 1,							// ����ģ�͵��㷨
	VE_VDMethodObject = 2,							// ����Ŀ����㷨
	VE_VDMethodPlateAndModel = 3,					// ���ڳ��ƺ�ģ�͵��㷨
	VE_VDMethodPlateAndObject = 4,					// ���ڳ��ƺ�Ŀ����㷨
	VE_VDMethodPlateAndModelAndObject = 5,			// ���ڳ��ơ�ģ�͡��˶�Ŀ�����㷨ͬʱ����
}VE_VDMethod;


typedef struct _VE_ModelDetectParam_
{
	ves32  method;
}VE_ModelDetectParam;

typedef struct _VEHICLEDETECT_PARAM_
{
	vebool	bVehiclePlate;			//����ʶ���Ƿ��� 
	vebool	bVehicleColor;			//������ɫʶ���Ƿ���
	vebool	bVehicleType;			//����ʶ���Ƿ���
	vebool	bVehicleBrand;			//����Ʒ��ʶ���Ƿ���
	vebool	bVehicle360Brand;		//����Ʒ��ʶ���Ƿ���
	vebool	bVehicleImgFeature;		//���������Ƿ���//

	vebool	bUseCnn;				//����CNN����ʶ��
	vebool	bUseSVM;				//����SVM����ʶ��
	vebool	bUseBoth;				//����׿����ǿ�ϳ��㷨ʶ��
	vebool	bRecoHeadOrTail;		//�Ƿ��ڲ��Զ�ʶ��ͷ�ͳ�β

	vebool	bVehicleInspec;			//����������
	vebool	bVehicleMirror;			//��ʻ�����
	vebool	bVehicleSafeBelty;		//��ȫ�����
	vebool	bVehicleZheyangban;		//��������
	vebool	bVehicleDiaozhui;		//��׹���
	vebool	bVehicleChouzhihe;		//��ֽ�м��

	vebool	bNoPlateCarEnable;		//���Ƴ����ʹ��
	vef32	fZoomRatio;				//���Ƴ��������ϵ��
	vef32	fNoPlateDetectScore;	//���Ƴ����ļ���ֵ
	vef32	fNoPlateRecoScore;		//���Ƴ����ĳ��ͷ�ֵ

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
	vebool	bHumanReco;						//�Ƿ�������ʶ��
	vebool	bUseCnn;						//����CNN����ʶ��

	////////////////////////////////////////////
	ves8    strCnnModelPath[256];			//p.txt,p.mod p.men p.dep
	veu8	nGPUId;
	veu8	nMethodID;
	ves8	reseverd[10];
}HumanRecoParam;

typedef struct _VE_VTConfigParam_
{
	vef32			fObjectCutRatio;				// Ŀ����ͼϵ����Ĭ��Ϊ1��Խ����ͼ��ΧԽ��
	VE_DTParam		vDetTrkParam;					// �����ٲ���
	VE_AjustPictParam vAjustParam;					// ͼ���������
	VehicleRecoParam vVehicleRecoParam;				// ����ʶ�����
	HumanRecoParam	 vHumanRecoParam;				// ����ʶ�����
	ModelDetectParam vModelDetectParam;				// ���ģ�Ͳ���

	ves8	reseverd[100];
}VE_VTConfigParam;


#endif
