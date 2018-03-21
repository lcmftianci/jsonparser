#ifndef _JSON_MANAGER_H__
#define _JSON_MANAGER_H__

#include "VeObjectDefine.h"

typedef void (*RESOURCE_CALLBACK)(std::string strJsonData, IplImage* pImg);

extern "C"
{
	void *createJsonManagerInternal(int nModel);
}

class JsonManager
{
public:
	JsonManager();
	virtual ~JsonManager();

	/*
		获取json字符串和IplImage数据
	*/
	virtual void SetResourceCallback(RESOURCE_CALLBACK callback);
	
	/*
		获取数据
	*/
	virtual int InsertJsonData(VE_ObjectRecoTask *Info);

	/*
		设置存储json文件的文件夹路径
	*/
	virtual int SetJsonFilePath(std::string strJsonFilePath, int nChannel = 0);
	
	/*
		解析json字符串
	*/
	virtual int ParseJson(std::string strJson, VE_ObjectRecoTask *objReco) = 0;
	
};

inline JsonManager * createJsonManager(int nModel = 0){ return reinterpret_cast<JsonManager*>(createJsonManagerInternal(nModel));} 

#endif

