#ifndef ___CourseInfo
#define ___CourseInfo

#include "speedrace_head1.h"
/*--���j���[���̃N���X--*/
class CourseInfo
{
	D3DXVECTOR3	Pos;
	float		Yaw;			//���[
	float		Roll;			//���[��
	float		Pitch;			//�s�b�`
	theObject	Map;
public:
	CourseInfo(void);
	void		MoveMap(void);
	theObject	ReturnObject(void);
};

#endif