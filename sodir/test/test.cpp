/*************************************************************************
	> File Name: test.cpp
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018年03月15日 星期四 13时00分17秒
 ************************************************************************/

#include<iostream>
#include<iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include "VideoInfoJsonManager.h"

using namespace std;

int main()
{
	cout << "init" << endl;
	VideoInfoJsonManager* pJson = createJsonManager(0);
	if (pJson == NULL)
		cout << "failed" << endl;

	pJson->SetJsonFilePath("/home/xu/jsonparser/jsonparser/sodir/test/");
	while(1)
	{	
		//data push
		cout << "insert RecoTask" << endl;
		VE_ObjectRecoTask* objReco = new VE_ObjectRecoTask;
		objReco->m_vObjInfo.m_uiObjTypeEx = ve_obj_typeex_car;
		strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, "f你好");
		strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, "n你好");
#if 0
		IplImage *pImg = cvLoadImage("img.png");

		if(pImg == nullptr)
			cout << "pjx id null" << endl;
		else
			cout << "not null " << endl;
#endif
		objReco->m_pObjImageEx = cvLoadImage("img.png");
		if(objReco->m_pObjImageEx == nullptr)
		  cout << "load png failed" << endl;
		else 
		  cout << "load png success" << endl;
		objReco->m_pObjImage = cvLoadImage("img.png");
		//cvShowImage("a", objReco.m_pObjImageEx);
		//cvWaitKey(0);

		//insert json
		pJson->InsertJsonData(objReco);
		usleep(1);
	}
	printf("break\n");
}


