#include "VideoInfoJsonManager.h"

#include <fstream>
#include <json/writer.h>
#include <json/json.h>
#include <json/value.h>
#include "filenameio.h"

#include <pthread.h>


//���������ռ�
using namespace std;

static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(char *bindata, size_t inlen, char **out, size_t *outlen)
{
	size_t _outlen = *outlen;
	char *_out = NULL;
	size_t out_pos = 0;
	if(NULL == *out)
	{
		_outlen = (inlen / 3 + (inlen%3 != 0)) * 4 + 1;
		_out = (char *)malloc(_outlen);
	}
	else
	{
		_outlen = *outlen;
	    _out = *out;
	}
	memset(_out,'=',_outlen);
	_out[_outlen-1] = 0;
	int bits_collected = 0;
	int accumulator = 0;
	for(int i = 0; i < inlen; i++)
	{
		accumulator = (accumulator << 8) | (bindata[i] & 0xffu);
		bits_collected += 8;
		while (bits_collected >= 6)
		{
			bits_collected -= 6;
			_out[out_pos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
		}
	}
	if(bits_collected >= 6)
	{
		if(NULL == *out)
		{
			free(_out);
		}
		return NULL;
	}								

	if(bits_collected >0)
	{
		// Any trailing bits that are missing.
		accumulator <<= 6 - bits_collected;
		_out[out_pos++] = b64_table[accumulator & 0x3fu];
	}								
	*outlen = _outlen;
	*out = _out;
	return _out;
}

//json�����ļ������࣬д���ļ��Ͷ�ȡ�ļ�
VideoInfoJsonManager* createJsonManager(int model)
{
	return new VideoInfoJsonManager(model);
}

void *IdleWriteThread(void *params)
{
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		pManager->ParseJson();
	}
}

void *IdleReadThread(void *params)
{
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
		pManager->SaveJsonFile();
}

//���ô洢json�ļ����ļ���·��
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
			createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		//m_arrJsonPath.push_back(strJsonFilePath);
		//strJsonFilePath += "/bak";
		//if(m_nModel == 0)
		//	createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		//m_arrJsonPathAfter.push_back(strJsonFilePath);
//	}
}

//��ȡ�Ѿ���ȡ������ļ����µ�json�ļ�����ؿ�
size_t VideoInfoJsonManager::GetVideoDataAfterRead(std::vector<VE_ObjectRecoTask*>& arrAfterObj)
{
	return arrAfterObj.size();
}

VideoInfoJsonManager::VideoInfoJsonManager(int model):m_nModel(model),m_nChannel(1)
{
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

	//���賬˵��ʱֱ�Ӹ���ÿһ·�������һ���ļ���

	if(model == 0)
	{
		//����Сʱ�����ļ��У�����ȡ�ļ���·���������Ա����
		//createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleWriteThread, this) != 0)
		{
			cout << "thread create success" << endl;
		}
		else
		{
			pthread_detach(pid);
		}
	}
	else
	{
		//����ʱ��ϳ��ļ���
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleReadThread, this) != 0)
		{
			cout << "thread create success" << endl;
		}
		else
		{
			pthread_detach(pid);
		}
	}
}

//�ⲿ��������json����
int VideoInfoJsonManager::InsertJsonData(VE_ObjectRecoTask *pInfo)
{
	m_arrObjRecoTask.push_back(pInfo);

	//����jsoncppʹ��
	//SaveJsonFile();
}

#if 0
//����ͽ���bin�ļ�
void VideoInfoJsonManager::SaveJsonBin()
{
		while(1)
		{
			usleep(10);
			if(m_arrObjectInfo.size() > 0)
			{
				std::cout << "start write json data to file" << std::endl;
				char jsonName[100];
				VE_Object obj = m_arrObjectInfo.front();
				//����ʱ��������ļ�
				sprintf(jsonName, "%lld.json", obj.m_uiTimeStamp);
				ofstream ofs(jsonName, ios::out|ios::binary);
                if(ofs)
				{
					cerr << "open json failed" << endl;
					return;
				}
				//��ʼд��json����
				Json::Value root = Json::Value();


			//������Ҫ���ݴ���������Ϣ�ж�����һ·�����Ҫ�ŵ��Ǹ��ļ�����
		}
}

void VideoInfoJsonManeger::ParseJsonBin(std::string strJsonPath)
{
	
}

#endif

#if 1
//����json�ļ����ļ�
int VideoInfoJsonManager::SaveJsonFile()
{
	while(1)
	{
		usleep(10);
		if(m_arrObjRecoTask.size() > 0)
		{
		    std::cout << "start write json data to file" << std::endl;
			ofstream ofs;
			//����ʱ��������ļ�
			char jsonPath[1024];
			char jsonName[1024];

			//��ʼд��json����
			Json::Value arrInfo;
			Json::Value root;

			VE_ObjectRecoTask* objReco = m_arrObjRecoTask.front();

			sprintf(jsonPath, "/%s_%d", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex);//д��IP;
			m_strJsonFilePath += jsonPath;	//�ļ���Ŀ¼�ϳ����
			createDirectory(m_strJsonFilePath.c_str());	//�����ļ���Ŀ¼
			sprintf(jsonName, "/%s_%d_%lld.json", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex,  objReco->m_vObjInfo.m_uiTimeStamp);	

			m_strJsonFilePath += jsonName;
			//������Ҫ���ݴ���������Ϣ�ж�����һ·�����Ҫ�ŵ��Ǹ��ļ�����
			ofs.open(m_strJsonFilePath.c_str());//���ݵ�ǰʱ���json�ļ�����
			if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bus || 
			   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car ||
			   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car)
			{
				cout << "carInfo" << endl;
				Json::Value carFeature;
				Json::Value carInfo;
				Json::Value car;
				carInfo["szPlate"] = objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate;						//���ƺ���
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

				//��IplImage��ȡΪbase64��ʽ
				
				char *out = (char*)malloc(objReco->m_pObjImageEx->nSize);
				size_t outlen;
				carInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
				//carInfo["m_pObjImageEx"] = 

				car["carInfo"] = carInfo;
				car["carFeature"] = carFeature;
				root["totcalCarInfo"] = car;				
				//arrInfo.append(car);
				//root["totalCarInfo"] = arrInfo;
				//root["carInfo"] = carInfo;
				//root["carFeature"] = carFeature;
			}
			else if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bicycle ||
					objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_motorbike ||
						objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_tricar)
			{
				cout << "otherInfo" << endl;	
				Json::Value NatureInfo;
				NatureInfo["name"] = objReco->m_vObjInfo.m_vNatureInfo.name;
				NatureInfo["type"] = objReco->m_vObjInfo.m_vNatureInfo.type;
				NatureInfo["pose"] = objReco->m_vObjInfo.m_vNatureInfo.pose;
				NatureInfo["truncated"] = objReco->m_vObjInfo.m_vNatureInfo.truncated;
				NatureInfo["score"] = objReco->m_vObjInfo.m_vNatureInfo.score;
				root["otherVehicleInfo"] = NatureInfo;
			}
			else if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_person)
			{
				cout << "peopleInfo" << endl;	
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
				root["PersonInifo"] = personInfo;
			}

			std::string strJson = root.toStyledString();
			//ofs << strJson.c_str();
			ofs.write(strJson.c_str(), strlen(strJson.c_str()));
			cout << strJson.c_str() << endl;
			ofs.close();
			m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
		}
	}
}
#endif

void VideoInfoJsonManager::ParseJson()
{
	while(1)
	{
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
				scanDir(m_arrJsonPath[i].c_str(), arrJsonFile, 0);
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
	}
}


#if 1
//��������Ҫ�����ݲ���������ʾ��ָ������
int VideoInfoJsonManager::ParseJsonFile(std::string strJsonPath)
{
	if(strJsonPath == "")
		return -1;
	ifstream ifs;
	ifs.open(strJsonPath.c_str());
	Json::Value jsonRoot;
	Json::Value value;
#if 1
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
	//Json::Reader reader;
	//Json::Value  value;
	//Json::CharReader reader;
	//if(reader.parse(strJson, value))
//	if(reader.parse(ifs, value))
	{
		VE_ObjectRecoTask* objReco = new VE_ObjectRecoTask;
		if(value.isMember("totalCarInfo"))
		{
			cout << "total car info is member" << endl;
			const Json::Value arrObj = value["totalCarInfo"];
			for(unsigned int i = 0; i < arrObj.size(); ++i)
			{
					if(!arrObj[i].isMember("carInfo"))
					continue;

				const Json::Value carInfo = arrObj[i]["carInfo"];
				const Json::Value carFeature = arrObj[i]["carFeature"];
				//std::string szPlate = carInfo["szPlate"].asString();
				//cout << "szPlate:" << szPlate.c_str() << endl;
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, carInfo["szPlate"].asString().c_str());						//���ƺ���
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, carInfo["szColor"].asString().c_str());						//������ɫ
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor, carInfo["cVehicleColorScore"].asString().c_str());					//������ɫ
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore = (veu8)carInfo["nVehicleColorScore"].asInt();			//������ɫ�÷�
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore = (veu8)carInfo["nVehicleTypeScore"].asInt();			//����Ʒ�Ƶ÷�
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType, carInfo["cVehicleType"].asString().c_str());					    //����
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand, carInfo["cVehicleBrand"].asString().c_str());					//����Ʒ��
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore = (veu8)carInfo["nVehicleBrandScore"].asInt();			//����Ʒ�Ƶ÷�
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand, carInfo["cVehicleSubBrand"].asString().c_str());			    //������Ʒ��
				//obj.m_vVehicleInfo.fVehicleCarFeature = (vef32)carFeature["fVehicleCarFeature"].asFloat();			//carFeature
			}
		}
		else if(value.isMember("otherVehicleInfo"))
		{
				const Json::Value NatureInfo = value["otherVehicleInfo"];
				strcpy(objReco->m_vObjInfo.m_vNatureInfo.name, NatureInfo["name"].asString().c_str());
				//strcpy(obj.m_vNatureInfo.type, NatureInfo["type"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.type = (veu8)NatureInfo["type"].asInt();
				//strcpy(obj.m_vNatureInfo.pose, NatureInfo["pose"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.pose = (veu8)NatureInfo["pose"].asInt();
				//strcpy(obj.m_vNatureInfo.truncated, NatureInfo["truncated"].asString().c_str());
				objReco->m_vObjInfo.m_vNatureInfo.truncated = (veu8)NatureInfo["truncated"].asInt();
				objReco->m_vObjInfo.m_vNatureInfo.score = (veu8)NatureInfo["score"].asInt();
		}
		else if(value.isMember("PersonInfo"))
		{	
				cout << "read peopleInfo" << endl;
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
		}
		m_arrObjRecoTask.push_back(objReco);
	}
	

#if 0
	for(auto i = 0; i < jsonRoot.size(); ++i)
	{
		cout << jsonRoot.size() << endl;
		for(auto sub = jsonRoot[i].begin(); sub != jsonRoot[i].end(); sub++)
		{
			cout << sub.name() << endl;//<< ":" << jsonRoot[i][sub.name()] << endl;
			//cout << sub.name() << ":" << (*sub) << endl;
		}
	}
#endif
}
#endif

//��ȡ������Ϣ
int VideoInfoJsonManager::GetVideoData(VE_ObjectRecoTask* vInfo)
{
	if (m_arrObjRecoTask.size() > 0)
	{
		vInfo = m_arrObjRecoTask[0];
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
	}
}

