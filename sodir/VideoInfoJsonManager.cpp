#include "VideoInfoJsonManager.h"

#include <fstream>
#include "json/writer.h"
#include "json/json.h"
#include "json/value.h"
#include "filenameio.h"
#include <pthread.h>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
#include <iconv.h>
#include "base64.h"

//引入命名空间
using namespace std;
using namespace cv;

pthread_mutex_t g_mutex;

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

#if 0
unsigned char *base64_decode(const char *code)    
{    
	//根据base64表，以字符找到对应的十进制数据    
	 int table[]={0,0,0,0,0,0,0,0,0,0,0,0,  
	              0,0,0,0,0,0,0,0,0,0,0,0,  
	              0,0,0,0,0,0,0,0,0,0,0,0,  
	              0,0,0,0,0,0,0,62,0,0,0,  
	              63,52,53,54,55,56,57,58,  
	              59,60,61,0,0,0,0,0,0,0,0,  
	              1,2,3,4,5,6,7,8,9,10,11,12,  
	              13,14,15,16,17,18,19,20,21,  
	              22,23,24,25,0,0,0,0,0,0,26,  
	              27,28,29,30,31,32,33,34,35,  
	              36,37,38,39,40,41,42,43,44,  
	              45,46,47,48,49,50,51};    
      long len;    
      long str_len;    
      unsigned char *res;    
      int i,j;    
      //计算解码后的字符串长度    
      len=strlen(code);    
      //判断编码后的字符串后是否有=    
      if(strstr(code,"=="))    
          str_len=len/4*3-2;    
      else if(strstr(code,"="))    
          str_len=len/4*3-1;    
	  else    
	      str_len=len/4;    
	  res=malloc(sizeof(unsigned char)*str_len+1);    
	  res[str_len]='\0';    
	  //以4个字符为一位进行解码    
	  for(i=0,j=0;i < len-2;j+=3,i+=4)    
	  {    
	        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合    
	        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合    
	        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合    
	  }
																																																					 return res;
}
int	base64_decode( const char * base64, unsigned char * bindata)
{
	int i, j;
	unsigned char k;
	unsigned char temp[4];
	for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
	{
	    memset( temp, 0xFF, sizeof(temp) );
	    for ( k = 0 ; k < 64 ; k ++ )
	    {
	         if ( base64char[k] == base64[i] )
	              temp[0]= k;
	    }
	    for ( k = 0 ; k < 64 ; k ++ )
	    {
	         if ( base64char[k] == base64[i+1] )
	               temp[1]= k;
	    }
	    for ( k = 0 ; k < 64 ; k ++ )
	    {
	         if ( base64char[k] == base64[i+2] )
	               temp[2]= k;
	    }
	    for ( k = 0 ; k < 64 ; k ++ )
	    {
	         if ( base64char[k] == base64[i+3] )
	               temp[3]= k;
	    }
							
	    bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) | ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;
					
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) | ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
	    if ( base64[i+3] == '=' )
	        break;
																				
	    bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) | ((unsigned char)(temp[3]&0x3F));	
	}
    return j;
}

#endif

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

VideoInfoJsonManager::~VideoInfoJsonManager()
{
//	delete m_pBase64;
	pthread_mutex_destroy(&g_mutex);
}


VideoInfoJsonManager::VideoInfoJsonManager(int model):m_nModel(model),m_nChannel(1),m_strJsonFilePath("")
{
	pthread_mutex_init(&g_mutex, NULL);
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
	//创建base64实例
//	m_pBase64 = new CBase64;

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
	pthread_mutex_lock(&g_mutex);
	m_arrObjRecoTask.push_back(pInfo);
	cout << "InsertJsonData Ok" << endl;
	pthread_mutex_unlock(&g_mutex);
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

string GetSaveJsonPath(string strRoot,VE_ObjectRecoTask* objReco)
{
	if(strRoot == "")
	{
		cout << "json path error!" << endl;
			return "";
	}
	
	//根据时间戳穿件文件
	char jsonPath[1024];
	char jsonName[1024]; 
	
	//createDirectory
	sprintf(jsonPath, "/%s_%d", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex);//写入IP;
	string l_strPath = strRoot;
	l_strPath += jsonPath;	//文件夹目录合成完毕
	createDirectory(l_strPath.c_str());	//创建文件夹目录
	
	//get Save Path
	sprintf(jsonName, "/%s_%d_%lld_%d.json", objReco->m_vVideoChannelInfo.cChannelDescript, objReco->m_vVideoChannelInfo.nChannelIndex,  objReco->m_vObjInfo.m_uiTimeStamp, g_index);	
	l_strPath += jsonName;
	printf("JSON Name is %s \n",l_strPath.c_str());
	
	return l_strPath;
}

//保存json文件到文件
int VideoInfoJsonManager::SaveJsonFile()
{	
	{
		cout << "In SaveJsonFile" << endl;	
		if(m_strJsonFilePath == "")
		{
			cout << "json path error!" << endl;
			return 0;
		}
		
		ofstream ofs;
		
		//开始写入json数据
		Json::Value arrInfo;
		Json::Value root;
		VE_ObjectRecoTask* objReco = m_arrObjRecoTask.front();
		
		string l_strPath = GetSaveJsonPath(m_strJsonFilePath, objReco);
		
		//这里需要根据传过来的信息判断是那一路相机需要放到那个文件夹下
		ofs.open(l_strPath.c_str());//根据当前时间给json文件命名
	
		//申请存储图片的空间
		string strBase64Out;
		size_t outlen;
		
		char lResDst[256];
		if(objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_bus || 
		   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_car ||
		   objReco->m_vObjInfo.m_uiObjTypeEx == ve_obj_typeex_truck)
		{
			cout << "carInfo" << endl;
			Json::Value carFeature;
			Json::Value carInfo;
			Json::Value car;
			
		//	gb2312toutf8(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, strlen(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate), lResDst, 256);
		//	printf("%s \n %s \n",objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate,lResDst);
		//	std::string plate = lResDst;
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

			carInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;			//图片宽度
			carInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;			//图片高度
			carInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//图片通道
	
			carFeature["fVehicleCarFeature"] = objReco->m_vObjInfo.m_vVehicleInfo.fVehicleCarFeature;
			//将IplImage读取为base64格式	
			CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->imageSize, strBase64Out);
			cout << "convert base64 end" << endl;
			//CBase64::Encode(pBuf, objReco->m_pObjImageEx->imageSize, strBase64Out);
			carInfo["m_pObjImageEx"] = strBase64Out.c_str();
			//carInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
			cout << "base 64 endl" << endl;
			car["carInfo"] = carInfo;
			car["carFeature"] = carFeature;
			root["totcalCarInfo"] = car;
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
			NatureInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;			//图片宽度
			NatureInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;			//图片高度
			NatureInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//图片通道
			//NatureInfo["m_pObjImageEx"] = base64_encode(objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, &out, &outlen);
			CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, strBase64Out);
			cout << "convert base64 end" << endl;
			NatureInfo["m_pObjImageEx"] = strBase64Out.c_str();
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
			personInfo["ImageExWidth"] = objReco->m_pObjImageEx->width;				//图片宽度
			personInfo["ImageExHeight"] = objReco->m_pObjImageEx->height;				//图片高度
			personInfo["ImageExChannel"] = objReco->m_pObjImageEx->nChannels;			//图片通道
			CBase64::Encode((const unsigned char*)objReco->m_pObjImageEx->imageData, objReco->m_pObjImageEx->nSize, strBase64Out);
			cout << "convert base64 end" << endl;
			personInfo["m_pObjImageEx"] = strBase64Out.c_str();
			root["PersonInifo"]		= personInfo;
		}

		std::string strJson = root.toStyledString();
		ofs.write(strJson.c_str(), strlen(strJson.c_str()));
		ofs.close();
		//清空队列里的某个信息
		cvReleaseImage(&(objReco->m_pObjImage));
		cvReleaseImage(&(objReco->m_pObjImageEx));
		delete objReco;
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
		cout << "hello--" << endl;
	}
}
#endif

void VideoInfoJsonManager::ParseJson()
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
		VE_ObjectRecoTask* objReco = new VE_ObjectRecoTask;
		string strIn;
		unsigned char* pOut;
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
				const Json::Value carInfo = arrObj["carInfo"];
				const Json::Value carFeature = arrObj["carFeature"];
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, carInfo["szPlate"].asString().c_str());						//车牌号码
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, carInfo["szColor"].asString().c_str());						//车牌颜色
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleColor, carInfo["cVehicleColorScore"].asString().c_str());					//车辆颜色
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleColorScore = (veu8)carInfo["nVehicleColorScore"].asInt();			//车辆颜色得分
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleTypeScore = (veu8)carInfo["nVehicleTypeScore"].asInt();			//车辆品牌得分
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleType, carInfo["cVehicleType"].asString().c_str());					    //车型
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleBrand, carInfo["cVehicleBrand"].asString().c_str());					//车辆品牌
				objReco->m_vObjInfo.m_vVehicleInfo.nVehicleBrandScore = (veu8)carInfo["nVehicleBrandScore"].asInt();			//车辆品牌得分
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.cVehicleSubBrand, carInfo["cVehicleSubBrand"].asString().c_str());			    //车辆子品牌
				strcpy(objReco->m_vObjInfo.m_vVehicleInfo.fVehicleCarFeature, carFeature["fVehicleCarFeature"].asString().c_str());			//carFeature
				
				//获取图片宽高
				IplImage *pImg = cvCreateImage(cvSize(carInfo["ImageExWidth"].asInt(), carInfo["ImageExHeight"].asInt()), 8, carInfo["ImageExChannel"].asInt()); 
				strIn = carInfo["m_pObjImageEx"].asString();
				CBase64::Decode(strIn, pOut, &uOutLen);						//解码base64
				//合成图片
				memcpy(pImg->imageData, pOut, uOutLen);
				objReco->m_pObjImageEx = pImg;
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
				
				//获取图片宽高
				IplImage *pImg = cvCreateImage(cvSize(NatureInfo["ImageExWidth"].asInt(), NatureInfo["ImageExHeight"].asInt()), 8, NatureInfo["ImageExChannel"].asInt()); 
				strIn = NatureInfo["m_pObjImageEx"].asString();
				CBase64::Decode(strIn, pOut, &uOutLen);						//解码base64
				//合成图片
				memcpy(pImg->imageData, pOut, uOutLen);
				objReco->m_pObjImageEx = pImg;
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
				//获取图片宽高
				IplImage *pImg = cvCreateImage(cvSize(personInfo["ImageExWidth"].asInt(), personInfo["ImageExHeight"].asInt()), 8, personInfo["ImageExChannel"].asInt()); 
				strIn = personInfo["m_pObjImageEx"].asString();
				CBase64::Decode(strIn, pOut, &uOutLen);						//解码base64
				//合成图片
				memcpy(pImg->imageData, pOut, uOutLen);
				objReco->m_pObjImageEx = pImg;
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
	pthread_mutex_lock(&g_mutex);
	if (m_arrObjRecoTask.size() > 0)
	{
		vInfo = m_arrObjRecoTask[0];
		m_arrObjRecoTask.erase(m_arrObjRecoTask.begin());
	}
	pthread_mutex_unlock(&g_mutex);
}

