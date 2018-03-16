/*************************************************************************
	> File Name: filenameio.h
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018��03��12�� ����һ 14ʱ31��22��
 ************************************************************************/

#include<iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
//#include <io.h>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>

#include "filenameio.h"

using namespace std;

//����ļ���
bool removeAllFile(char *strDir)
{
	 DIR *dp;  
	 struct dirent *entry;  
	 struct stat statbuf;  
     if ((dp = opendir(strDir)) == NULL)  
	 {  
	       fprintf(stderr, "cannot open directory: %s\n", strDir);  
	          return -1;  
	 }  
	 chdir (strDir);  
	 while ((entry = readdir(dp)) != NULL)  
	 {  
	     lstat(entry->d_name, &statbuf);  
	     if (S_ISREG(statbuf.st_mode))  
	     {  
	         remove(entry->d_name);  
	     }  
	 }  
	 return 0;  
}

//�����ļ�
bool CutFile(std::string strSrc, std::string strDis)
{
	FILE *in, *out;
	char ch ;
	if ((in = fopen(strSrc.c_str(),"r")) == NULL)
	{
		printf("canot find the in.txt file!\n");
		return false;
	}
	if ((out = fopen(strDis.c_str(),"w"))==NULL)
	{
		printf("canot find the out.txt file!\n");
		return false;
	}
	ch = fgetc(in);
	while (ch!=EOF)
	{
		fputc(ch,out);
		//putchar(ch); //��in.txt ��������ʾ��dos���� ��
		ch = fgetc(in);
	}
	fclose(in); // �ر��ļ�
	fclose(out);

	remove(strSrc.c_str());
}


//��ȡ��ǰִ�г���·��
size_t GetCurrentExePath( char* processdir,char* processname, size_t len)  
{  
       char* path_end;  
       if(readlink("/proc/self/exe", processdir,len) <=0)  
               return -1;  
       path_end = strrchr(processdir,  '/');  
       if(path_end == NULL)  
              return -1;  
       ++path_end;  
       strcpy(processname, path_end);  
       *path_end = '\0';  
       return (size_t)(path_end - processdir);  
} 

//��ȡ�ļ�������޸�ʱ��
void GetFileLastModifyTime(char *strPath,int *size,long *modify_time)
{
	FILE * fp;
	int fd;
	struct stat buf;
	fp=fopen(strPath,"r"); //C.zip in current directory, I use it as a test
	fd=fileno(fp);
	fstat(fd, &buf);
	
	*size = buf.st_size; //get file size (byte)
	*modify_time = buf.st_mtime; //latest modification time (seconds passed from 01/01/00:00:00 1970 UTC)
	fclose(fp);
	fp = NULL;
}


//����ʱ����ĳ���ļ����´����ļ���,�������ļ�������
char* CreateDirectoryByTime(char *chPath)
{
	return NULL;
}


//�����ļ����ƴ����ļ���,����ֵΪ0��ʾ�����ɹ�������1��ʾ�ļ����Ѵ��ڣ�����-1��ʾ����ʧ��λ��ԭ��
int createDirectory(const char* strPath)
{
	int nCreate;
	nCreate=mkdir(strPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(!nCreate)
		cout<<"create success"<< strPath <<endl;
	else
		cout<<"create path failed"<<endl;
	return nCreate;
}


//��������ļ���
int safeCreateDirectory(char* strPath)
{
	//���λ�ȡ·���е�/����\�������ļ���
	char *chPath;
	chPath = (char*)malloc(1024);
	strcpy(chPath, strPath);
	char *p = (char*)malloc(1024);
	while(1)
	{
		p = strchr(chPath, '\\');
		if(p != NULL)
		{
			cout << p << endl;

			unsigned char tmp = (unsigned char)(strchr(chPath, '\\') - chPath);  
			char* pre = (tmp > 0)?strndup(chPath, tmp):strdup(chPath);

			createDirectory(pre);

			chdir(pre);

			int size = strlen(p);
			
			cout << size << endl;

			if(size == 1)
				break;

			cout << size << endl;

			strcpy(chPath, p + 1);
			cout << chPath << endl;
			cout << "------------------------------------" << endl;
			free(pre);
		}
		else
		{
			break;
			//return -1;
		}
	}

	free(p);
	free(chPath);
	return 0;
}


//��ȡ��ǰϵͳʱ��
int GetCurrentTime()
{
	time_t now;
	time(&now);
	return now;
}


//��ȡ��ǰʱ����嵽Сʱ
int GetCurrentHour(char* strCurrent)
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time); //��ȡ�����1970�����ڵ�����
	tm nowTime;
	localtime_r(&time.tv_sec, &nowTime);
//	char current[1024];
//	sprintf(current, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
	sprintf(strCurrent, "%04d%02d%02d%02d", nowTime.tm_year + 1900, nowTime.tm_mon, nowTime.tm_mday, nowTime.tm_hour);

//	cout << strCurrent << endl;

	return 0;
}

//�����ļ���
void ReverseDirctory(const char* strPath, std::vector<std::string> & arrJsonFilePath)
{
#if 0
	intptr_t handle;
    _finddata_t findData;
	handle = _findfirst(dir, &findData);    // ����Ŀ¼�еĵ�һ���ļ�
	if (handle == -1)
	{
	       cout << "Failed to find first file!\n";
	       return;
	}
	
	do
	{
	      if (findData.attrib & _A_SUBDIR && strcmp(findData.name, ".") == 0 && strcmp(findData.name, "..") == 0)    // �Ƿ�����Ŀ¼���Ҳ�Ϊ"."��".."
			  cout << findData.name << "\t<dir>\n";
		  else
		      cout << findData.name << "\t" << findData.size << endl;
	} while (_findnext(handle, &findData) == 0);    // ����Ŀ¼�е���һ���ļ�
				
	cout << "Done!\n";
	_findclose(handle);    // �ر��������
#endif
}

void scanDir(const char *dir,std::vector<std::string>& arrJsonPath, int depth)
{
	DIR *dp;                      // ������Ŀ¼��ָ��  
	struct dirent *entry;         // ����dirent�ṹָ�뱣�����Ŀ¼  
	struct stat statbuf;          // ����statbuf�ṹ�����ļ�����  
	if((dp = opendir(dir)) == NULL) // ��Ŀ¼����ȡ��Ŀ¼��ָ�룬�жϲ����Ƿ�ɹ�  
	{  
	     puts("can't open dir.");  
	     return;  
	}  
	chdir (dir);                     // �л�����ǰĿ¼  
	while((entry = readdir(dp)) != NULL)  // ��ȡ��һ��Ŀ¼��Ϣ�����δ����ѭ��  
	{  
	     lstat(entry->d_name, &statbuf); // ��ȡ��һ����Ա����  
	     if(S_IFDIR &statbuf.st_mode)    // �ж���һ����Ա�Ƿ���Ŀ¼  
	     {  
	          if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
	              continue;  
	          printf("%*s%s/\n", depth, "", entry->d_name);  // ���Ŀ¼����  
	     //     scanDir(entry->d_name, depth+4);              // �ݹ��������ɨ����һ��Ŀ¼����������ֻ������ǰĿ¼�µ�����json�ļ� 
	     }  
	     else  
		 {
			 //printf("%*s%s\n", depth, "", entry->d_name);  // ������Բ���Ŀ¼�ĳ�Ա 
			 arrJsonPath.push_back(entry->d_name);
		 }
	}  
	chdir("..");                                                  // �ص��ϼ�Ŀ¼  
	closedir(dp);                                                 // �ر���Ŀ¼��
}


//����תstring
std::string IntToStr(int num)
{
	char strNum[100];
	sprintf(strNum, "%d", num);
	return string(strNum);
}
