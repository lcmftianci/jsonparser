#ifndef _VIDEO_INFO_JSON_MANAGER_H__
#define _VIDEO_INFO_JSON_MANAGER_H__


/*
VideoJsonInfoManager �����

һ��д�����һ����ȡ����

һ·��Ƶһ���ļ��У�

ÿ���ļ��������кܶఴʱ�䴴�����ļ���(һ��Сʱһ���ļ���)

ʱ���ļ������ÿ���ļ�����һ������ÿ�����󶼿�����ǰ�˳����ȡ����ʾ

ֻҪ���������ͻᴴ���µ��ļ���

�ж��ļ����Ƿ���ڣ�������ڲ��ٴ���ֱ�Ӷ�ȡ�����json����

�������һ�������ȡ����ոմ洢���ļ��أ���ȡ�ļ�������ʱ��
*/

#include <iostream>
#include <queue>
#include <vector>
#include "VeObjectDefine.h"


//class CBase64;

#if 0
typedef struct  
{
	std::vector<VE_VehicleInfo>			m_arrVehicleInfo;			// ��������
	std::vector<VE_HumanInfo>			m_arrHumanInfo;				// ��������
	std::vector<VE_NatureInfo>			m_arrNatureInfo;			// ��Ȼ��������
	//IplImage *m_pObjSubImage[VE_MAX_OBJSUBIMAGE_LEN];//Ŀ����ͼ����
}VideoInfo;
#endif


class VideoInfoJsonManager
{
public:
	/*
	 *if model == 0 saver
	 *else model  parser
	 * */
	VideoInfoJsonManager(int model = 0);

	/*
	 *��������
	 * */
	virtual ~VideoInfoJsonManager();

public:		
	/*��ȡ����*/
	int InsertJsonData(VE_ObjectRecoTask *Info);

	//���ô洢json�ļ����ļ���·��
	int SetJsonFilePath(std::string strJsonFilePath, int nChannel = 0);

public:
	/*��������*/
	int SaveJsonFile();
	
	/*�������ݲ��ҿ���������ʾ*/
	int ParseJsonFile(std::string strJsonPath);

	/*��ȡ��ǰĿ¼�µ�����json�ļ�
	 *�����Ա����vetor���棬Ȼ���������������һ���ͽ�json�ļ��ƶ�����һ���ļ�����
	 *�����������֮�󣬾ͽ�vector������±����ļ���
	 *�������һ��Сʱ
	 * */
	void ParseJson();
    int GetObjRecoTaskSize(){ return m_arrObjRecoTask.size();}
	//����ͽ���bin�ļ�
//	void SaveJsonBin();

//	void ParseJsonBin(std::string strJsonPath);

public:
	//�˺������ڽ��������͸�Qtǰ�˳�����ʾ
	int GetVideoData(VE_ObjectRecoTask *Info);


	//��ȡ�Ѿ���ȡ������ļ����µ�json�ļ�����ؿ�
	//size_t GetVideoDataAfterRead(std::vector<VE_Object> & arrAfterObj);
	size_t GetVideoDataAfterRead(std::vector<VE_ObjectRecoTask*> & arrAfterObj);

private:
	//std::vector<VE_Object> m_arrObjectInfo;		//�洢��Ƶ��Ϣ����

	std::vector<VE_ObjectRecoTask*> m_arrObjRecoTask;	//��Ƶ����
	std::vector<std::string> m_arrJsonPath;		//�洢Ϊ��ʾ������	m_arrJsonPath[0];
	std::vector<std::string> m_arrJsonPathAfter;//�洢��ʾ��������	m_arrJsonPathAfter[0];

	int m_nModel;								//ģʽ���á�д��0����1
	int m_nChannel;								//���ü�¼���

	/*
	 *example:
	 * home/xxx/jsonparser/
	 * home/xxx/jsonparser/2018031314/ ----��Ҫ�������ļ��У��ں�json�ļ�
	 * home/xxx/jsonparser/2018011314/bak/ -----�洢������json�ļ�
	 * */
	std::string strExePath;						//�ϳ�jsonpath��                            
	std::string m_strJsonFilePath;				//��ǰsearch path,����ʱ���ȡ���ļ�·��
	std::string m_strJsonFilePathAfterRead;       //open after path

//	CBase64	    *m_pBase64;						//base64�����
};


VideoInfoJsonManager* createJsonManager(int model = 0);


#endif //_VIDEO_INFO_JSON_MANAGER_H__
