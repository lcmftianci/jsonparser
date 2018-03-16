#include "VideoInfoJsonManager.h"

#include <fstream>
#include <json/writer.h>
#include <json/json.h>
#include <json/value.h>
#include "filenameio.h"

#include <pthread.h>


//引入命名空间
using namespace std;


//json数据文件管理类，写入文件和读取文件
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

//设置存储json文件的文件夹路径
int VideoInfoJsonManager::SetJsonFilePath(std::string strJsonFilePath, int nChannel)
{	
	std::string strTmp = strJsonFilePath;
	m_nChannel = nChannel;
	for(int i = 0; i < nChannel; ++i)
	{
		strJsonFilePath = strTmp; 
		strJsonFilePath += IntToStr(i); 
#if 0
		if(createDirectory(const_cast<char*>(strJsonFilePath.c_str())))
			return 0;
		else
			return -1;
#endif
		

		//创建文件夹//创建缓存文件夹和读后文件夹
		if(m_nModel == 0)
			createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		m_arrJsonPath.push_back(strJsonFilePath);
		strJsonFilePath += "/bak";
		if(m_nModel == 0)
			createDirectory(const_cast<char*>(strJsonFilePath.c_str()));
		m_arrJsonPathAfter.push_back(strJsonFilePath);
	}
}

//获取已经读取过后的文件夹下的json文件翻遍回看
size_t VideoInfoJsonManager::GetVideoDataAfterRead(std::vector<VE_Object> & arrAfterObj)
{
	return arrAfterObj.size();
}

VideoInfoJsonManager::VideoInfoJsonManager(int model):m_nModel(model),m_nChannel(1)
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

	if(model == 0)
	{
		//根据小时创建文件夹，并获取文件夹路径，赋予成员变量
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
		//根据时间合成文件夹
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

//外部调用推送json队列
int VideoInfoJsonManager::InsertJsonData(const VE_Object &Info)
{
	m_arrObjectInfo.push_back(Info);

	//测试jsoncpp使用
	//SaveJsonFile();
}

//保存json文件到文件
int VideoInfoJsonManager::SaveJsonFile()
{
#if 0
	Json::Value arrCar;				//car  carInfo carfeature 
	Json::Value carInfo;
	Json::Value carFeature;
	Json::Value totalCarInfo;
	Json::Value carCount;		   //
	Json::FastWriter jWriter;	   //
	Json::Value root;			   //根目录
	root["name"] = "tocy";
	root["salary"] = 100;
	root["msg"] = "work hard";
	
	Json::Value arrFiles;
	
	for(int i = 0; i < 4; i++)
	{
		Json::Value files;
		Json::Value carFeature;
		Json::Value carInfo;
		Json::Value car;
		files["a"] = "1.ts";
		files["b"] = "2.txt";
		//carInfo["carInfo"] = files;
		//carFeature["carFeature"] = files;
	
		car["carInfo"] = files;
		car["carFeature"] = files;

		//car.append(carInfo);
		//car.append(carFeature);
		//car[0]["carInfo"] = carInfo;
		//car[0]["carFeature"] = carFeature;
		//arrFiles.append(carInfo);
		arrFiles.append(car);
	}

	root["Files"] = arrFiles;

	std::string json_file =  root.toStyledString(); //jWriter.write(root).toStyledString();
	std::cout << "demo write json object ==============\n";
	std::cout << json_file << std::endl;
#endif

	while(1)
	{
	//	std::cout << "start write json data to file" << std::endl;
		usleep(10);
		if(m_arrObjectInfo.size() > 0)
		{
		    std::cout << "start write json data to file" << std::endl;
			ofstream ofs;
#if 0
			int time = GetCurrentTime();
#endif
			//根据时间戳穿件文件
			char jsonName[100];
			//std::vector<VE_Object>::iterator iterObj = m_arrObjectInfo.begin();
			//if(iterObj->VE_ObjectTypeEx)

			//开始写入json数据
			Json::Value arrInfo;
			Json::Value root;


			VE_Object obj = m_arrObjectInfo.front();
			sprintf(jsonName, "%lld.json", obj.m_uiTimeStamp);

			//这里需要根据传过来的信息判断是那一路相机需要放到那个文件夹下


			ofs.open(jsonName);//根据当前时间给json文件命名
			if(obj.m_uiObjTypeEx == ve_obj_typeex_bus || 
			   obj.m_uiObjTypeEx == ve_obj_typeex_car ||
			   obj.m_uiObjTypeEx == ve_obj_typeex_car)
			{
				cout << "carInfo" << endl;
				Json::Value carFeature;
				Json::Value carInfo;
				Json::Value car;
				carInfo["szPlate"] = obj.m_vVehicleInfo.plateinfo.szPlate;						//车牌号码
				carInfo["szColor"] = obj.m_vVehicleInfo.plateinfo.szColor;						//车牌颜色
				carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;					//车辆颜色
				carInfo["nVehicleColorScore"] = obj.m_vVehicleInfo.nVehicleColorScore;			//车辆颜色得分
				carInfo["nVehicleTypeScore"] = obj.m_vVehicleInfo.nVehicleTypeScore;			//车辆品牌得分
				carInfo["cVehicleType"] = obj.m_vVehicleInfo.cVehicleType;					    //车型
				carInfo["cVehicleBrand"] = obj.m_vVehicleInfo.cVehicleBrand;					//车辆品牌
				carInfo["nVehicleBrandScore"] = obj.m_vVehicleInfo.nVehicleBrandScore;			//车辆品牌得分
				carInfo["cVehicleSubBrand"] = obj.m_vVehicleInfo.cVehicleSubBrand;			    //车辆子品牌
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色
				//carInfo["cVehicleColor"] = obj.m_vVehicleInfo.cVehicleColor;			//车辆颜色

				
				carFeature["fVehicleCarFeature"] = obj.m_vVehicleInfo.fVehicleCarFeature;

				car["carInfo"] = carInfo;
				car["carFeature"] = carFeature;
				arrInfo.append(car);
				root["totalCarInfo"] = arrInfo;
			}
			else if(obj.m_uiObjTypeEx == ve_obj_typeex_bicycle ||
					obj.m_uiObjTypeEx == ve_obj_typeex_motorbike ||
					obj.m_uiObjTypeEx == ve_obj_typeex_tricar)
			{
				cout << "otherInfo" << endl;
				Json::Value NatureInfo;
				NatureInfo["name"] = obj.m_vNatureInfo.name;
				NatureInfo["type"] = obj.m_vNatureInfo.type;
				NatureInfo["pose"] = obj.m_vNatureInfo.pose;
				NatureInfo["truncated"] = obj.m_vNatureInfo.truncated;
				NatureInfo["score"] = obj.m_vNatureInfo.score;
				root["otherVehicleInfo"] = NatureInfo;
			}
			else if(obj.m_uiObjTypeEx == ve_obj_typeex_person)
			{
				cout << "peopleInfo" << endl;
				Json::Value personInfo;
				personInfo["coatColor"] = obj.m_vHumanInfo.tfHumanFea[0].cFeature;
				personInfo["coatColorScore"] = obj.m_vHumanInfo.tfHumanFea[0].nFeatureScore;
				personInfo["trousersColor"] = obj.m_vHumanInfo.tfHumanFea[1].cFeature;
				personInfo["trousersColorScore"] = obj.m_vHumanInfo.tfHumanFea[1].nFeatureScore;
				personInfo["hairType"] = obj.m_vHumanInfo.tfHumanFea[2].cFeature;
				personInfo["hairScore"] = obj.m_vHumanInfo.tfHumanFea[2].nFeatureScore;
				personInfo["coatType"] = obj.m_vHumanInfo.tfHumanFea[3].cFeature;
				personInfo["coatTypeScore"] = obj.m_vHumanInfo.tfHumanFea[3].nFeatureScore;
				personInfo["trousersType"] = obj.m_vHumanInfo.tfHumanFea[4].cFeature;
				personInfo["trousersTypeScore"] = obj.m_vHumanInfo.tfHumanFea[4].nFeatureScore;
				personInfo["bagType"] = obj.m_vHumanInfo.tfHumanFea[5].cFeature;
				personInfo["bagTypeScore"] = obj.m_vHumanInfo.tfHumanFea[5].nFeatureScore;
				personInfo["ageType"] = obj.m_vHumanInfo.tfHumanFea[6].cFeature;
				personInfo["ageTypeScore"] = obj.m_vHumanInfo.tfHumanFea[6].nFeatureScore;
				personInfo["gender"] = obj.m_vHumanInfo.tfHumanFea[7].cFeature;
				personInfo["genderScore"] = obj.m_vHumanInfo.tfHumanFea[7].nFeatureScore;
				personInfo["raceType"] = obj.m_vHumanInfo.tfHumanFea[8].cFeature;
				personInfo["raceTypeScore"] = obj.m_vHumanInfo.tfHumanFea[8].nFeatureScore;
				personInfo["hatType"] = obj.m_vHumanInfo.tfHumanFea[9].cFeature;
				personInfo["hatTypeScore"] = obj.m_vHumanInfo.tfHumanFea[9].nFeatureScore;
				root["PersonInifo"] = personInfo;
			}

			std::string strJson = root.toStyledString();
			//ofs << strJson.c_str();
			ofs.write(strJson.c_str(), strlen(strJson.c_str()));
			cout << strJson.c_str() << endl;
			ofs.close();
			m_arrObjectInfo.erase(m_arrObjectInfo.begin());
		}
	}
}

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
			if(m_arrObjectInfo.size() == 0)
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

//解析出需要的数据并将数据显示到指定串口
int VideoInfoJsonManager::ParseJsonFile(std::string strJsonPath)
{
	if(strJsonPath == "")
		return -1;
	ifstream ifs;
	ifs.open(strJsonPath.c_str());
	Json::Value jsonRoot;
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	JSONCPP_STRING errs;
	if(!parseFromStream(builder, ifs, &jsonRoot, &errs))
	{
		return -1;
	}

	string strJson = jsonRoot.toStyledString();
	cout << strJson.c_str() << endl;

	Json::Reader reader;
	Json::Value  value;
	if(reader.parse(strJson, value))
	{
		VE_Object obj;
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
				strcpy(obj.m_vVehicleInfo.plateinfo.szPlate, carInfo["szPlate"].asString().c_str());						//车牌号码
				strcpy(obj.m_vVehicleInfo.plateinfo.szColor, carInfo["szColor"].asString().c_str());						//车牌颜色
				strcpy(obj.m_vVehicleInfo.cVehicleColor, carInfo["cVehicleColorScore"].asString().c_str());					//车辆颜色
				obj.m_vVehicleInfo.nVehicleColorScore = (veu8)carInfo["nVehicleColorScore"].asInt();			//车辆颜色得分
				obj.m_vVehicleInfo.nVehicleTypeScore = (veu8)carInfo["nVehicleTypeScore"].asInt();			//车辆品牌得分
				strcpy(obj.m_vVehicleInfo.cVehicleType, carInfo["cVehicleType"].asString().c_str());					    //车型
				strcpy(obj.m_vVehicleInfo.cVehicleBrand, carInfo["cVehicleBrand"].asString().c_str());					//车辆品牌
				obj.m_vVehicleInfo.nVehicleBrandScore = (veu8)carInfo["nVehicleBrandScore"].asInt();			//车辆品牌得分
				strcpy(obj.m_vVehicleInfo.cVehicleSubBrand, carInfo["cVehicleSubBrand"].asString().c_str());			    //车辆子品牌
				//obj.m_vVehicleInfo.fVehicleCarFeature = (vef32)carFeature["fVehicleCarFeature"].asFloat();			//carFeature
			}
		}
		else if(value.isMember("otherVehicleInfo"))
		{
				const Json::Value NatureInfo = value["otherVehicleInfo"];
				strcpy(obj.m_vNatureInfo.name, NatureInfo["name"].asString().c_str());
				//strcpy(obj.m_vNatureInfo.type, NatureInfo["type"].asString().c_str());
				obj.m_vNatureInfo.type = (veu8)NatureInfo["type"].asInt();
				//strcpy(obj.m_vNatureInfo.pose, NatureInfo["pose"].asString().c_str());
				obj.m_vNatureInfo.pose = (veu8)NatureInfo["pose"].asInt();
				//strcpy(obj.m_vNatureInfo.truncated, NatureInfo["truncated"].asString().c_str());
				obj.m_vNatureInfo.truncated = (veu8)NatureInfo["truncated"].asInt();
				obj.m_vNatureInfo.score = (veu8)NatureInfo["score"].asInt();
		}
		else if(value.isMember("PersonInfo"))
		{	
				cout << "read peopleInfo" << endl;
				const Json::Value personInfo = value["PersonInfo"];
				strcpy(obj.m_vHumanInfo.tfHumanFea[0].cFeature, personInfo["coatColor"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[0].nFeatureScore = (veu8)personInfo["coatColorScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[1].cFeature, personInfo["trousersColor"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[1].nFeatureScore = (veu8)personInfo["trousersColorScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[2].cFeature, personInfo["hairType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[2].nFeatureScore = (veu8)personInfo["hairScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[3].cFeature, personInfo["coatType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[3].nFeatureScore = (veu8)personInfo["coatTypeScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[4].cFeature, personInfo["trousersType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[4].nFeatureScore = (veu8)personInfo["trousersTypeScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[5].cFeature, personInfo["bagType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[5].nFeatureScore = (veu8)personInfo["bagTypeScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[6].cFeature, personInfo["ageType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[6].nFeatureScore = (veu8)personInfo["ageTypeScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[7].cFeature, personInfo["gender"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[7].nFeatureScore = (veu8)personInfo["genderScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[8].cFeature, personInfo["raceType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[8].nFeatureScore = (veu8)personInfo["raceTypeScore"].asInt();
				strcpy(obj.m_vHumanInfo.tfHumanFea[9].cFeature, personInfo["hatType"].asString().c_str());
				obj.m_vHumanInfo.tfHumanFea[9].nFeatureScore = (veu8)personInfo["hatTypeScore"].asInt();
		}
		m_arrObjectInfo.push_back(obj);
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


//获取物体信息
int VideoInfoJsonManager::GetVideoData(VE_Object &vInfo)
{
	if (m_arrObjectInfo.size() > 0)
	{
		vInfo = m_arrObjectInfo[0];
		m_arrObjectInfo.erase(m_arrObjectInfo.begin());
	}
}

