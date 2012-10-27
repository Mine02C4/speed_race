#include "MapClass.h"

/*--MapClass--*/
//�R���X�g���N�^
MapClass::MapClass(int typeMap,int* errorFlag)
: MapType(typeMap)
{
	ZeroObject( &Map );
	ZeroObject( &Wall );
	ZeroObject( &End );
	SetupSitus( &Map, kMDL_G_MAP01 );
	SetupSitus( &Wall, kMDL_W_WALL01 );
	SetupSitus( &End, kMDL_E_END);
	g_ObjModels[Map.objType] = new ModelClass(kCourseIndex[MapType][0],errorFlag);
	g_ObjModels[Wall.objType] = new ModelClass(kCourseIndex[MapType][1],errorFlag);
	g_ObjModels[End.objType] = new ModelClass(kCourseIndex[MapType][2],errorFlag);
}
//�f�X�g���N�^
MapClass::~MapClass(void)
{
	delete g_ObjModels[Map.objType];
	delete g_ObjModels[Wall.objType];
}
//Map��Object��Ԃ�
theObject MapClass::ReturnMap()
{
	return Map;
}
//Wall��Object��Ԃ�
theObject MapClass::ReturnWall()
{
	return Wall;
}
//End��Ԃ�
theObject MapClass::ReturnEnd(void)
{
	return End;
}