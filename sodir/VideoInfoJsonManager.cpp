#include "VideoInfoJsonManager.h"
#include <iostream>
#include <fstream>
#include "json/writer.h"
#include "json/json.h"
#include "json/value.h"
#include "filenameio.h"
#include <pthread.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <iconv.h>
#include "base64.h"

#include "timeused.h"

//���������ռ�
using namespace std;
using namespace cv;

pthread_mutex_t g_mutex;


/*
	GetJsonFilePath,��������json�ļ�·��
*/
int VideoInfoJsonManager::GetJsonFilePath(std::vector<std::string> &arrJsonPath)
{
//	m_strJsonFilePath
}



/*
	�������ƣ�createJsonManager(int model);
	�������ã���������,json�����ļ������࣬д���ļ��Ͷ�ȡ�ļ�
	���������int model, 0��ʾͼƬתbase64,1��ʾ����base64,3��ʾ���Թ���
*/
VideoInfoJsonManager* createJsonDataManager(int model)
{
	return new VideoInfoJsonManager(model);
}


/*
	�������ƣ�IdleReadThread();
	�������ã�����json�߳�
	���������������ָ��
*/
void *IdleReadThread(void *params)
{
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		while(1)
		{
			usleep(10);
			pthread_mutex_lock(&g_mutex);
			pManager->ParseJson();
			pthread_mutex_unlock(&g_mutex);
		}
	}
}


int g_index = 0;
int g_index_ = 0;

/*
	�������ã����Խ��������ȶ��ԣ��ͷ�vector obj
*/
void *IdleTestParserThread(void* params)
{
	
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		while(1)
		{
			usleep(200);
			pthread_mutex_lock(&g_mutex);
			pManager->Release();
			pthread_mutex_unlock(&g_mutex);
		}
	}
}


/*
	�������ƣ�IdelWriteThread()
	�������ã�ת��json�߳�
	������������������
*/
void *IdleWriteThread(void *params)
{
	cout << "thread start" << endl;
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		while(1)
		{
			
			//if(m_arrObjRecoTask.size() > 0)
			if( pManager->GetObjRecoTaskSize() > 0)
			{
				cout << "vector size::" << pManager->GetObjRecoTaskSize() << endl;
				g_index++;
				usleep(10);
				pthread_mutex_lock(&g_mutex);
				cout << "SaveJsonFile" << endl;
				pManager->SaveJsonFile();
				pthread_mutex_unlock(&g_mutex);
			}
			else
			{
				//g_index_++;
				usleep(10);
				//if(g_index_ > 200) break;
			}
		}
		
	}
	else
	{
		cout << "manager is null" << endl;
	}
}



/*
	�������ƣ�SetJsonFilePath
	�������ã����ô洢json�ļ����ļ���·��
	���������string, strJonsFilePath-�ⲿ����Ĺ���·��,int channel-��ǰͨ��
*/
int VideoInfoJsonManager::SetJsonFilePath(std::string strJsonFilePath, int nChannel)
{
	m_strJsonFilePath = strJsonFilePath;
//	std::string strTmp = strJsonFilePath;
//	m_nChannel = nChannel;
	//for(int i = 0; i < nChannel; ++i)
//	{
//		strJsonFilePath = strTmp; 
//		strJsonFilePath += IntToStr(i); 
#if 0
		if(createDirectory(const_cast<char*>(strJsonFilePath.c_str())))
			return 0;
		else
			return -1;
#endif
		

		//�����ļ���//���������ļ��кͶ����ļ���
		if(m_nModel == 0)
			createDirectory(strJsonFilePath.c_str());
		//m_arrJsonPath.push_back(strJsonFilePath);
		//strJsonFilePath += "/bak";
		//if(m_nModel == 0)
		//	createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		//m_arrJsonPathAfter.push_back(strJsonFilePath);
//	}
}



/*
	��������:GetVideoDataAfterRead:
	�������ã���ȡ�Ѿ���ȡ������ļ����µ�json�ļ�����ؿ�
	���������obj����
*/
size_t VideoInfoJsonManager::GetVideoDataAfterRead(std::vector<VE_ObjectRecoTask*>& arrAfterObj)
{
	return arrAfterObj.size();
}

/*
	�������ƣ�~VideoInfoJsonManager()
	�������ã���������
*/
VideoInfoJsonManager::~VideoInfoJsonManager()
{
//	delete m_pBase64;
	pthread_mutex_destroy(&g_mutex);
}


/*
	�������ƣ�VideoInfoJsonManager()
	�������ã����캯��
*/
VideoInfoJsonManager::VideoInfoJsonManager(int model):m_nModel(model),m_nChannel(1),m_strJsonFilePath(""),m_callback(nullptr),m_pUserData(nullptr)
{
	pthread_mutex_init(&g_mutex, NULL);
#if 0
	//��ʼ�����в�����������ȡ��ǰʱ�侫ȷ��Сʱ
	strJsonFilePath = IntToStr(GetCurrentTime());
	cout << "----------------------" << strJsonFilePath.c_str() << endl;
	char chHour[100];
	char chExePath[100];
	char chExeName[100];
	GetCurrentExePath(chExePath, chExeName, 200);
	strExePath = chExePath;								//��¼�ļ�·���´κϳ�Ŀ¼�ǲ��ٻ�ȡ
	strJsonFilePath = chExePath;						
	cout << "======================" << strJsonFilePath.c_str() << endl;
	
	GetCurrentHour(chHour);
	strJsonFilePath += chHour;							//��ǰ�洢json�ļ���·�������д��ݹ�����json�ļ��洢�����ļ����£�����ÿ��һ��Сʱ�����»�ȡʱ�䴴��һ���µ��ļ���
	cout << "======================" << strJsonFilePath.c_str() << endl;	
#endif 
	//����base64ʵ��
//	m_pBase64 = new CBase64;

	//���賬˵��ʱֱ�Ӹ���ÿһ·�������һ���ļ���
	cout << "constructor" << endl;
	if(model == 0) //д��json�ļ�
	{
		//����Сʱ�����ļ��У�����ȡ�ļ���·���������Ա����
		//createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleWriteThread, this) != 0)
		{
			cout << "thread write create failed" << endl;
		}
		else
		{
			pthread_detach(pid);
			cout << "thread write create success" << endl;
		}
	}
	else if(model == 1)//����json�ļ�
	{
		//����ʱ��ϳ��ļ���
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleReadThread, this) != 0)
		{
			cout << "read thread create failed" << endl;
		}
		else
		{
			cout << "read thread create success" << endl;
			pthread_detach(pid);
		}
	}
	else if(model == 3)
	{
		//����ʱ��ϳ��ļ���
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleTestParserThread, this) != 0)
		{
			cout << "testParser thread create failed" << endl;
		}
		else
		{
			cout << "testParser thread create success" << endl;
			pthread_detach(pid);
		}
		
	}
}


/*	
	�������ƣ�InsertJsonData
	�������ã��ⲿ��������json����
	�������:obj����
*/
int VideoInfoJsonManager::InsertJsonData(VE_ObjectRecoTask *pInfo)
{
	pthread_mutex_lock(&g_mutex);
	m_arrObjRecoTask.push_back(pInfo);
	cout << "InsertJsonData Ok" << endl;
	pthread_mutex_unlock(&g_mutex);
	//����jsoncppʹ��
	//SaveJsonFile();
}


#if 1
/*
	�������ƣ�gb2312toutf8
	�������ã��ַ�ת��
*/
int gb2312toutf8(const char *sourcebuf, size_t sourcelen, char *destbuf, size_t destlen)
{
	iconv_t cd;
	cd = iconv_open("utf-8", "gb2312");
	//cd = iconv_open("utf-8", "us-ascii");
	
	memset(destbuf, 0, destlen);
	const char **source = &sourcebuf;
	char **dest = &destbuf;
	iconv(cd, (char**)(source), &sourcelen, dest, &destlen);
	iconv_close(cd);
	return 0;
}


/*
	�������ƣ�GetSaveJsonPath
	�������ã��ϳ�json�ļ�����
*/
string GetSaveJsonPath(string strRoot,VE_ObjectRecoTask* objReco)
{
	if(strRoot == "")
	{
		cout << "json path error!" << endl;
			return "";
	}
	
	//����ʱ��������ļ�
	char jsonPath[1024];
	char jsonName[1024]; 
	
	//createDirectory
	sprintf(jsonPath, "/%s_%d", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex);//д��IP;
	string l_strPath = strRoot;
	l_strPath += jsonPath;	//�ļ���Ŀ¼�ϳ����
	createDirectory(l_strPath.c_str());	//�����ļ���Ŀ¼
	
	//get Save Path
	sprintf(jsonName, "/%s_%d_%lld_%d", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex,  objReco->m_vObjInfo.m_uiTimeStamp, g_index);	
	l_strPath += jsonName;
	g_index++;
	printf("JSON Name is %s \n",l_strPath.c_str());
	
	return l_strPath;
}


/*
	�������ƣ�SaveJsonFile
	�������ã�����json�ļ����ļ�
	�������: ��
*/
int VideoInfoJsonManager::SaveJsonFile()
{
	timeused tim;	
	{
		cout << "In SaveJsonFile" << endl;	
		if(m_strJsonFilePath == "")
		{
			cout << "json path error!" << endl;
			return 0;
		}
		
		ofstream ofs;
		
		//��ʼд��json����
		Json::Value arrInfo;
		Json::Value root;
		VE_ObjectRecoTask* objReco = m_arrObjRecoTask.front();
		
		string l_strPath = GetSaveJsonPath(m_strJsonFilePath, objReco);
		
		//������Ҫ���ݴ���������Ϣ�ж�����һ·�����Ҫ�ŵ��Ǹ��ļ�����
		string l_strPathJson = l_strPath;
		string l_strPathJpg = l_strPath;
		l_strPathJson += ".json";
		l_strPathJpg += ".jpg";
		ofs.open(l_strPathJson.c_str());//���ݵ�ǰʱ���json�ļ�����
	
		//����洢ͼƬ�Ŀռ�
		string strBase64Out;
		size_t outlen;
		
		char lResDst[256];
		if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bus || 
		   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car ||
		   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_truck)
		{
			cout << "carInfo=============================================================================" << endl;
			Json::Value carFeature;
			Json::Value carInfo;
			Json::Value car;
			
			//gb2312toutf8(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, strlen(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate), lResDst, 256);
			printf("%s \n %s \n",objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate,lResDst);
			carInfo["szPlate"] = objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate;						//���ƺ���
			//gb2312toutf8(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, strlen(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor), lResDst, 256);
			carInfo["szColor"] = objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor;						//������ɫ
			carInfo["cVehicleColor"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor;					//������ɫ
			carInfo["nVehicleColorScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore;			//������ɫ�÷�
			carInfo["nVehicleTypeScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore;			//����Ʒ�Ƶ÷�
			carInfo["cVehicleType"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType;					    //����
			carInfo["cVehicleBrand"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand;					//����Ʒ��
			carInfo["nVehicleBrandScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore;			//����Ʒ�Ƶ÷�
			carInfo["cVehicleSubBrand"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand;			    //������Ʒ��
			//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//������ɫ
			//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//������ɫ
			//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//������ɫ	
			carFeature["fVehicleCarFeature"] = objReco->m_vObjInfo.m_vVehicleInfo.fVehicleCarFeature;

			if(objReco->m_pObjImageEx != nullptr)
				cvSaveImage(l_strPathJpg.c_str(), objReco->m_pObjImageEx);
			
#if 0	//��ʱ����json�ʼ۴洢ͼƬ
			carInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;			//ͼƬ���
			carInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;			//ͼƬ�߶�
			carInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//ͼƬͨ��
			//��IplImage��ȡΪbase64��ʽ
			if(objReco->m_pObjImageEx != nullptr)
			{	
				CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->imageSize, strBase64Out);
			}
			cout << objReco->m_pObjImageEx->width << "---" << objReco->m_pObjImageEx->height << "---" << objReco->m_pObjImageEx->nChannels << endl;;
			cout << "convert base64 end" << endl;
			carInfo["m_pObjImageEx"] = strBase64Out.c_str();
#endif
			car["carInfo"] = carInfo;
			car["carFeature"] = carFeature;
			root["totalCarInfo"] = car;
		}
		else if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bicycle ||
				objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_motorbike ||
					objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_tricar)
		{
			cout << "otherInfo**************************************************************************" << endl;
			Json::Value NatureInfo;
			NatureInfo["name"] = objReco->m_vObjInfo.m_vNatureInfo.name;
			NatureInfo["type"] = objReco->m_vObjInfo.m_vNatureInfo.type;
			NatureInfo["pose"] = objReco->m_vObjInfo.m_vNatureInfo.pose;
			NatureInfo["truncated"] = objReco->m_vObjInfo.m_vNatureInfo.truncated;
			NatureInfo["score"] = objReco->m_vObjInfo.m_vNatureInfo.score;
			if(objReco->m_pObjImageEx != nullptr)
				cvSaveImage(l_strPathJpg.c_str(), objReco->m_pObjImageEx);
#if 0	//��ʱ����json�ʼ۴洢ͼƬ
			NatureInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;			//ͼƬ���
			NatureInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;			//ͼƬ�߶�
			NatureInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//ͼƬͨ��
			//NatureInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
			cout << objReco->m_pObjImageEx->width << "---" << objReco->m_pObjImageEx->height << "---" << objReco->m_pObjImageEx->nChannels << endl;;
			if(objReco->m_pObjImageEx != nullptr)
			{
				CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->imageSize, strBase64Out);
			}
			cout << "convert base64 end" << endl;
			NatureInfo["m_pObjImageEx"] = strBase64Out.c_str();
#endif
			root["otherVehicleInfo"] = NatureInfo;
		}
		else if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_person)
		{
			//if(sizeof(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea)/sizeof(VE_TinyFeature) < 10)
			//	cout << "human info error" << endl;
			
			cout << "peopleInfoi------------------------------------------------------------------------" << endl;	
			Json::Value personInfo;
			personInfo["coatColor"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[0].cFeature;
			personInfo["coatColorScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[0].nFeatureScore;
			personInfo["trousersColor"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[1].cFeature;
			personInfo["trousersColorScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[1].nFeatureScore;
			personInfo["hairType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[2].cFeature;
			personInfo["hairScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[2].nFeatureScore;
			personInfo["coatType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[3].cFeature;
			personInfo["coatTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[3].nFeatureScore;
			personInfo["trousersType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[4].cFeature;
			personInfo["trousersTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[4].nFeatureScore;
			personInfo["bagType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[5].cFeature;
			personInfo["bagTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[5].nFeatureScore;
			personInfo["ageType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[6].cFeature;
			personInfo["ageTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[6].nFeatureScore;
			personInfo["gender"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[7].cFeature;
			personInfo["genderScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[7].nFeatureScore;
			personInfo["raceType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[8].cFeature;
			personInfo["raceTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[8].nFeatureScore;
			personInfo["hatType"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[9].cFeature;
			personInfo["hatTypeScore"] = objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[9].nFeatureScore;
		
			if(objReco->m_pObjImageEx != nullptr)
				cvSaveImage(l_strPathJpg.c_str(), objReco->m_pObjImageEx);

#if 0	//��ʱ����json�ʼ۴洢ͼƬ
			personInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;				//ͼƬ���
			personInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;				//ͼƬ�߶�
			personInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//ͼƬͨ��
			cout << objReco->m_pObjImageEx->width << "---" << objReco->m_pObjImageEx->height << "---" << objReco->m_pObjImageEx->nChannels << endl;;
			if(objReco->m_pObjImageEx != nullptr)
			{
				CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->imageSize, strBase64Out);
			}
			cout << "convert base64 end" << endl;
			personInfo["m_pObjImageEx"] = strBase64Out.c_str();
#endif		
			root["PersonInfo"]		= personInfo;
		}
		std::string strJson = root.toStyledString();
		//ʹ�ûص������������⴫
		if(objReco->m_pObjImageEx != nullptr && m_callback != nullptr)
			m_callback(strJson, cvCloneImage(objReco->m_pObjImageEx));
		ofs.write(strJson.c_str(), strlen(strJson.c_str()));
		ofs.close();
		//��ն������ĳ����Ϣ
		if(objReco->m_pObjImage != nullptr)
			cvReleaseImage(&(objReco->m_pObjImage));
		if(objReco->m_pObjImageEx != nullptr)
			cvReleaseImage(&(objReco->m_pObjImageEx));
		delete objReco;
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
	}
}
#endif


/*
	�ͷ�obj
*/
int VideoInfoJsonManager::Release()
{
	cout << "Release" << endl;
	int i = 0;
	while(m_arrObjRecoTask.size() > 0)
	{
		cvReleaseImage(&(m_arrObjRecoTask[i]->m_pObjImageEx));
		delete m_arrObjRecoTask[i];
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
		i++;	
	}
}


/*
	�������ƣ�
	�������ã�����json�ļ�
*/
void VideoInfoJsonManager::ParseJson()
{

#if 0
	std::string strJsonPath = "";
	std::string strJsonPathAfter = "";

	//�����ƶ��ļ����µ������ļ�
	std::vector<std::string> arrJsonFile;
	for(int i = 0; i < m_nChannel; ++i)
	{
		usleep(1);
		if(m_arrObjRecoTask.size() == 0)
		{
			arrJsonFile.clear();
			scanDir(m_arrJsonPath[i].c_str(), arrJsonFile, 0, false);
			for(int j = 0; j < arrJsonFile.size(); ++j)
			{
				strJsonPath = m_arrJsonPath[i];
				strJsonPath += arrJsonFile[i];
				strJsonPathAfter = m_arrJsonPathAfter[i];
				strJsonPathAfter = arrJsonFile[i];
				ParseJsonFile(strJsonPath);
				//�ļ���������һ���ļ�����
				CutFile(strJsonPath, strJsonPathAfter);
			}
		}
	}
#endif 
}


int nTestParse = 0;

#if 1

/*
	�������ƣ�ParsrJsonFile
	�������ã���������Ҫ�����ݲ���������ʾ��ָ������
	�������:json�ļ�·��
*/	
int VideoInfoJsonManager::ParseJsonFile(std::string strJsonPath)
{
	pthread_mutex_lock(&g_mutex);
	cout << "Parse Json" << endl;
	if(strJsonPath == "")
		return -1;
	ifstream ifs;
	ifs.open(strJsonPath.c_str());
	Json::Value jsonRoot;
	Json::Value value;
#if 0
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING errs;
	if(!parseFromStream(builder, ifs, &value, &errs))
	{
		return -1;
	}
	//string strJson = jsonRoot.toStyledString();
	//cout << strJson.c_str() << endl;
#endif
	Json::Reader reader;
	//Json::Value  value;
	//Json::CharReader reader;
	//if(reader.parse(strJson, value))
	if(reader.parse(ifs, value))
	{
		cout << "CarInfo" << endl;
		VE_ObjectRecoTask* objReco = new VE_ObjectRecoTask;
		string strIn;
		unsigned long uOutLen;
		if(value.isMember("totalCarInfo"))
		{
			cout << "total car info is member" << endl;
			const Json::Value arrObj = value["totalCarInfo"];
			//for(unsigned int i = 0; i < arrObj.size(); ++i)
			{
					//if(!arrObj[i].isMember("carInfo"))
			//		if(!arrObj.isMember("carInfo"))
			//			continue;

			//	const Json::Value carInfo = arrObj[i]["carInfo"];
			//	const Json::Value carFeature = arrObj[i]["carFeature"];
				//std::string szPlate = carInfo["szPlate"].asString();
				//cout << "szPlate:" << szPlate.c_str() << endl;
				cout << "totalCarInfo" << endl;
				const Json::Value carInfo = arrObj["carInfo"];
				cout << "carInfo" << endl;
				const Json::Value carFeature = arrObj["carFeature"];
				cout << "carFeature" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, carInfo["szPlate"].asString().c_str());						//���ƺ���
				cout << "szPlate" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, carInfo["szColor"].asString().c_str());						//������ɫ
				cout << "szColor" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor, carInfo["cVehicleColor"].asString().c_str());					//������ɫ
				cout << "cVehicleColor" << endl;
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore = (veu8)carInfo["nVehicleColorScore"].asInt();			//������ɫ�÷�
				cout << "nVehicleColorScore" << endl;
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore = (veu8)carInfo["nVehicleTypeScore"].asInt();			//����Ʒ�Ƶ÷�
				cout << "nVehicleTypeScore" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType, carInfo["cVehicleType"].asString().c_str());					    //����
				cout << "nVehicleType" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand, carInfo["cVehicleBrand"].asString().c_str());					//����Ʒ��
				cout << "cVehicleBrand" << endl;
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore = (veu8)carInfo["nVehicleBrandScore"].asInt();			//����Ʒ�Ƶ÷�
				cout << "nVehicleBrandScore" << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand, carInfo["cVehicleSubBrand"].asString().c_str());			    //������Ʒ��
				cout << "nVehicleSubBrand" << endl;
				memcpy(objReco->m_vObjInfo.m_vVehicleInfo.fVehicleCarFeature, carFeature["fVehicleCarFeature"].asString().c_str(), carFeature["fVehicleCarFeature"].asString().length());			//carFeature
				cout << "fVehicleCarFeature" << endl;
				
			
				//��ȡͼƬ���
				int nWidth = carInfo["ImageExWidth"].asInt();
				int nHeight = carInfo["ImageExHeight"].asInt();
				int nChannel = carInfo["ImageExChannel"].asInt();
				objReco->m_pObjImageEx = cvCreateImage(cvSize(carInfo["ImageExWidth"].asInt(), carInfo["ImageExHeight"].asInt()), 8, carInfo["ImageExChannel"].asInt()); 
				cout << "ImageEx"  << carInfo["ImageExWidth"].asInt() << "---" << carInfo["ImageExHeight"].asInt() << "--" << carInfo["ImageExChannel"].asInt() << endl;
				strIn = carInfo["m_pObjImageEx"].asString();
				unsigned char* pOut = (unsigned char*)malloc(strIn.length());
			//	cout << strIn.c_str() << endl;
				//CBase64::Decode(strIn, pOut, &uOutLen);
				memcpy(pOut, strIn.c_str(), strIn.length());
				cout << strIn.length() << endl;
				cout << "-----"<< endl;
				cout << nWidth* nHeight *nChannel  << endl;
				CBase64::base64Decode((char*)pOut, strIn.length());
				//����base64
				cout << "m_pObjImageEx" << endl;
				//cout << uOutLen << endl;
			 	//cout << pOut << endl;	
				//�ϳ�ͼƬ
				memcpy(objReco->m_pObjImageEx->imageData, pOut, nWidth*nHeight*nChannel);
				cout << "total endl" << endl;
				char strJpgPath[100];
				sprintf(strJpgPath, "%d.jpg", nTestParse);
				nTestParse++;
				cvSaveImage(strJpgPath, objReco->m_pObjImageEx);
				free(pOut);
			}
		}
		else if(value.isMember("otherVehicleInfo"))
		{
				cout << "OtherInfo" << endl;
				const Json::Value NatureInfo = value["otherVehicleInfo"];
				strcpy(objReco->m_vObjInfo.m_vNatureInfo.name, NatureInfo["name"].asString().c_str());
				//strcpy(obj.m_vNatureInfo.type, NatureInfo["type"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.type = (veu8)NatureInfo["type"].asInt();
				//strcpy(obj.m_vNatureInfo.pose, NatureInfo["pose"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.pose = (veu8)NatureInfo["pose"].asInt();
				//strcpy(obj.m_vNatureInfo.truncated, NatureInfo["truncated"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.truncated = (veu8)NatureInfo["truncated"].asInt();
				objReco->m_vObjInfo.m_vNatureInfo.score = (veu8)NatureInfo["score"].asInt();
				
				int nWidth = NatureInfo["ImageExWidth"].asInt();
				int nHeight = NatureInfo["ImageExHeight"].asInt();
				int nChannel = NatureInfo["ImageExChannel"].asInt();
				//��ȡͼƬ���
				IplImage *pImg = cvCreateImage(cvSize(NatureInfo["ImageExWidth"].asInt(), NatureInfo["ImageExHeight"].asInt()), 8, NatureInfo["ImageExChannel"].asInt()); 
				strIn = NatureInfo["m_pObjImageEx"].asString();
				unsigned char* pOut = (unsigned char*)malloc(strIn.length());
				CBase64::base64Decode((char*)pOut, strIn.length());
				//�ϳ�ͼƬ
				memcpy(pImg->imageData, pOut, nWidth*nChannel*nHeight);
				objReco->m_pObjImageEx = pImg;
				cvSaveImage("1.jpg" ,pImg);
				free(pOut);
		}
		else if(value.isMember("PersonInfo"))
		{	
				cout << "PeopleInfo" << endl;
				const Json::Value personInfo = value["PersonInfo"];
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[0].cFeature, personInfo["coatColor"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[0].nFeatureScore = (veu8)personInfo["coatColorScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[1].cFeature, personInfo["trousersColor"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[1].nFeatureScore = (veu8)personInfo["trousersColorScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[2].cFeature, personInfo["hairType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[2].nFeatureScore = (veu8)personInfo["hairScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[3].cFeature, personInfo["coatType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[3].nFeatureScore = (veu8)personInfo["coatTypeScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[4].cFeature, personInfo["trousersType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[4].nFeatureScore = (veu8)personInfo["trousersTypeScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[5].cFeature, personInfo["bagType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[5].nFeatureScore = (veu8)personInfo["bagTypeScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[6].cFeature, personInfo["ageType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[6].nFeatureScore = (veu8)personInfo["ageTypeScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[7].cFeature, personInfo["gender"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[7].nFeatureScore = (veu8)personInfo["genderScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[8].cFeature, personInfo["raceType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[8].nFeatureScore = (veu8)personInfo["raceTypeScore"].asInt();
				strcpy(objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[9].cFeature, personInfo["hatType"].asString().c_str());
				objReco->m_vObjInfo.m_vHumanInfo.tfHumanFea[9].nFeatureScore = (veu8)personInfo["hatTypeScore"].asInt();
				int nWidth = personInfo["ImageExWidth"].asInt();
				int nHeight = personInfo["ImageExHeight"].asInt();
				int nChannel = personInfo["ImageExChannel"].asInt();
				//��ȡͼƬ���
				IplImage *pImg = cvCreateImage(cvSize(personInfo["ImageExWidth"].asInt(), personInfo["ImageExHeight"].asInt()), 8, personInfo["ImageExChannel"].asInt()); 
				strIn = personInfo["m_pObjImageEx"].asString();
				unsigned char* pOut = (unsigned char*)malloc(strIn.length());
				CBase64::base64Decode((char*)pOut, strIn.length());
				//�ϳ�ͼƬ
				memcpy(pImg->imageData, pOut, nWidth*nChannel*nHeight);
				objReco->m_pObjImageEx = pImg;
				cvSaveImage("1.jpg" ,pImg);
				free(pOut);
		}
		m_arrObjRecoTask.push_back(objReco);
	}
	
	pthread_mutex_unlock(&g_mutex);
}

#endif




/*	
	�������ƣ�
	�������ã���ȡ������Ϣ
	�����������ȡobj
*/
int VideoInfoJsonManager::GetVideoData(VE_ObjectRecoTask* vInfo)
{
	pthread_mutex_lock(&g_mutex);
	if (m_arrObjRecoTask.size() > 0)
	{
		vInfo = m_arrObjRecoTask[0];
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
	}
	pthread_mutex_unlock(&g_mutex);
}

