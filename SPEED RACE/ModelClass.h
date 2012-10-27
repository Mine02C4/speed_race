#ifndef ___ModelClass
#define ___ModelClass

#include "speedrace_head1.h"
#include "CarClass.h"
#include "RaceClass.h"
/*--���f���i�[�p�N���X--*/
class	ModelClass
{
	LPD3DXMESH				objMesh;			// �V�X�e�����̃��b�V���I�u�W�F�N�g
	D3DMATERIAL9*			objMeshMaterials;	// ���b�V���̎���(materials)
	LPDIRECT3DTEXTURE9*		objMeshTextures;	// ���b�V���̃e�N�X�`��(texture)
	DWORD					objNumOfMaterials;	// ���b�V�������̐�
	HRESULT		LoadFromXFileModel(LPCTSTR,LPDIRECT3DDEVICE9);
	HRESULT		LoadTexture( LPTSTR, LPDIRECT3DTEXTURE9*,LPCTSTR, LPDIRECT3DDEVICE9);
	void		InitModel(void);
public:
	ModelClass(const TCHAR*,int*);
	~ModelClass(void);
	LPD3DXMESH			ReturnMesh(void);
	DWORD				ReturnNumOfMaterials(void);
	D3DMATERIAL9*		ReturnMeshMaterials(void);
	LPDIRECT3DTEXTURE9*	ReturnMeshTextures(void);
};

#include "Global.h"
#endif