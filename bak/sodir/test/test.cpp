/*************************************************************************
	> File Name: test.cpp
	> Author: lcmf
	> Mail: mrhlingchen@163.com 
	> Created Time: 2018年03月15日 星期四 13时00分17秒
 ************************************************************************/

#include<iostream>
#include "VideoInfoJsonManager.h"

using namespace std;

int main()
{
	cout << "init" << endl;
	VideoInfoJsonManager* pJson = createJsonManager(0);
	if (pJson == NULL)
		cout << "failed" << endl;

	cout << "insert data" << endl;
//data push
	cout << "insert RecoTask" << endl;
	VE_ObjectRecoTask objReco;
	objReco.m_vObjInfo.m_uiObjTypeEx = ve_obj_typeex_car;
	strcpy(objReco.m_vObjInfo.m_vVehicleInfo.plateinfo.szPlate, "asd");
	strcpy(objReco.m_vObjInfo.m_vVehicleInfo.plateinfo.szColor, "rgb");
	cout << "insert Json" << endl;

	pJson->SetJsonFilePath("/home/long");

//insert json
	pJson->InsertJsonData(&objReco);
	printf("hello\n");
	getchar();
}


