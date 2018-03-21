#ifndef _VIDEO_INFO_JSON_MANAGER_H__
#define _VIDEO_INFO_JSON_MANAGER_H__


/*
	VideoJsonInfoManager �����

	�������ļ��У���������������channel�½�ÿһ·���ļ���:ip_channel
	
	������Ƶ������channel����ʱ�������json�ļ�·����ip_channel_timestamp.json			

	һ��д�����һ����ȡ����,һ·��Ƶһ���ļ��У�ÿ���ļ��������кܶఴʱ�䴴�����ļ���(һ��Сʱһ���ļ���)

	ʱ���ļ������ÿ���ļ�����һ������ÿ�����󶼿�����ǰ�˳����ȡ����ʾ

	ֻҪ���������ͻᴴ���µ��ļ���

	�ж��ļ����Ƿ���ڣ�������ڲ��ٴ���ֱ�Ӷ�ȡ�����json����

	�������һ�������ȡ����ոմ洢���ļ��أ���ȡ�ļ�������ʱ��
*/

#include <iostream>
#include <queue>
#include <vector>
#include "VeObjectDefine.h"
#include "JsonManager.h"



class VideoInfoJsonManager : public JsonManager
{
public:
	/*
	 	���캯����
		���������int model: 0 ��ʾ�����ļ���1��ʾ�����ļ�
	 */
	VideoInfoJsonManager(int model = 0);

	/*
	 	��������
	 */
	virtual ~VideoInfoJsonManager();

public:	
	/*
		��ȡjson�ַ�����IplImage����
	*/
	void SetResourceCallback(RESOURCE_CALLBACK callback){ m_callback = callback; }
	
	/*
		��ȡ����
	*/
	int InsertJsonData(VE_ObjectRecoTask *Info);

	/*
		���ô洢json�ļ����ļ���·��
	*/
	int SetJsonFilePath(std::string strJsonFilePath, int nChannel = 0);

	/*
		��������
	*/
	int SaveJsonFile();
	
	/*
		�������ݲ��ҿ���������ʾ
	*/
	int ParseJsonFile(std::string strJsonPath);


	/*
		����obj,test
	*/
	int Release();

	/*
		��ȡ��ǰĿ¼�µ�����json�ļ�
	 	�����Ա����vetor���棬Ȼ���������������һ���ͽ�json�ļ��ƶ�����һ���ļ�����
	 	�����������֮�󣬾ͽ�vector������±����ļ���
	 	�������һ��Сʱ
	 */
	void ParseJson();
    	
	/*
		�������ƣ�GetObjRecoTaskSize()
		����ֵ�����ص�ǰ�洢����Ĵ�С
	*/
	int GetObjRecoTaskSize(){ return m_arrObjRecoTask.size();}
	
public:
	//�˺������ڽ��������͸�Qtǰ�˳�����ʾ
	int GetVideoData(VE_ObjectRecoTask *Info);


	//��ȡ����json�ļ���·��
	int GetJsonFilePath(std::vector<std::string> &arrJsonPath);


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
	std::string m_strJsonFilePathAfterRead;      		//open after path
	RESOURCE_CALLBACK m_callback;				//��ȡ���ݵĻص�����
	
//	CBase64	    *m_pBase64;						//base64�����
};


VideoInfoJsonManager* createJsonDataManager(int model = 0);


#endif //_VIDEO_INFO_JSON_MANAGER_H__
