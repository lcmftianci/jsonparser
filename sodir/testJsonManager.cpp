#include "VideoInfoJsonManager.h"

#include <iostream>
#include "filenameio.h"

#include <vector>


using namespace std;

int main(int argc, char** argv)
{

	VideoInfoJsonManager* pJson = createJsonManager(0);
	if (pJson == NULL)
		cout << "failed" << endl;

//test insert
#if 0
	VE_Object veo;
	veo.m_uiObjTypeEx = ve_obj_typeex_car;
	strcpy(veo.m_vVehicleInfo.plateinfo.szPlate, "asd");
	strcpy(veo.m_vVehicleInfo.plateinfo.szColor, "rgb");
	pJson->InsertJsonData(veo);
#endif

//test parser
    //pJson->ParseJsonFile(argv[1]);


#if 0
//	safeCreateDirectory("vidol\\abn\\hjk\\");

	cout << GetCurrentTime() << endl;
	//获取当前时间具体到小时
	char strCur[40];
	GetCurrentHour(strCur);

	cout << strCur << endl;


	std::vector<char*> arrPath;

//	ReverseDirectory(argv[1], arrPath);

	scanDir(argv[1], arrPath, 0);


	std::vector<char*>::iterator iterPath = arrPath.begin();
	for(; iterPath != arrPath.end(); ++iterPath)
	{
		cout << "-----" <<*iterPath << endl;
	}

#endif
	getchar();

#if 0 
	int size;
    long modifytime;
	GetFileLastModifyTime(argv[1], &size, &modifytime);	


	cout << "hello json\n" << size << "---" << modifytime << endl;
#endif
	
	return 0;
}
