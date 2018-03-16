/*************************************************************************
	> File Name: filenameio.h
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018��03��12�� ����һ 14ʱ31��22��
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

//��ȡ�ļ�������޸�ʱ��
void GetFileLastModifyTime(char *strPath,int *size,long *modify_time);


//����ʱ����ĳ���ļ����´����ļ���,�������ļ�������
char* CreateDirectoryByTime(char *chPath);


//�����ļ����ƴ����ļ���,����ֵΪ0��ʾ�����ɹ�������1��ʾ�ļ����Ѵ��ڣ�����-1��ʾ����ʧ��λ��ԭ��
int createDirectory(char* strPath);


//��������ļ���
int safeCreateDirectory(char* strPath);


//��ȡ��ǰϵͳʱ����嵽����
int GetCurrentTime();


//��ȡ��ǰʱ����嵽Сʱ
int GetCurrentHour(char* strCurrnet);

//�����ļ���
void ReverseDirctory(const char* strPath, std::vector<std::string> & arrJsonFilePath);


void scanDir(const char *dir, std::vector<std::string>& arrJsonPath, int depth);


//����תstring
std::string IntToStr(int num);


//��ȡ��ǰִ�г���·��
size_t GetCurrentExePath( char* processdir,char* processname, size_t len);


//�����ļ�
bool CutFile(std::string strSrc, std::string strDis);


//ɾ�������ļ�
bool removeAllFile(char *strDir);

#endif  //FILENAMEIO__
