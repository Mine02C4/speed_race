#include "ModelClass.h"

/*--ModelClass--*/
//�R���X�g���N�^
ModelClass::ModelClass(const TCHAR* ModelName,int* errorFlag)
{
	InitModel();
	if(FAILED(LoadFromXFileModel(ModelName,g_pD3DDevice)))
	{
		*errorFlag = TRUE;
	}
}
//�f�X�g���N�^
ModelClass::~ModelClass(void)
{
	/*���b�V���}�e���A���̉��*/
	SAFE_DELETE_ARRAY(objMeshMaterials);

	/*�e�N�X�`���̉��*/
	if( objMeshTextures )
	{
		for(DWORD j0 = 0; j0 < objNumOfMaterials ; j0++ ){ SAFE_RELEASE( objMeshTextures[j0] );}
		SAFE_DELETE_ARRAY( objMeshTextures );
	}

	/*���b�V���̊J��*/
	SAFE_RELEASE( objMesh );
}
//X�t�@�C������̃��f���f�[�^�̓ǂݍ���
HRESULT ModelClass::LoadFromXFileModel(LPCTSTR FileName, LPDIRECT3DDEVICE9 theDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;	/*�ꎞ�o�b�t�@�p*/

	// X�t�@�C���̓ǂݍ��݁B
	if(FAILED(D3DXLoadMeshFromX(FileName, D3DXMESH_SYSTEMMEM,theDevice, NULL,&pD3DXMtrlBuffer,NULL,&objNumOfMaterials,&objMesh)))
	{
		TCHAR cautionString[kSTR_LENGTH];
		wsprintf(cautionString,TEXT("X�t�@�C���w%s�x��������܂���ł����B"),FileName);
		MessageBox(NULL,cautionString,kWINDOW_TITLE,MB_OK);
		return(E_FAIL);
	}

	//=========================================================================
	// pD3DXMtrlBuffer����A�}�e���A����e�N�X�`���̖��̂Ȃǂ̏����擾����B
	D3DXMATERIAL* d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	objMeshMaterials= new D3DMATERIAL9[objNumOfMaterials];
	objMeshTextures	= new LPDIRECT3DTEXTURE9[objNumOfMaterials];

	for(DWORD i0=0; i0 < objNumOfMaterials ; i0++ )
	{
		// �����imaterial�j�̃R�s�[
		objMeshMaterials[i0] = d3dxMaterials[i0].MatD3D;

		// �����imaterial�j�Ɋ�����ݒ肷��iD3DX���������Ȃ����߁j
		objMeshMaterials[i0].Ambient = objMeshMaterials[i0].Diffuse;

		objMeshTextures[i0] = NULL;
		if( d3dxMaterials[i0].pTextureFilename != NULL && lstrlen(d3dxMaterials[i0].pTextureFilename) > 0 )
		{
			if(FAILED( LoadTexture(d3dxMaterials[i0].pTextureFilename,&objMeshTextures[i0],kModelsPath,theDevice)))
			{
				TCHAR cautionString[kSTR_LENGTH];

				wsprintf(cautionString,TEXT("X�t�@�C���w%s�x�Ŏw�肳�ꂽ�e�N�X�`���}�b�v�t�@�C���w%s�x��������܂���ł����B"),FileName,d3dxMaterials[i0].pTextureFilename);
				MessageBox(NULL,cautionString,kWINDOW_TITLE,MB_OK);
				return( E_FAIL );
			}
		}
	}

	// �����imaterial�j�o�b�t�@�̊J���B
	pD3DXMtrlBuffer->Release();

	return(S_OK);
}
//�摜�t�@�C�����e�N�X�`���Ƃ��ēǂݍ���
HRESULT ModelClass::LoadTexture(LPTSTR FileName, LPDIRECT3DTEXTURE9* aTexture, LPCTSTR aSearchPath, LPDIRECT3DDEVICE9 theDevice)
{
	// �e�N�X�`���̐���
	// ������܂��J�����g�t�H���_��T���A������Ȃ��ꍇ�́umodels�v�t�H���_����T��
	if(FAILED(D3DXCreateTextureFromFile(theDevice,FileName,aTexture)))
	{
		return(E_FAIL);
	}

	return( S_OK );
}
//���f���̏�����
void ModelClass::InitModel(void)
{
	objMesh				= NULL;
	objMeshMaterials	= NULL;
	objMeshTextures		= NULL;
	objNumOfMaterials	= 0;
}
//���b�V����Ԃ�
LPD3DXMESH ModelClass::ReturnMesh(void)
{
	return objMesh;
}
//�}�e���A���̐���Ԃ�
DWORD ModelClass::ReturnNumOfMaterials(void)
{
	return objNumOfMaterials;
}
//�}�e���A����Ԃ�
D3DMATERIAL9* ModelClass::ReturnMeshMaterials(void)
{
	return objMeshMaterials;
}
//�e�N�X�`����Ԃ�
LPDIRECT3DTEXTURE9* ModelClass::ReturnMeshTextures(void)
{
	return objMeshTextures;
}