#include "MenuClass.h"
#include "PartMenu.h"
#include "MenuBack.h"
#include "CourseInfo.h"
#include "Global.h"

/*--MenuClass--*/
//コンストラクタ
MenuClass::MenuClass(int* errorFlag)
{
	ChangeBack = FALSE;
	MenuInfo = kMENU_INFO_MAIN;
	MoveFlag = FALSE;
	PointerMove = FALSE;
	PtPointer = kMENU_KYOUSOUKAISI;
	SettingCourse = kCTYPE_END;
	CoureseRanking = kCTYPE_TOUGE;
	RankingKeyCounter = 10;
	int i;

	for(i = 0;i < kNUM_OF_MENU;i++)
	{
		MenuPart[i] = new PartMenu(i);
	}
	SetCourse = new CourseInfo();
	ZeroObject(&RankingCourse);
	SetupSitus(&RankingCourse,NULL);
	RankingCourse.objPos.y = 80.0f;
	ModelRankingCourseTouge = new ModelClass(kXMENU_TOUGE_RANK,errorFlag);
	ModelRankingCourseGake = new ModelClass(kXMENU_GAKE_RANK,errorFlag);
	MenuModels[kMENU_KYOUSOUKAISI] = new ModelClass(kXMENU_KYOUSOUKAISI,errorFlag);
	MenuModels[kMENU_SOUSAHOUHOU] = new ModelClass(kXMENU_SOUSAHOUHOU,errorFlag);
	MenuModels[kMENU_RANKING] = new ModelClass(kXMENU_RANKING,errorFlag);
	MenuModels[kMENU_SYUURYOU] = new ModelClass(kXMENU_SYUURYOU,errorFlag);
	MenuModels[kMENU_MENU] = new ModelClass(kXMENU_MENU,errorFlag);
	MenuModels[kMENU_MODORU] = new ModelClass(kXMENU_MODORU,errorFlag);
	MenuModels[kMENU_TOUGE] = new ModelClass(kXMENU_TOUGE,errorFlag);
	MenuModels[kMENU_GAKE] = new ModelClass(kXMENU_GAKE,errorFlag);
	MenuModels[kMENU_POINTER] = new ModelClass(kXMENU_POINTER,errorFlag);
	MenuModels[kINFO_HOWTOPLAY] = new ModelClass(kXINFO_HOWTOPLAY,errorFlag);
	MenuModels[kINFO_RANKING] = new ModelClass(kXINFO_RANKING,errorFlag);
	MiniCourse[kCTYPE_TOUGE] = new ModelClass(kXMESH_TOUGE,errorFlag);
	MiniCourse[kCTYPE_GAKE] = new ModelClass(kXMESH_GAKE,errorFlag);
	BackModel = new ModelClass(kXMENU_BACK,errorFlag);
	BackMenu = new MenuBack();
	SetupResultBillbords(g_pD3DDevice,errorFlag);
	if(FAILED(LoadTextureFromImageFiles(kFILE_TEX_ASCII,&BillbordTexture,kImagesPath,g_pD3DDevice))){ *errorFlag = TRUE; }
}
//デストラクタ
MenuClass::~MenuClass(void)
{
	int i;
	for(i = 0;i < kNUM_OF_MENU;i++)
	{
		delete MenuPart[i];
	}
	for(i = 0;i < kNUM_OF_MENU;i++)
	{
		delete MenuModels[i];
	}
	for(i=0;i<kCTYPE_NUM;i++)
	{
		delete MiniCourse[i];
	}
	delete BackModel;
	delete BackMenu;
	delete SetCourse;
	SAFE_RELEASE(BillbordFVFBuffer);
	SAFE_RELEASE(BillbordTexture);
}
//メニューを進める
void MenuClass::DoMenu(void)
{
	ChkMove();
	ChkAct();
	AppPoint();
	MovePointer();
	AppMove();
	BackMenu->MoveBord();
	SetCourse->MoveMap();
}
//メニューの描画
void MenuClass::MenuRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Zバッファとバックバッファのクリア
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(30,30,64), 1.0f, 0 );

	// シーンの開始
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{
		int i,i2;
		SetupRanderStates(theDevice,kRS_MENU);		// レンダリングステート設定

		SetupWorldMatrix(theDevice,&gBillEyePt,&gBillLookatPt,&gBillUp);// カメラ視点設定

		for(i=0;i<(kNUM_OF_MENU-3);i++)
		{
			if((i!=PtPointer)&&(i!=HeadPart)&&(MenuPart[i]->ChkOut()==FALSE))
			{
				drawMesh(theDevice,&MenuPart[i]->ReturnObject(),MenuModels[i]);
			}
		}
		SetupRanderStates(theDevice,kRS_BRIGHT);




		if((MenuPart[HeadPart]->ChkOut())==FALSE){drawMesh(theDevice,&MenuPart[HeadPart]->ReturnObject(),MenuModels[HeadPart]);}
		if((MenuPart[PtPointer]->ChkOut())==FALSE){drawMesh(theDevice,&MenuPart[PtPointer]->ReturnObject(),MenuModels[PtPointer]);}
		if((MenuPart[kINFO_HOWTOPLAY]->ChkOut())==FALSE){drawMesh(theDevice,&MenuPart[kINFO_HOWTOPLAY]->ReturnObject(),MenuModels[kINFO_HOWTOPLAY]);}
		if((MenuPart[kINFO_RANKING]->ChkOut())==FALSE){drawMesh(theDevice,&MenuPart[kINFO_RANKING]->ReturnObject(),MenuModels[kINFO_RANKING]);}

		SetupRanderStates(theDevice,kRS_BACK);
		for(i=0;i<kNUM_BACK_MENU;i++)
		{
			drawMesh(theDevice,&BackMenu->ReturnBord(i),BackModel);
		}

		SetupRanderStates(theDevice,kRS_POINTER);
		drawMesh(theDevice,&MenuPart[kMENU_POINTER]->ReturnObject(),MenuModels[kMENU_POINTER]);
		

		if(SettingCourse != kCTYPE_END)
		{
			SetupWorldMatrix(theDevice,&D3DXVECTOR3(0.0f,1500.0f,1500.0f),&gBillLookatPt,&gBillUp);// カメラ視点設定
			gLightLight.Type       = D3DLIGHT_DIRECTIONAL;
			gLightLight.Diffuse.r  = 1.0f;
			gLightLight.Diffuse.g  = 1.0f;
			gLightLight.Diffuse.b  = 1.0f;
			gLightLight.Diffuse.a  = 0.0f;
			gLightVec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
			SetupLights(theDevice,gLightVec,gLightLight,0);
			SetupRanderStates(theDevice,kRS_MESH);
			drawMesh(theDevice,&SetCourse->ReturnObject(),MiniCourse[SettingCourse]);
		}
		
		if((MenuInfo==kMENU_INFO_RANKING)&&(MoveFlag==FALSE)&&(ChangeBack==FALSE)&&(RankingCounter==0))
		{
			SetupRanderStates(theDevice,kRS_ALPHA);
			for(i=0;(i<5)&&(i<NumList);i++)
			{
				for(i2=0;i2<kNUM_MAX_RANKING;i2++)
				{
					drawFVF(theDevice,&RankingBord[i][i2],BillbordTexture,BillbordFVFBuffer);
				}
			}
			switch(CoureseRanking)
			{
			case kCTYPE_TOUGE:
				drawMesh(theDevice,&RankingCourse,ModelRankingCourseTouge);
				break;
			case kCTYPE_GAKE:
				drawMesh(theDevice,&RankingCourse,ModelRankingCourseGake);
				break;
			}
		}
		// ビルボードを加算合成で描画
		// モデルもそのままで描画できる
/*		SetupRanderStates( theDevice , kRS_ADDBLEND );	// レンダリングステート設定
		for( i0 = 0 ; i0 < kNUM_OF_BILLBORDS ; i0++ )
		{
			if((g_Billbord[i0].LDflag != kLD_STANDBY)&&(g_Billbord[i0].TexType == kTEX_EXPLODE2 ))
			{  drawFVF( theDevice , &g_Billbord[i0] , &gEyePt ); }
		}

		
		//=================================================================情報レイヤの描画
		// Zバッファとバックバッファのクリア
		theDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

		SetupWorldMatrix( theDevice , &gBilEyePt , &gLookatPt , &gBillUpVec );// 情報レイヤ用の視点設定
		//SetupRanderStates( theDevice , kRS_ADDBLEND );
		SetupRanderStates( theDevice , kRS_ALPHA );

		for( i0 = 0 ; i0 < kNUM_OF_INFOBORDS ; i0++ )
		{
			if( g_Infobord[i0].TexType != kTEX_NONE ){ drawFVF( theDevice , &g_Infobord[i0] , &gBilEyePt ); }
		}*/

		//終わりの定型処理
		// シーンの終了
		theDevice->EndScene();
	}

	// バックバッファを表画面に反映させる。
	HRESULT hr = theDevice->Present( NULL, NULL, NULL, NULL );
}
//動きを指定
void MenuClass::AppPoint(void)
{
	D3DXVECTOR3 BufVec = D3DXVECTOR3(0.0f,0.0f,0.0f);
	if(ChangeBack==TRUE)
	{
		MenuPart[kMENU_POINTER]->AppMode(kPART_INFO_OUT,BufVec);
	}
	else
	{
		if(MoveFlag == FALSE)
		{
			int i;
			for(i=0;i<(kNUM_OF_MENU - 1);i++)
			{
				MenuPart[i]->AppMode(kPART_INFO_OUT,BufVec);
			}
			switch(MenuInfo)
			{
			case kMENU_INFO_MAIN:
				MenuPart[kMENU_KYOUSOUKAISI]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_SOUSAHOUHOU]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_RANKING]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_SYUURYOU]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_MENU]->AppMode(kPART_INFO_HEAD,BufVec);
				HeadPart = kMENU_MENU;
				break;
			case kMENU_INFO_SET_COURSE:
				MenuPart[kMENU_KYOUSOUKAISI]->AppMode(kPART_INFO_HEAD,BufVec);
				MenuPart[kMENU_TOUGE]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_GAKE]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kMENU_MODORU]->AppMode(kPART_INFO_IN,BufVec);
				HeadPart = kMENU_KYOUSOUKAISI;
				break;
			case kMENU_INFO_SOUSAHOUHOU:
				MenuPart[kMENU_SOUSAHOUHOU]->AppMode(kPART_INFO_HEAD,BufVec);
				MenuPart[kMENU_MODORU]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kINFO_HOWTOPLAY]->AppMode(kPART_INFO_IN,BufVec);
				HeadPart = kMENU_SOUSAHOUHOU;
				break;
			case kMENU_INFO_RANKING:
				MenuPart[kMENU_RANKING]->AppMode(kPART_INFO_HEAD,BufVec);
				MenuPart[kMENU_MODORU]->AppMode(kPART_INFO_IN,BufVec);
				MenuPart[kINFO_RANKING]->AppMode(kPART_INFO_IN,BufVec);
				HeadPart = kMENU_RANKING;
				break;
			}
		}
	}
}
//動きを確認
void MenuClass::ChkMove(void)
{
	int i;
	PointerMove = FALSE;
	for(i=0;i < (kNUM_OF_MENU - 1);i++)
	{
		if(MenuPart[i]->ChkMove() == TRUE)
		{
			MoveFlag = TRUE;
			break;
		}
		else
		{
			MoveFlag = FALSE;
		}
	}
	if(RankingCounter>0)
	{
		RankingCounter--;
	}
	if(MenuPart[kMENU_POINTER]->ChkMove() == TRUE)
	{
		PointerMove = TRUE;
	}
	if(MoveFlag == FALSE)
	{
		ChangeBack = FALSE;
	}
}
//ポインターを動かす
void MenuClass::MovePointer(void)
{
	if(ChangeBack==FALSE)
	{
		if((MoveFlag == FALSE)&&(PointerMove == FALSE))
		{
			if(g_pKeyState[DIK_UP] & 0x80)
			{
				UpPointer();
			}
			if(g_pKeyState[DIK_DOWN] & 0x80)
			{
				DownPointer();
			}
			MenuPart[kMENU_POINTER]->AppMode(kPART_INFO_CUSTOMPOS,MenuPart[PtPointer]->ReturnPos());
			if((PtPointer == kMENU_TOUGE)||(PtPointer == kMENU_GAKE))
			{
				if(PtPointer == kMENU_TOUGE)
				{
					SettingCourse = kCTYPE_TOUGE;
				}
				if(PtPointer == kMENU_GAKE)
				{
					SettingCourse = kCTYPE_GAKE;
				}
			}
			else
			{
				SettingCourse = kCTYPE_END;
			}
		}
	}
}
//ポインターを上に動かす
void MenuClass::UpPointer(void)
{
	switch(MenuInfo)
	{
	case kMENU_INFO_MAIN:
		PtPointer--;
		PtPointer = (PtPointer + kMENU_SYUURYOU + 1 - kMENU_KYOUSOUKAISI) % (kMENU_SYUURYOU + 1) + kMENU_KYOUSOUKAISI;
		break;
	case kMENU_INFO_SET_COURSE:
		PtPointer--;
		if(PtPointer < kMENU_MODORU)
		{
			PtPointer = kMENU_GAKE;
		}
		if(PtPointer > kMENU_GAKE)
		{
			PtPointer = kMENU_MODORU;
		}
		break;
	case kMENU_INFO_SOUSAHOUHOU:
		break;
	case kMENU_INFO_RANKING:
		break;
	}
}
//ポインターを下に動かす
void MenuClass::DownPointer(void)
{
	switch(MenuInfo)
	{
	case kMENU_INFO_MAIN:
		PtPointer++;
		PtPointer = (PtPointer + kMENU_SYUURYOU + 1 - kMENU_KYOUSOUKAISI) % (kMENU_SYUURYOU + 1) + kMENU_KYOUSOUKAISI;
		break;
	case kMENU_INFO_SET_COURSE:
		PtPointer++;
		if(PtPointer < kMENU_MODORU)
		{
			PtPointer = kMENU_GAKE;
		}
		if(PtPointer > kMENU_GAKE)
		{
			PtPointer = kMENU_MODORU;
		}
		break;
	case kMENU_INFO_SOUSAHOUHOU:
		break;
	case kMENU_INFO_RANKING:
		break;
	}
}
//動かす
void MenuClass::AppMove(void)
{
	int i;
	for(i=0;i<kNUM_OF_MENU;i++)
	{
		MenuPart[i]->MovePart();
	}
}
//アクションをチェック
void MenuClass::ChkAct(void)
{
	if((MenuInfo==kMENU_INFO_RANKING)&&(MoveFlag==FALSE)&&(ChangeBack==FALSE)&&(RankingCounter==0))
	{
		if(RankingKeyCounter > 0)
		{
			RankingKeyCounter--;
		}
		if(((g_pKeyState[DIK_LEFT] & 0x80)||(g_pKeyState[DIK_RIGHT] & 0x80))&&(RankingKeyCounter == 0))
		{
			switch(CoureseRanking)
			{
			case kCTYPE_TOUGE:
				CoureseRanking = kCTYPE_GAKE;
				RankingKeyCounter = 10;
				break;
			case kCTYPE_GAKE:
				CoureseRanking = kCTYPE_TOUGE;
				RankingKeyCounter = 10;
				break;
			}
			RankingSet();
		}
	}
	if((MoveFlag == FALSE)&&(PointerMove == FALSE)&&(ChangeBack==FALSE))
	{
		if(g_pKeyState[DIK_RETURN] & 0x80)
		{
			switch(PtPointer)
			{
			case kMENU_KYOUSOUKAISI:
				MenuInfo = kMENU_INFO_SET_COURSE;
				PtPointer = kMENU_TOUGE;
				SettingCourse = kCTYPE_TOUGE;
				ChangeBack = TRUE;
				break;
			case kMENU_SOUSAHOUHOU:
				MenuInfo = kMENU_INFO_SOUSAHOUHOU;
				PtPointer = kMENU_MODORU;
				ChangeBack = TRUE;
				break;
			case kMENU_RANKING:
				MenuInfo = kMENU_INFO_RANKING;
				PtPointer = kMENU_MODORU;
				ChangeBack = TRUE;
				MoveFlag=TRUE;
				RankingCounter = 10;
				RankingSet();
				break;
			case kMENU_SYUURYOU:
				PostQuitMessage( 0 );
				break;
			case kMENU_TOUGE:
				gCourseType = kCTYPE_TOUGE;
				g_GameMode = kMODE_INIT_RACE;
				break;
			case kMENU_GAKE:
				gCourseType = kCTYPE_GAKE;
				g_GameMode = kMODE_INIT_RACE;
				break;
			case kMENU_MODORU:
				switch(MenuInfo)
				{
				case kMENU_INFO_SOUSAHOUHOU:
					MenuInfo = kMENU_INFO_MAIN;
					PtPointer = kMENU_KYOUSOUKAISI;
					ChangeBack = TRUE;
					break;
				case kMENU_INFO_SET_COURSE:
					MenuInfo = kMENU_INFO_MAIN;
					PtPointer = kMENU_KYOUSOUKAISI;
					ChangeBack = TRUE;
					break;
				case kMENU_INFO_RANKING:
					MenuInfo = kMENU_INFO_MAIN;
					PtPointer = kMENU_KYOUSOUKAISI;
					ChangeBack = TRUE;
					break;
				}
				break;
			}
		}
	}
}
//ランキング
void MenuClass::RankingSet(void)
{
	char line[512];
	int lineCnt=0;
	FILE* RankingFile;
	switch(CoureseRanking)
	{
	case kCTYPE_TOUGE:
		if((RankingFile = fopen(kTEXT_RANKING_TOUGE, "r")) == NULL) {
			PostQuitMessage(0);
			return;
		}
		break;
	case kCTYPE_GAKE:
		if((RankingFile = fopen(kTEXT_RANKING_GAKE, "r")) == NULL) {
			PostQuitMessage(0);
			return;
		}
		break;
	}

	/*EOFまで一行ずつ読み込み、行数をカウントしていく*/
	while(fgets(line,256,RankingFile) != NULL){
		lineCnt++;
	}
	fclose(RankingFile);

	switch(CoureseRanking)
	{
	case kCTYPE_TOUGE:
		if((RankingFile = fopen(kTEXT_RANKING_TOUGE, "r")) == NULL) {
			PostQuitMessage(0);
			return;
		}
		break;
	case kCTYPE_GAKE:
		if((RankingFile = fopen(kTEXT_RANKING_GAKE, "r")) == NULL) {
			PostQuitMessage(0);
			return;
		}
		break;
	}
	NumList = lineCnt / 3;

	theRanking* RankingList;
	RankingList = new theRanking[NumList];
	int i,i2;
	for(i=0;i<NumList;i++)
	{
		fscanf(RankingFile,"%d",&(RankingList[i].RankIndex));
		fscanf(RankingFile,"%s",&(RankingList[i].PlayerName));
		fscanf(RankingFile,"%f",&(RankingList[i].theTime));
	}
	fclose(RankingFile);

	qsort(RankingList,NumList,sizeof(theRanking),sort_ranking);

	for(i=0;i<5;i++)
	{
		for(i2=0;i2<kNUM_MAX_RANKING;i2++)
		{
			ZeroBillbord(&RankingBord[i][i2]);
			RankingBord[i][i2].objPos	= D3DXVECTOR3(450.0f-i2*45.0f,230.0f-i*64.0f,0.0f);
			RankingBord[i][i2].TexType = kTEX_ASCII;
			RankingBord[i][i2].objScal = 0.6f;
		}
	}
	for(i=0;(i<5)&&(i<NumList);i++)
	{
		sprintf(RankingChar[i],"No.%d  %8.8s  %03.2f",i+1,RankingList[i].PlayerName,RankingList[i].theTime);
	}
	for(i=0;(i<5)&&(i<NumList);i++)
	{
		for(i2=0;i2<kNUM_MAX_RANKING;i2++)
		{
			RankingBord[i][i2].TexIndex = RankingChar[i][i2];
		}
	}
	delete RankingList;
}
//ビルボード作成
void MenuClass::SetupResultBillbords(LPDIRECT3DDEVICE9 theDevice,int* errorFlag)
{
	if( FAILED(theDevice->CreateVertexBuffer(	sizeof(theCUSTOMVERTEX)*4,
												0,
												kD3DFVF_CUSTOMVERTEX,
												D3DPOOL_MANAGED,
												&BillbordFVFBuffer,
												NULL )))
	{
		MessageBox( NULL, "CreateVertexBufferに失敗", kERROR_MSG, MB_OK|MB_ICONHAND );
		*errorFlag = TRUE;
		return;
	}

    theCUSTOMVERTEX* tmpVertex;

	// 頂点バッファをロック
	if( FAILED( BillbordFVFBuffer->Lock( 0, 0, (void**)&tmpVertex, 0 ) ) )
	{
		*errorFlag = TRUE;
		return;
	}

	// 頂点は四つ、頂点座標、頂点色、テクスチャ座標を設定する
	// D3DPT_TRIANGLESTRIPで描画するので頂点の番号と座標の対応に注意

	float	tmpSize = kSIZE_OF_BILLBOARD / 2.0f;
	tmpVertex[0].position	= D3DXVECTOR3( -tmpSize , -tmpSize , 0.0f );
	tmpVertex[0].tu			= 0.0f;
	tmpVertex[0].tv			= 0.0f;

	tmpVertex[1].position	= D3DXVECTOR3( tmpSize , -tmpSize , 0.0f );
	tmpVertex[1].tu			= 1.0f;
	tmpVertex[1].tv			= 0.0f;

	tmpVertex[2].position	= D3DXVECTOR3( -tmpSize , tmpSize , 0.0f );
	tmpVertex[2].tu			= 0.0f;
	tmpVertex[2].tv			= 1.0f;

	tmpVertex[3].position	= D3DXVECTOR3( tmpSize , tmpSize , 0.0f );
	tmpVertex[3].tu			= 1.0f;
	tmpVertex[3].tv			= 1.0f;

	// 書き込みが終わったので頂点バッファのロックを解除する
	BillbordFVFBuffer->Unlock();
}