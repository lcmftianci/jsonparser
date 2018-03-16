/*************************************************************************
	> File Name: filenameio.h
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018年03月12日 星期一 14时31分22秒
 ************************************************************************/
#ifndef _FILE_NAME_IO_H__
#define _FILE_NAME_IO_H__

#include<iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

#include <vector>

using namespace std;

//获取文件的最后修改时间
void GetFileLastModifyTime(char *strPath,int *size,long *modify_time);


//根据时间在某个文件夹下创建文件夹,并返回文件夹名称
char* CreateDirectoryByTime(char *chPath);


//根据文件名称创建文件夹,返回值为0表示创建成功，返回1表示文件夹已存在，返回-1表示创建失败位置原因
int createDirectory(char* strPath);


//创建深度文件夹
int safeCreateDirectory(char* strPath);


//获取当前系统时间具体到分秒
int GetCurrentTime();


//获取当前时间具体到小时
int GetCurrentHour(char* strCurrnet);

//遍历文件夹
void ReverseDirctory(const char* strPath, std::vector<std::string> & arrJsonFilePath);


void scanDir(const char *dir, std::vector<std::string>& arrJsonPath, int depth);


//整数转string
std::string IntToStr(int num);


//获取当前执行程序路径
size_t GetCurrentExePath( char* processdir,char* processname, size_t len);


//剪切文件
bool CutFile(std::string strSrc, std::string strDis);


//删除所有文件
bool removeAllFile(char *strDir);

#endif  //FILENAMEIO__
