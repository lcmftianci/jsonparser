#include <iostream>
#include "JsonManager.h"
#include "VideoInfoJsonManager.h"



void *createJsonManagerInternal(int nModel)
{
	return new  VideoInfoJsonManager(nModel);
}

JsonManager::~JsonManager()
{

}

JsonManager::JsonManager()
{
	
}
/*
	设置存储json文件的文件夹路径
*/
int JsonManager::SetJsonFilePath(std::string strJsonFilePath, int nChannel)
{
  	return 0;
}

/*
	获取json字符串和IplImage数据
*/
void JsonManager::SetResourceCallback(RESOURCE_CALLBACK callback)
{

}
	
/*
	获取数据
*/
int JsonManager::InsertJsonData(VE_ObjectRecoTask *Info)
{
	return 0;
}
