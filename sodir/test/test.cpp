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
#include "JsonManager.h"
//#include "VideoInfoJsonManager.h"

using namespace std;

void GetData(string strJson, IplImage* pImg)
{
	cout << "callback" << endl;	
}

int main(int argc, char** argv)
{
	cout << "init" << endl;
	JsonManager* pJson = createJsonManager(0);
	if (pJson == NULL)
		cout << "failed" << endl;
#if 1
	pJson->SetJsonFilePath("/home/xu/json/sodir/test/");
	pJson->SetResourceCallback(GetData);
	while(1)
	{	
		//data push
		cout << "insert RecoTask" << endl;
		VE_ObjectRecoTask* objReco = new VE_ObjectRecoTask;
		strcpy(objReco->m_vVideoChannelInfo.cChannelDescript, "1900");
		objReco->m_vVideoChannelInfo.nChannelIndex = 10;
		objReco->m_vObjInfo.m_uiObjTypeEx = ve_obj_typeex_car;
		strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, "f你好");
		strcpy(objReco->m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, "n你好");
		cout << "load img" << endl;
		objReco->m_pObjImageEx = cvLoadImage("id_0_12.jpg");
		objReco->m_pObjImage = cvLoadImage("img.png");
		//insert json
		cout << "insert json" << endl;
		pJson->InsertJsonData(objReco);
		usleep(10);

		cout << "insert RecoTask" << endl;
		VE_ObjectRecoTask* objReco1 = new VE_ObjectRecoTask;
		strcpy(objReco1->m_vVideoChannelInfo.cChannelDescript, "1900");
		objReco1->m_vVideoChannelInfo.nChannelIndex = 10;
		objReco1->m_vObjInfo.m_uiObjTypeEx = ve_obj_typeex_person;
		for(int i = 0; i < 10; i++)
		{
			strcpy(objReco1->m_vObjInfo.m_vHumanInfo.tfHumanFea[i].cFeature, "你好");
			objReco1->m_vObjInfo.m_vHumanInfo.tfHumanFea[i].nFeatureIndex = 10;
			objReco1->m_vObjInfo.m_vHumanInfo.tfHumanFea[i].nFeatureScore = 90;
			//strcpy(objReco->m_vObjInfo.m_vHumanInfo.plateinfo.szColor, "你好");
		}
		cout << "load img" << endl;
		objReco1->m_pObjImageEx = cvLoadImage("id_0_18.jpg");
		objReco1->m_pObjImage = cvLoadImage("img.png");
		//insert json
		cout << "insert json" << endl;
		pJson->InsertJsonData(objReco1);
		usleep(10);
		
		cout << "insert RecoTask" << endl;
		VE_ObjectRecoTask* objReco2 = new VE_ObjectRecoTask;
		strcpy(objReco2->m_vVideoChannelInfo.cChannelDescript, "1900");
		objReco2->m_vVideoChannelInfo.nChannelIndex = 10;
		objReco2->m_vObjInfo.m_uiObjTypeEx = ve_obj_typeex_bicycle;
		strcpy(objReco2->m_vObjInfo.m_vNatureInfo.name, "你好");
		//strcpy(objReco->m_vObjInfo.m_vNatureInfo.plateinfo.szColor, "你好");
		cout << "load img" << endl;
		objReco2->m_pObjImageEx = cvLoadImage("id_0_11.jpg");
		objReco2->m_pObjImage = cvLoadImage("img.png");
		//insert json
		cout << "insert json" << endl;
		pJson->InsertJsonData(objReco2);
		usleep(10);
	}
#endif

#if 0
	while(1)
	{	
		usleep(100);
		pJson->ParseJsonFile(argv[1]);
		cout << "ParseOK" << endl;
		//pJson->Release();
	}
#endif
	getchar();
	printf("break\n");
}


