#ifndef _VIDEO_INFO_JSON_MANAGER_H__
#define _VIDEO_INFO_JSON_MANAGER_H__


/*
	VideoJsonInfoManager 类设计

	输入总文件夹，根据视屏描述和channel新建每一路的文件夹:ip_channel
	
	根据视频描述和channel还有时间戳构建json文件路径：ip_channel_timestamp.json			

	一个写入程序，一个读取程序,一路视频一个文件夹，每个文件夹里面有很多按时间创建的文件夹(一个小时一个文件夹)

	时间文件家里的每个文件都是一个对象，每个对象都可以用前端程序读取并显示

	只要程序启动就会创建新的文件夹

	判断文件夹是否存在，如果存在不再创建直接读取里面的json数据

	如何让另一个程序读取这个刚刚存储的文件呢，获取文件创建的时间
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
	 	构造函数：
		输入参数：int model: 0 表示保存文件，1表示解析文件
	 */
	VideoInfoJsonManager(int model = 0);

	/*
	 	析构函数
	 */
	virtual ~VideoInfoJsonManager();

public:	
	/*
		获取json字符串和IplImage数据
	*/
	//void SetResourceCallback(RESOURCE_CALLBACK callback, void *pUserData){ m_callback = callback; m_pUserData = pUserData;}
	void SetResourceCallback(RESOURCE_CALLBACK callback){ m_callback = callback;}
	
	/*
		获取数据
	*/
	int InsertJsonData(VE_ObjectRecoTask *Info);

	/*
		设置存储json文件的文件夹路径
	*/
	int SetJsonFilePath(std::string strJsonFilePath, int nChannel = 0);

	/*
		保存数据
	*/
	int SaveJsonFile();
	
	/*
		解析数据并且开放用于显示
	*/
	int ParseJsonFile(std::string strJsonPath);


	/*
		析构obj,test
	*/
	int Release();

	/*
		获取当前目录下的所有json文件
	 	存入成员变量vetor里面，然后逐个解析，解析一个就将json文件移动到另一个文件夹里
	 	如果遍历完了之后，就将vector清空重新遍历文件夹
	 	如果过完一个小时
	 */
	void ParseJson();
    	
	/*
		函数名称：GetObjRecoTaskSize()
		返回值：返回当前存储对象的大小
	*/
	int GetObjRecoTaskSize(){ return m_arrObjRecoTask.size();}
	
public:
	//此函数用于将数据推送给Qt前端程序显示
	int GetVideoData(VE_ObjectRecoTask *Info);


	//获取所有json文件的路径
	int GetJsonFilePath(std::vector<std::string> &arrJsonPath);


	//获取已经读取过后的文件夹下的json文件翻遍回看
	//size_t GetVideoDataAfterRead(std::vector<VE_Object> & arrAfterObj);
	size_t GetVideoDataAfterRead(std::vector<VE_ObjectRecoTask*> & arrAfterObj);

private:
	//std::vector<VE_Object> m_arrObjectInfo;		//存储视频信息队列

	std::vector<VE_ObjectRecoTask*> m_arrObjRecoTask;	//视频队列
	std::vector<std::string> m_arrJsonPath;		//存储为显示的数据	m_arrJsonPath[0];
	std::vector<std::string> m_arrJsonPathAfter;//存储显示过的数据	m_arrJsonPathAfter[0];

	int m_nModel;								//模式设置。写是0读是1
	int m_nChannel;								//设置记录相机

	/*
	 *example:
	 * home/xxx/jsonparser/
	 * home/xxx/jsonparser/2018031314/ ----需要便利的文件夹，内含json文件
	 * home/xxx/jsonparser/2018011314/bak/ -----存储浏览或的json文件
	 * */
	std::string strExePath;						//合成jsonpath用                            
	std::string m_strJsonFilePath;				//当前search path,根据时间获取的文件路径
	std::string m_strJsonFilePathAfterRead;      		//open after path
	RESOURCE_CALLBACK m_callback;				//获取数据的回调函数
	void 	   *m_pUserData;				//用户数据
	
//	CBase64	    *m_pBase64;						//base64编解器
};


VideoInfoJsonManager* createJsonDataManager(int model = 0);


#endif //_VIDEO_INFO_JSON_MANAGER_H__
