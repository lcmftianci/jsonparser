/*************************************************************************
	> File Name: filenameio.h
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018年03月12日 星期一 14时31分22秒
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

//清空文件夹
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

//剪切文件
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
		//putchar(ch); //是in.txt 的内容显示在dos窗口 下
		ch = fgetc(in);
	}
	fclose(in); // 关闭文件
	fclose(out);

	remove(strSrc.c_str());
}


//获取当前执行程序路径
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

//获取文件的最后修改时间
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


//根据时间在某个文件夹下创建文件夹,并返回文件夹名称
char* CreateDirectoryByTime(char *chPath)
{
	return NULL;
}


//根据文件名称创建文件夹,返回值为0表示创建成功，返回1表示文件夹已存在，返回-1表示创建失败位置原因
int createDirectory(char* strPath)
{
	int nCreate;
	nCreate=mkdir(strPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if(!nCreate)
		cout<<"create success"<< strPath <<endl;
	else
		cout<<"create path failed"<<endl;
	return nCreate;
}


//创建深度文件夹
int safeCreateDirectory(char* strPath)
{
	//依次获取路径中的/或者\并创建文件夹
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


//获取当前系统时间
int GetCurrentTime()
{
	time_t now;
	time(&now);
	return now;
}


//获取当前时间具体到小时
int GetCurrentHour(char* strCurrent)
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time); //获取相对于1970到现在的秒数
	tm nowTime;
	localtime_r(&time.tv_sec, &nowTime);
//	char current[1024];
//	sprintf(current, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
	sprintf(strCurrent, "%04d%02d%02d%02d", nowTime.tm_year + 1900, nowTime.tm_mon, nowTime.tm_mday, nowTime.tm_hour);

//	cout << strCurrent << endl;

	return 0;
}

//遍历文件夹
void ReverseDirctory(const char* strPath, std::vector<std::string> & arrJsonFilePath)
{
#if 0
	intptr_t handle;
    _finddata_t findData;
	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
	       cout << "Failed to find first file!\n";
	       return;
	}
	
	do
	{
	      if (findData.attrib & _A_SUBDIR && strcmp(findData.name, ".") == 0 && strcmp(findData.name, "..") == 0)    // 是否是子目录并且不为"."或".."
			  cout << findData.name << "\t<dir>\n";
		  else
		      cout << findData.name << "\t" << findData.size << endl;
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
				
	cout << "Done!\n";
	_findclose(handle);    // 关闭搜索句柄
#endif
}

void scanDir(const char *dir,std::vector<std::string>& arrJsonPath, int depth)
{
	DIR *dp;                      // 定义子目录流指针  
	struct dirent *entry;         // 定义dirent结构指针保存后续目录  
	struct stat statbuf;          // 定义statbuf结构保存文件属性  
	if((dp = opendir(dir)) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功  
	{  
	     puts("can't open dir.");  
	     return;  
	}  
	chdir (dir);                     // 切换到当前目录  
	while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环  
	{  
	     lstat(entry->d_name, &statbuf); // 获取下一级成员属性  
	     if(S_IFDIR &statbuf.st_mode)    // 判断下一级成员是否是目录  
	     {  
	          if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
	              continue;  
	          printf("%*s%s/\n", depth, "", entry->d_name);  // 输出目录名称  
	     //     scanDir(entry->d_name, depth+4);              // 递归调用自身，扫描下一级目录，但是我们只遍历当前目录下的所有json文件 
	     }  
	     else  
		 {
			 //printf("%*s%s\n", depth, "", entry->d_name);  // 输出属性不是目录的成员 
			 arrJsonPath.push_back(entry->d_name);
		 }
	}  
	chdir("..");                                                  // 回到上级目录  
	closedir(dp);                                                 // 关闭子目录流
}


//整数转string
std::string IntToStr(int num)
{
	char strNum[100];
	sprintf(strNum, "%d", num);
	return string(strNum);
}
