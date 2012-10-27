#include "GameResult.h"
#include "Global.h"

/*--GameResult--*/
//コンストラクタ
GameResult::GameResult(int* errorFlag)
{
	ResultInfo = kRESULT_INFO_SPIN;
	int i;
	for(i=0;i<5;i++)
	{
		ZeroObject(&NumberRoll[i]);
		SetupSitus(&NumberRoll[i],NULL);
	}
	ZeroObject(&KoronBord);
	SetupSitus(&KoronBord,NULL);
	RollNum = new ModelClass(kXRESULT_NUMROLL,errorFlag);
	KoronMesh = new ModelClass(kXRESULT_KORON,errorFlag);
	ModelNameIn = new ModelClass(kXRESULT_NAME_IN,errorFlag);
	ModelForm = new ModelClass(kXRESULT_FORM,errorFlag);
	FormBord = new PartMenu(kINFO_FORM);
	NameIn = new PartMenu(kINFO_NAME_IN);
	BackModel = new ModelClass(kXMENU_BACK,errorFlag);
	BackMenu = new MenuBack();

	SetupResultBillbords(g_pD3DDevice,errorFlag);

	MoveFlag = FALSE;

	theTime = gGameResultTime;
	for(i=0;i<5;i++)
	{
		SpinFlag[i] = TRUE;
	}
	TimeCounter = 0;
	NumTime[0] = int(theTime/100000);
	NumTime[1] = int((theTime%100000)/10000);
	NumTime[2] = int((theTime%100000%10000)/1000);
	NumTime[3] = int((theTime%100000%10000%1000)/100);
	NumTime[4] = int((theTime%100000%10000%1000%100)/10);

	NumberRoll[0].objPos.x = 360.0f;
	NumberRoll[1].objPos.x = 240.0f;
	NumberRoll[2].objPos.x = 120.0f;
	NumberRoll[3].objPos.x = -120.0f;
	NumberRoll[4].objPos.x = -240.0f;

	KoronBord.objPos.z = 200.0f;

	if(FAILED(LoadTextureFromImageFiles(kFILE_TEX_ASCII,&BillbordTexture,kImagesPath,g_pD3DDevice))){ *errorFlag = TRUE; }

	for(i=0;i<kNUM_MAX_PLAYERNAME;i++)
	{
		ZeroBillbord(&NameBord[i]);
		NameBord[i].objPos	= D3DXVECTOR3(230.0f-i*64.0f,0.0f,0.0f);
		NameBord[i].TexType = kTEX_ASCII;
	}
	for(i=0;i<6;i++)
	{
		ZeroBillbord(&RaceTime[i]);
		RaceTime[i].objPos = D3DXVECTOR3(80.0f-i*64.0f,210.0f,1.0f);
		RaceTime[i].TexType = kTEX_ASCII;
	}
	for(i=0;i<10;i++)
	{
		ZeroBillbord(&RankBord[i]);
		RankBord[i].objPos = D3DXVECTOR3(80.0f-i*64.0f,150.0f,1.0f);
		RankBord[i].TexType = kTEX_ASCII;
	}

	char	tmpChar[10];
	FloatTime = NumTime[0] * 10000 + NumTime[1] * 1000 + NumTime[2] * 100 + NumTime[3] * 10 + NumTime[4];
	FloatTime /= 100;
	sprintf(tmpChar,"%03.2f",FloatTime);

	//----------------------------
	// 情報レイヤ処理	
	for(i=0;i<7;i++)
	{
		RaceTime[i].TexIndex = tmpChar[i];
	}
}
//デストラクタ
GameResult::~GameResult(void)
{
	delete ModelNameIn;
	delete NameIn;
	delete RollNum;
	delete KoronMesh;
	delete ModelForm;
	delete FormBord;
	delete BackModel;
	delete BackMenu;
	SAFE_RELEASE(BillbordFVFBuffer);
	SAFE_RELEASE(BillbordTexture);
}
//結果を進める
void GameResult::DoResult(void)
{
	if(ResultInfo==kRESULT_INFO_SPIN)
	{
		SpinRoll();
	}
	if(ResultInfo==kRESULT_INFO_STOP)
	{
		if(g_pKeyState[DIK_RETURN] & 0x80)
		{
			ResultInfo = kRESULT_INFO_NAME_SET;
			PlayerName = new std::string;
			StringPointer = 0;

			char line[256];
			int lineCnt=0;

			FILE* RankingFile;
			switch(gCourseType)
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

			switch(gCourseType)
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
			int i;
			for(i=0;i<NumList;i++)
			{
				fscanf(RankingFile,"%d",&(RankingList[i].RankIndex));
				fscanf(RankingFile,"%s",&(RankingList[i].PlayerName));
				fscanf(RankingFile,"%f",&(RankingList[i].theTime));
			}
			fclose(RankingFile);
			qsort(RankingList,NumList,sizeof(theRanking*),sort_ranking);
			for(i=0;i<NumList;i++)
			{
				if(RankingList[i].theTime > FloatTime)
				{
					PlayerRank = i+1;
					break;
				}
				if(i==(NumList-1))
				{
					PlayerRank = NumList +1;
				}
			}
			char	tmpChar[12];
			sprintf(tmpChar,"%d",PlayerRank);

			//----------------------------
			// 情報レイヤ処理	
			for(i=0;i<11;i++)
			{
				RankBord[i].TexIndex = tmpChar[i];
			}
		}
	}
	if(ResultInfo==kRESULT_INFO_NAME_SET)
	{
		SetName();
		UpdateResultInfo();
		if((g_pKeyState[DIK_RETURN] & 0x80)&&(MoveFlag==FALSE))
		{
			FILE* RankingFile;
			switch(gCourseType)
			{
			case kCTYPE_TOUGE:
				if((RankingFile = fopen(kTEXT_RANKING_TOUGE, "a+")) == NULL) {
					PostQuitMessage(0);
					return;
				}
				break;
			case kCTYPE_GAKE:
				if((RankingFile = fopen(kTEXT_RANKING_GAKE, "a+")) == NULL) {
					PostQuitMessage(0);
					return;
				}
				break;
			}
			fprintf(RankingFile,"\n%d\n",NumList+1);
			char BufChar[kNUM_MAX_PLAYERNAME+1];
			sprintf( BufChar , "%s", PlayerName->c_str());
			if(BufChar[0]=='\0')
			{
				fprintf(RankingFile,"NoName\n");
			}
			else
			{
				fprintf(RankingFile,"%s\n",PlayerName->c_str());
			}
			fprintf(RankingFile,"%f",FloatTime);
			g_GameMode = kMODE_END_RESULT;
		}
	}
	BackMenu->MoveBord();
}
//ロールを回す
void GameResult::SpinRoll(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		if(SpinFlag[i]==TRUE)
		{
			NumberRoll[i].objRoll += 20*kDEG_TO_RAD;
		}
	}
	TimeCounter++;
	if(TimeCounter==40)
	{
		SpinFlag[4] = FALSE;
		NumberRoll[4].objRoll = ((NumTime[4]+1)%10)*36*kDEG_TO_RAD;
	}
	if(TimeCounter==80)
	{
		SpinFlag[3] = FALSE;
		NumberRoll[3].objRoll = ((NumTime[3]+1)%10)*36*kDEG_TO_RAD;
	}
	if(TimeCounter==120)
	{
		SpinFlag[2] = FALSE;
		NumberRoll[2].objRoll = ((NumTime[2]+1)%10)*36*kDEG_TO_RAD;
	}
	if(TimeCounter==160)
	{
		SpinFlag[1] = FALSE;
		NumberRoll[1].objRoll = ((NumTime[1]+1)%10)*36*kDEG_TO_RAD;
	}
	if(TimeCounter==200)
	{
		SpinFlag[0] = FALSE;
		NumberRoll[0].objRoll = ((NumTime[0]+1)%10)*36*kDEG_TO_RAD;
		ResultInfo = kRESULT_INFO_STOP;
	}
}
void GameResult::ResultRender(LPDIRECT3DDEVICE9 theDevice)
{
	// Zバッファとバックバッファのクリア
	theDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(30,30,64), 1.0f, 0 );

	// シーンの開始
	if( SUCCEEDED( theDevice->BeginScene() ) )
	{
		int i;
		
		SetupWorldMatrix(theDevice,&gBillEyePt,&gBillLookatPt,&gBillUp);// カメラ視点設定

		gLightLight.Type       = D3DLIGHT_DIRECTIONAL;
		gLightLight.Diffuse.r  = 1.0f;
		gLightLight.Diffuse.g  = 1.0f;
		gLightLight.Diffuse.b  = 1.0f;
		gLightLight.Diffuse.a  = 0.0f;
		gLightVec = D3DXVECTOR3(0.0f,0.0f,-1.0f);
		SetupLights(theDevice,gLightVec,gLightLight,0);

		if((ResultInfo==kRESULT_INFO_SPIN)||(ResultInfo==kRESULT_INFO_STOP))
		{
			SetupRanderStates(theDevice,kRS_MESH);		// レンダリングステート設定
			for(i=0;i<5;i++)
			{
				drawMesh(theDevice,&NumberRoll[i],RollNum);
			}
			drawMesh(theDevice,&KoronBord,KoronMesh);
		}

		SetupRanderStates(theDevice,kRS_BACK);
		for(i=0;i<kNUM_BACK_MENU;i++)
		{
			drawMesh(theDevice,&BackMenu->ReturnBord(i),BackModel);
		}
		
		

		SetupRanderStates(theDevice,kRS_BRIGHT);
		if(ResultInfo==kRESULT_INFO_NAME_SET)
		{
			drawMesh(theDevice,&(NameIn->ReturnObject()),ModelNameIn);
			drawMesh(theDevice,&(FormBord->ReturnObject()),ModelForm);
		}
		if((ResultInfo==kRESULT_INFO_NAME_SET)&&(MoveFlag==FALSE))
		{
			SetupRanderStates(theDevice,kRS_INFO);
			for(i=0;i<kNUM_MAX_PLAYERNAME;i++)
			{
				drawFVF(theDevice,&NameBord[i],BillbordTexture,BillbordFVFBuffer);
			}
			for(i=0;i<10;i++)
			{
				drawFVF(theDevice,&RankBord[i],BillbordTexture,BillbordFVFBuffer);
			}
			for(i=0;i<6;i++)
			{
				drawFVF(theDevice,&RaceTime[i],BillbordTexture,BillbordFVFBuffer);
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
//キーボードをチェック
void GameResult::ChkKey(void)
{
	if(g_pKeyState[DIK_RETURN] & 0x80)
	{
		ResultInfo = kRESULT_INFO_NAME_SET;
	}
}
//名前を入力
void GameResult::SetName(void)
{
	D3DXVECTOR3 BufVec;
	if(MoveFlag == FALSE)
	{
		NameIn->AppMode(kPART_INFO_HEAD,BufVec);
		FormBord->AppMode(kPART_INFO_IN,BufVec);
	}
	if((NameIn->ChkMove()==TRUE)||(FormBord->ChkMove()==TRUE))
	{
		MoveFlag = TRUE;
	}
	else
	{
		MoveFlag = FALSE;
	}
	int i;
	for(i=0;i<256;i++)
	{
		if((i!=DIK_LSHIFT)&&(i!=DIK_RSHIFT))
		{
			if(LastKeyState[i]!=0)
			{
				if(g_pKeyState[i] & 0x80)
				{
					g_pKeyState[i] = NULL;
					LastKeyState[i]--;
					if(LastKeyState[i]<0)
					{
						LastKeyState[i] = 0;
					}
				}
				else
				{
					LastKeyState[i] = FALSE;
				}
			}
			else
			{
				if(g_pKeyState[i] & 0x80)
				{
					LastKeyState[i] = 20;
				}
			}
		}
	}
	if(MoveFlag==FALSE)
	{
		if((g_pKeyState[DIK_LSHIFT] & 0x80)||(g_pKeyState[DIK_RSHIFT] & 0x80))
		{
			if((g_pKeyState[DIK_0] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"~");
				StringPointer++;
			}
			if((g_pKeyState[DIK_1] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"!");
				StringPointer++;
			}
			if((g_pKeyState[DIK_2] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"\"");
				StringPointer++;
			}
			if((g_pKeyState[DIK_3] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"#");
				StringPointer++;
			}
			if((g_pKeyState[DIK_4] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"$");
				StringPointer++;
			}
			if((g_pKeyState[DIK_5] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"%");
				StringPointer++;
			}
			if((g_pKeyState[DIK_6] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"&");
				StringPointer++;
			}
			if((g_pKeyState[DIK_7] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"\'");
				StringPointer++;
			}
			if((g_pKeyState[DIK_8] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"(");
				StringPointer++;
			}
			if((g_pKeyState[DIK_9] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,")");
				StringPointer++;
			}
			if((g_pKeyState[DIK_A] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"A");
				StringPointer++;
			}
			if((g_pKeyState[DIK_B] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"B");
				StringPointer++;
			}
			if((g_pKeyState[DIK_C] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"C");
				StringPointer++;
			}
			if((g_pKeyState[DIK_D] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"D");
				StringPointer++;
			}
			if((g_pKeyState[DIK_E] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"E");
				StringPointer++;
			}
			if((g_pKeyState[DIK_F] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"F");
				StringPointer++;
			}
			if((g_pKeyState[DIK_G] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"G");
				StringPointer++;
			}
			if((g_pKeyState[DIK_H] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"H");
				StringPointer++;
			}
			if((g_pKeyState[DIK_I] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"I");
				StringPointer++;
			}
			if((g_pKeyState[DIK_J] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"J");
				StringPointer++;
			}
			if((g_pKeyState[DIK_K] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"K");
				StringPointer++;
			}
			if((g_pKeyState[DIK_L] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"L");
				StringPointer++;
			}
			if((g_pKeyState[DIK_M] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"M");
				StringPointer++;
			}
			if((g_pKeyState[DIK_N] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"N");
				StringPointer++;
			}
			if((g_pKeyState[DIK_O] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"O");
				StringPointer++;
			}
			if((g_pKeyState[DIK_P] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"P");
				StringPointer++;
			}
			if((g_pKeyState[DIK_Q] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"Q");
				StringPointer++;
			}
			if((g_pKeyState[DIK_R] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"R");
				StringPointer++;
			}
			if((g_pKeyState[DIK_S] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"S");
				StringPointer++;
			}
			if((g_pKeyState[DIK_T] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"T");
				StringPointer++;
			}
			if((g_pKeyState[DIK_U] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"U");
				StringPointer++;
			}
			if((g_pKeyState[DIK_V] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"V");
				StringPointer++;
			}
			if((g_pKeyState[DIK_W] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"W");
				StringPointer++;
			}
			if((g_pKeyState[DIK_X] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"X");
				StringPointer++;
			}
			if((g_pKeyState[DIK_Y] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"Z");
				StringPointer++;
			}
			if((g_pKeyState[DIK_MINUS] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"=");
				StringPointer++;
			}
			if((g_pKeyState[DIK_PREVTRACK] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"-");
				StringPointer++;
			}
			if((g_pKeyState[DIK_YEN] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"|");
				StringPointer++;
			}
			if((g_pKeyState[DIK_AT] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"`");
				StringPointer++;
			}
			if((g_pKeyState[DIK_LBRACKET] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"{");
				StringPointer++;
			}
			if((g_pKeyState[DIK_SEMICOLON] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"+");
				StringPointer++;
			}
			if((g_pKeyState[DIK_COLON] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"*");
				StringPointer++;
			}
			if((g_pKeyState[DIK_RBRACKET] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"}");
				StringPointer++;
			}
			if((g_pKeyState[DIK_COMMA] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"<");
				StringPointer++;
			}
			if((g_pKeyState[DIK_PERIOD] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,">");
				StringPointer++;
			}
			if((g_pKeyState[DIK_SLASH] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"?");
				StringPointer++;
			}
			if((g_pKeyState[DIK_BACKSLASH] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"_");
				StringPointer++;
			}
		}
		else
		{
			if((g_pKeyState[DIK_0] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"0");
				StringPointer++;
			}
			if((g_pKeyState[DIK_1] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"1");
				StringPointer++;
			}
			if((g_pKeyState[DIK_2] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"2");
				StringPointer++;
			}
			if((g_pKeyState[DIK_3] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"3");
				StringPointer++;
			}
			if((g_pKeyState[DIK_4] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"4");
				StringPointer++;
			}
			if((g_pKeyState[DIK_5] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"5");
				StringPointer++;
			}
			if((g_pKeyState[DIK_6] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"6");
				StringPointer++;
			}
			if((g_pKeyState[DIK_7] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"7");
				StringPointer++;
			}
			if((g_pKeyState[DIK_8] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"8");
				StringPointer++;
			}
			if((g_pKeyState[DIK_9] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"9");
				StringPointer++;
			}
			if((g_pKeyState[DIK_A] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"a");
				StringPointer++;
			}
			if((g_pKeyState[DIK_B] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"b");
				StringPointer++;
			}
			if((g_pKeyState[DIK_C] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"c");
				StringPointer++;
			}
			if((g_pKeyState[DIK_D] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"d");
				StringPointer++;
			}
			if((g_pKeyState[DIK_E] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"e");
				StringPointer++;
			}
			if((g_pKeyState[DIK_F] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"f");
				StringPointer++;
			}
			if((g_pKeyState[DIK_G] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"g");
				StringPointer++;
			}
			if((g_pKeyState[DIK_H] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"h");
				StringPointer++;
			}
			if((g_pKeyState[DIK_I] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"i");
				StringPointer++;
			}
			if((g_pKeyState[DIK_J] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"j");
				StringPointer++;
			}
			if((g_pKeyState[DIK_K] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"k");
				StringPointer++;
			}
			if((g_pKeyState[DIK_L] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"l");
				StringPointer++;
			}
			if((g_pKeyState[DIK_M] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"m");
				StringPointer++;
			}
			if((g_pKeyState[DIK_N] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"n");
				StringPointer++;
			}
			if((g_pKeyState[DIK_O] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"o");
				StringPointer++;
			}
			if((g_pKeyState[DIK_P] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"p");
				StringPointer++;
			}
			if((g_pKeyState[DIK_Q] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"q");
				StringPointer++;
			}
			if((g_pKeyState[DIK_R] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"r");
				StringPointer++;
			}
			if((g_pKeyState[DIK_S] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"s");
				StringPointer++;
			}
			if((g_pKeyState[DIK_T] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"t");
				StringPointer++;
			}
			if((g_pKeyState[DIK_U] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"u");
				StringPointer++;
			}
			if((g_pKeyState[DIK_V] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"v");
				StringPointer++;
			}
			if((g_pKeyState[DIK_W] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"w");
				StringPointer++;
			}
			if((g_pKeyState[DIK_X] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"x");
				StringPointer++;
			}
			if((g_pKeyState[DIK_Y] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"z");
				StringPointer++;
			}
			if((g_pKeyState[DIK_MINUS] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"-");
				StringPointer++;
			}
			if((g_pKeyState[DIK_PREVTRACK] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"^");
				StringPointer++;
			}
			if((g_pKeyState[DIK_YEN] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"\\");
				StringPointer++;
			}
			if((g_pKeyState[DIK_AT] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"@");
				StringPointer++;
			}
			if((g_pKeyState[DIK_LBRACKET] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"[");
				StringPointer++;
			}
			if((g_pKeyState[DIK_SEMICOLON] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,");");
				StringPointer++;
			}
			if((g_pKeyState[DIK_COLON] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,":");
				StringPointer++;
			}
			if((g_pKeyState[DIK_RBRACKET] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"]");
				StringPointer++;
			}
			if((g_pKeyState[DIK_COMMA] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,",");
				StringPointer++;
			}
			if((g_pKeyState[DIK_PERIOD] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,".");
				StringPointer++;
			}
			if((g_pKeyState[DIK_SLASH] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"/");
				StringPointer++;
			}
			if((g_pKeyState[DIK_BACKSLASH] & 0x80)&&(StringPointer<kNUM_MAX_PLAYERNAME))
			{
				PlayerName->replace(StringPointer,StringPointer,"\\");
				StringPointer++;
			}
		}
		if((g_pKeyState[DIK_BACK] & 0x80)&&(StringPointer>-1))
		{
			if(StringPointer>0)
			{
				StringPointer--;
			}
			PlayerName->replace(StringPointer,StringPointer,"");
		}
	}
	NameIn->MovePart();
	FormBord->MovePart();
}
//ビルボード作成
void GameResult::SetupResultBillbords(LPDIRECT3DDEVICE9 theDevice,int* errorFlag)
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
//文字情報を表示
void GameResult::UpdateResultInfo(void)
{
	int i;

	char	tmpChar[kNUM_MAX_PLAYERNAME+1];
	for(i=0;i<(kNUM_MAX_PLAYERNAME+1);i++)
	{
		tmpChar[i] = '\0';
	}
	sprintf( tmpChar , "%s", PlayerName->c_str());

	//----------------------------
	// 情報レイヤ処理	
	for(i=0;i<kNUM_MAX_PLAYERNAME;i++)
	{
		NameBord[i].TexIndex = NULL;
	}
	for(i=0;i<kNUM_MAX_PLAYERNAME;i++)
	{
		//文字コードが0x00だったら、そこで処理を打ち切る
		if( tmpChar[i] == '\0' ) {
			break;
		}
		NameBord[i].TexIndex = tmpChar[i];
	}
}
