#include "VideoInfoJsonManager.h"

#include <fstream>
#include <json/writer.h>
#include <json/json.h>
#include <json/value.h>
#include "filenameio.h"

#include <pthread.h>


//引入命名空间
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

//json数据文件管理类，写入文件和读取文件
VideoInfoJsonManager* createJsonManager(int model)
{
	return new VideoInfoJsonManager(model);
}

void *IdleReadThread(void *params)
{
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		pManager->ParseJson();
	}
}

void *IdleWriteThread(void *params)
{
	cout << "thread start" << endl;
	VideoInfoJsonManager* pManager = (VideoInfoJsonManager*)params;
	if(pManager != NULL)
	{
		cout << "save json" << endl;
		pManager->SaveJsonFile();
	}
	else
	{
		cout << "manager is null" << endl;
	}
}

//设置存储json文件的文件夹路径
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
		

		//创建文件夹//创建缓存文件夹和读后文件夹
		if(m_nModel == 0)
			createDirectory(strJsonFilePath.c_str());
		//m_arrJsonPath.push_back(strJsonFilePath);
		//strJsonFilePath += "/bak";
		//if(m_nModel == 0)
		//	createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		//m_arrJsonPathAfter.push_back(strJsonFilePath);
//	}
}

//获取已经读取过后的文件夹下的json文件翻遍回看
size_t VideoInfoJsonManager::GetVideoDataAfterRead(std::vector<VE_ObjectRecoTask*>& arrAfterObj)
{
	return arrAfterObj.size();
}

VideoInfoJsonManager::VideoInfoJsonManager(int model):m_nModel(model),m_nChannel(1),m_strJsonFilePath("")
{
#if 0
	//初始化所有参数，包括获取当前时间精确到小时
	strJsonFilePath = IntToStr(GetCurrentTime());
	cout << "----------------------" << strJsonFilePath.c_str() << endl;
	char chHour[100];
	char chExePath[100];
	char chExeName[100];
	GetCurrentExePath(chExePath, chExeName, 200);
	strExePath = chExePath;								//记录文件路径下次合成目录是不再获取
	strJsonFilePath = chExePath;						
	cout << "======================" << strJsonFilePath.c_str() << endl;
	
	GetCurrentHour(chHour);
	strJsonFilePath += chHour;							//当前存储json文件的路径，所有传递过来的json文件存储到此文件夹下，并且每过一个小时就重新获取时间创建一个新的文件夹
	cout << "======================" << strJsonFilePath.c_str() << endl;	
#endif 

	//供需超说暂时直接根据每一路相机创建一个文件夹
	cout << "constructor" << endl;
	if(model == 0)
	{
		//根据小时创建文件夹，并获取文件夹路径，赋予成员变量
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
	else
	{
		//根据时间合成文件夹
		pthread_t pid;
		if(pthread_create(&pid, NULL, IdleReadThread, this) != 0)
		{
			cout << "thread create failed" << endl;
		}
		else
		{
			cout << "thread create success" << endl;
			pthread_detach(pid);
		}
	}
}

//外部调用推送json队列
int VideoInfoJsonManager::InsertJsonData(VE_ObjectRecoTask *pInfo)
{
	cout << "insert" << endl;
	m_arrObjRecoTask.push_back(pInfo);
	cout << "insert ok" << endl;

	//测试jsoncpp使用
	//SaveJsonFile();
}

#if 0
//保存和解析bin文件
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
				//根据时间戳穿件文件
				sprintf(jsonName, "%lld.json", obj.m_uiTimeStamp);
				ofstream ofs(jsonName, ios::out|ios::binary);
                if(ofs)
				{
					cerr << "open json failed" << endl;
					return;
				}
				//开始写入json数据
				Json::Value root = Json::Value();


			//这里需要根据传过来的信息判断是那一路相机需要放到那个文件夹下
		}
}

void VideoInfoJsonManeger::ParseJsonBin(std::string strJsonPath)
{
	
}

#endif

#if 1
//保存json文件到文件
int VideoInfoJsonManager::SaveJsonFile()
{	
	cout << "save file" << endl;
	while(1)
	{
		usleep(10);
		if(m_arrObjRecoTask.size() > 0)
		{
		    std::cout << "start write json data to file" << std::endl;
			if(m_strJsonFilePath == "")
			{
				cout << "json path error!" << endl;
				continue;
			}
			ofstream ofs;
			//根据时间戳穿件文件
			char jsonPath[1024];
			char jsonName[1024];

			//开始写入json数据
			Json::Value arrInfo;
			Json::Value root;

			VE_ObjectRecoTask* objReco = m_arrObjRecoTask.front();

			sprintf(jsonPath, "/%s_%d", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex);//写入IP;
			m_strJsonFilePath += jsonPath;	//文件夹目录合成完毕
			createDirectory(m_strJsonFilePath.c_str());	//创建文件夹目录
			sprintf(jsonName, "/%s_%d_%lld.json", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex,  objReco->m_vObjInfo.m_uiTimeStamp);	

			m_strJsonFilePath += jsonName;
			//这里需要根据传过来的信息判断是那一路相机需要放到那个文件夹下
			ofs.open(m_strJsonFilePath.c_str());//根据当前时间给json文件命名
			
			//申请存储图片的空间
			char *out;
			size_t outlen;
			
			if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bus || 
			   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car ||
			   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car)
			{
				cout << "carInfo" << endl;
				Json::Value carFeature;
				Json::Value carInfo;
				Json::Value car;
				carInfo["szPlate"] = objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate;						//车牌号码
				carInfo["szColor"] = objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor;						//车牌颜色
				carInfo["cVehicleColor"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor;					//车辆颜色
				carInfo["nVehicleColorScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore;			//车辆颜色得分
				carInfo["nVehicleTypeScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore;			//车辆品牌得分
				carInfo["cVehicleType"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType;					    //车型
				carInfo["cVehicleBrand"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand;					//车辆品牌
				carInfo["nVehicleBrandScore"] = objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore;			//车辆品牌得分
				carInfo["cVehicleSubBrand"] = objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand;			    //车辆子品牌
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色
		
				carFeature["fVehicleCarFeature"] = objReco->m_vObjInfo.m_vVehicleInfo.fVehicleCarFeature;
	
				//将IplImage读取为base64格式	
				//char out[objReco->m_pObjImageEx->nSize];
				cout << "base 64" << endl;
				//base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
				//carInfo["m_pObjImageEx"] = out;
				carInfp["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
				cout << "base 64 endl" << endl;
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
				NatureInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
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
				personInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
				root["PersonInifo"]		= personInfo;
			}

			std::string strJson = root.toStyledString();
			//ofs << strJson.c_str();
			ofs.write(strJson.c_str(), strlen(strJson.c_str()));
			cout << strJson.c_str() << endl;
			ofs.close();
			//清空队列里的某个信息
			free(out);
			cvReleaseImage(&(objReco->m_pObjImage));
			cvReleaseImage(&(objReco->m_pObjImageEx));
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

		//遍历制定文件夹下的所有文件
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
					//文件拷贝到另一个文件夹下
					CutFile(strJsonPath, strJsonPathAfter);
				}
			}
		}
	}
}


#if 1
//解析出需要的数据并将数据显示到指定串口
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
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, carInfo["szPlate"].asString().c_str());						//车牌号码
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, carInfo["szColor"].asString().c_str());						//车牌颜色
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor, carInfo["cVehicleColorScore"].asString().c_str());					//车辆颜色
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore = (veu8)carInfo["nVehicleColorScore"].asInt();			//车辆颜色得分
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore = (veu8)carInfo["nVehicleTypeScore"].asInt();			//车辆品牌得分
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType, carInfo["cVehicleType"].asString().c_str());					    //车型
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand, carInfo["cVehicleBrand"].asString().c_str());					//车辆品牌
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore = (veu8)carInfo["nVehicleBrandScore"].asInt();			//车辆品牌得分
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand, carInfo["cVehicleSubBrand"].asString().c_str());			    //车辆子品牌
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

//获取物体信息
int VideoInfoJsonManager::GetVideoData(VE_ObjectRecoTask* vInfo)
{
	if (m_arrObjRecoTask.size() > 0)
	{
		vInfo = m_arrObjRecoTask[0];
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
	}
}

