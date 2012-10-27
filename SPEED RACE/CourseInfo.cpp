#include "CourseInfo.h"
#include "Global.h"

/*--CourseInfo--*/
//コンストラクタ
CourseInfo::CourseInfo(void)
{
	Yaw		= 0.0f;
	Pitch	= 0.0f;
	Roll	= 0.0f;
	Pos		= D3DXVECTOR3(-500.0f,-50.0f,0.0f);
	ZeroObject(&Map);
	SetupSitus(&Map,NULL);
}
//マップを移動
void CourseInfo::MoveMap(void)
{
	Yaw	+= 1.0f*kDEG_TO_RAD;
	Map.objPos = Pos;
	Map.objYaw = Yaw;
	Map.objPitch = Pitch;
	Map.objRoll = Roll;
}
//Objectを返す
theObject CourseInfo::ReturnObject(void)
{
	return Map;
}