#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"
#include "csv.h"
#include "TextList.h"
// #include "ReadCSV.h"

Scene g_Scene;

static ESceneType s_nextScene = SCENE_NULL;

#pragma region TitleScene

#define SOLID 0
#define SHADED 1
#define BLENDED 2

const wchar_t* str[] = {
	L"여기는 타이틀씬입니다. 텍스트와 관련된 여러가지를 테스트해봅시다.",
	L"B키를 누르면 폰트가 굵게 변합니다.",
	L"I키를 누르면 폰트가 이탤릭체로 변합니다.",
	L"U키를 누르면 텍스트에 밑줄이 생깁니다.",
	L"S키를 누르면 텍스트에 취소선이 생깁니다.",
	L"N키를 누르면 다시 원래대로 돌아옵니다.",
	L"C키를 누르면 렌더 모드가 바뀝니다. (Solid -> Shaded -> Blended)",
	L"1키를 누르면 텍스트가 작아집니다.",
	L"2키를 누르면 텍스트가 커집니다.",
	L"스페이스 키를 누르면 다음 씬으로 넘어갑니다."
};

typedef struct TitleSceneData
{
	Text	GuideLine[10];
	Text	TestText;
	int32	FontSize;
	int32	RenderMode;

	float   ActiveTime;

	Image	TitleBackground;
	int32	TitleBack_X;
	int32	TitleBack_Y;

	Image	PressSpaceKey;
	int32	PressSpace_X;
	int32	PressSpace_Y;

} TitleSceneData;

void init_title(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));

	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	for (int32 i = 0; i < 10; ++i)
	{
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, str[i], wcslen(str[i]));
	}

	data->FontSize = 24;
	Text_CreateText(&data->TestText, "d2coding.ttf", data->FontSize, L"이 텍스트가 변합니다.", 13);

	data->RenderMode = SOLID;

	Image_LoadImage(&data->TitleBackground, "NewFrontier(1).png");
	Image_LoadImage(&data->PressSpaceKey, "NewFrontier(2).png");

	data->TitleBack_X = 0;
	data->TitleBack_Y - 0;
	data->PressSpace_X = 0;
	data->PressSpace_Y = 0;
}

void update_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	data->ActiveTime += Timer_GetDeltaTime();

	if ((int32)data->ActiveTime % 2 == 0)
	{
		Image_SetAlphaValue(&data->PressSpaceKey, 255);
	}
	else if ((int32)data->ActiveTime % 2 != 0)
	{
		Image_SetAlphaValue(&data->PressSpaceKey, 0);
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_MAIN);
	}
}

void render_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	for (int32 i = 0; i < 10; ++i)
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 10, 20 * i, color);

	}
	/*																지워~~~~~~~~~~~~~~~
	switch (data->RenderMode)
	{
	case SOLID:
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->TestText, 400, 400, color);
	}
	break;
	case SHADED:
	{
		SDL_Color bg = { .a = 255 };
		SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
		Renderer_DrawTextShaded(&data->TestText, 400, 400, fg, bg);
	}
	break;
	case BLENDED:
	{
		Renderer_DrawImage(&data->TestImage, 400, 400);
		SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
		Renderer_DrawTextBlended(&data->TestText, 400, 400, color);
	}
	break;
	}
	*/
	Renderer_DrawImage(&data->TitleBackground, data->TitleBack_X, data->TitleBack_Y);
	Renderer_DrawImage(&data->PressSpaceKey, data->PressSpace_X, data->PressSpace_Y);


}

void release_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 10; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}
	Text_FreeText(&data->TestText);

	SafeFree(g_Scene.Data);
}
#pragma endregion

//Main
#pragma region MainScene 1

typedef struct MainSceneData
{
	CsvFile		CsvFile;
	int32		SelectButtonQuantity;

	Text		TextLine[TEXT_MAX_LINE];
	bool		IsText;
	bool		DeltaRun;
	float		TextDeltaTime;
	int32		TextLength;

	Music		BGM;
	float		Volume;
	SoundEffect Effect;
	float		Speed;

	Image		BackGround;
	int32		Back_X;
	int32		Back_Y;

	Image		SelectButton;
	int32		Select_X;
	int32		Select_Y;

	Image		PointerButton;
	int32		Pointer_X;
	int32		Pointer_Y;

	Image		TextUI;
	int32		TextUI_X;
	int32		TextUI_Y;

	int32		Alpha;
} MainSceneData;

void logOnFinished(void)
{
	LogInfo("You can show this log on stopped the music");
}

void log2OnFinished(int32 channel)
{
	LogInfo("You can show this log on stopped the effect");
}


static char* s_Buffer;
static char* s_BufferPointer;
static int32 s_CurrentPage = 1;
static int32 s_PrevPage = 0;
static char* s_PrevBGM = "";
void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	CreateCsvFile(&data->CsvFile, "DB_project.CSV");
	
	data->IsText = false;
	data->DeltaRun = false;
	data->TextDeltaTime = 0.0f;

	int32 Row = s_CurrentPage;
	char* str_background = ParseToAscii(data->CsvFile.Items[Row][BACK_IMG_NAME]);
	if (NULL != *str_background)
	{
		Image_LoadImage(&data->BackGround, str_background);
	}

	char* str_choose = ParseToAscii(data->CsvFile.Items[Row][CHOOSE_IMG_NAME]);
	if (NULL != *str_choose)
	{
		Image_LoadImage(&data->SelectButton, str_choose);
	}
	Image_LoadImage(&data->PointerButton, "Pointer.png");
	Image_LoadImage(&data->TextUI, "Text_UI.png");

	char* str_bgm = ParseToAscii(data->CsvFile.Items[Row][SOUND_MUSIC_NAME]);
	if (*s_PrevBGM != *str_bgm)
	{
		if (NULL != *str_bgm)
		{
			Audio_LoadMusic(&data->BGM, str_bgm);
			Audio_HookMusicFinished(logOnFinished);
		}
		// Audio_Play(&data->BGM, INFINITY_LOOP);
		
		Audio_Stop();
		Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 1000);
		s_PrevBGM = str_bgm;
	}

	char* str_se = ParseToAscii(data->CsvFile.Items[Row][SOUND_EFFECT_NAME]);
	if (NULL != *str_se)
	{
		Audio_LoadSoundEffect(&data->Effect, str_se);
		Audio_HookSoundEffectFinished(log2OnFinished);
	}

	if (s_PrevPage != s_CurrentPage)
	{
		Audio_StopSoundEffect();
		Audio_PlaySoundEffect(&data->Effect, 0);

		s_PrevPage = s_CurrentPage;
	}
	
	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->Back_X = 0;
	data->Back_Y = 0;
	data->Select_X = 0;
	data->Select_Y = 0;
	data->TextUI_X = 0;
	data->TextUI_Y = 0;
	data->Alpha = 255;
	data->Pointer_X = 0;

	data->SelectButtonQuantity = ParseToInt(data->CsvFile.Items[Row][NUMBER_OF_OPTIONS]);
	switch (data->SelectButtonQuantity)
	{
	case 1:
		data->Pointer_Y = CHOOSE_POSITION_BOTTOM;
		break;
	case 2:
		data->Pointer_Y = CHOOSE_POSITION_MIDDLE;
		break;
	case 3:
		data->Pointer_Y = CHOOSE_POSITION_TOP;
		break;
	}

	//FreeCsvFile(&data->CsvFile);

}

void update_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	// main(교수님꺼ㅏ~)

		/*----------------단순줄바꿈출력(한줄안댐)야발---------------------*/


	wchar_t* str_text = ParseToUnicode(data->CsvFile.Items[s_CurrentPage][FULL_TEXT]);
	const wchar_t* lineStart = str_text;
	while (!data->IsText)
	{
		for (int32 line = 0; line < TEXT_MAX_LINE; line++)
		{
			const wchar_t* lineEnd = lineStart;
			while (true)
			{
				if (L'\n' == *lineEnd || L'\0' == *lineEnd)
				{
					break;
				}
				++lineEnd;
			}
			int32 lineLength = lineEnd - lineStart;
			Text_CreateText(&data->TextLine[line], DEFAULT_FONT, DEFAULT_FONT_SIZE, lineStart, lineLength);

			if (L'\0' == *lineEnd)
			{
				data->IsText = !data->IsText;
				break;
			}
			lineStart = lineEnd + 1;
		}

	}



	/*------------------------------------------------------------*/

	if (Input_GetKeyDown('M'))
	{
		if (Audio_IsMusicPlaying())
		{
			Audio_Stop();
		}
		else
		{
			Audio_Play(&data->BGM, INFINITY_LOOP);
		}
	}

	if (Input_GetKeyDown('P'))
	{
		if (Audio_IsMusicPaused())
		{
			Audio_Resume();
		}
		else
		{
			Audio_Pause();
		}
	}

	if (Input_GetKey('1'))
	{
		data->Volume -= 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey('2'))
	{
		data->Volume += 0.01f;
		Audio_SetVolume(data->Volume);
	}

	// ���� �ʿ�
	if (Input_GetKeyDown(VK_DOWN) && CHOOSE_POSITION_TOP <= data->Pointer_Y && data->Pointer_Y < CHOOSE_POSITION_BOTTOM)

	{
		data->Pointer_Y += 80;
	}
	if (Input_GetKeyDown(VK_UP) && CHOOSE_POSITION_MIDDLE < data->Pointer_Y && data->SelectButtonQuantity == 2)
	{
		data->Pointer_Y -= 80;
	}
	if (Input_GetKeyDown(VK_UP) && CHOOSE_POSITION_TOP < data->Pointer_Y && data->SelectButtonQuantity == 3)
	{
		data->Pointer_Y -= 80;
	}

	int32 SelectNextPage = 1;								//��� ���� ��
	int32 Row = s_CurrentPage;
	// ������ ����
 	if (Input_GetKeyDown(VK_SPACE))
	{
		data->TextLength = 0;

		int32 num_choose_1;
		if (data->Pointer_Y == CHOOSE_POSITION_TOP)		//
		{
			num_choose_1 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_1]);
			SelectNextPage = num_choose_1;
		}
		else if (data->Pointer_Y == CHOOSE_POSITION_MIDDLE)	// ������ 2 ���� Scene
		{
			int32 num_choose_2;
			if (data->SelectButtonQuantity == 2)
			{
				num_choose_2 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_1]);
				SelectNextPage = num_choose_2;
			}
			else if (data->SelectButtonQuantity == 3)
			{
				num_choose_2 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_2]);
				SelectNextPage = num_choose_2;
			}

		}
		else if (data->Pointer_Y == CHOOSE_POSITION_BOTTOM)	// ������ 3 ���� Scene
		{
			int32 num_choose_3;
			if (data->SelectButtonQuantity == 1)
			{
				num_choose_3 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_1]);
				SelectNextPage = num_choose_3;
			}
			else if (data->SelectButtonQuantity == 2)
			{
				num_choose_3 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_2]);
				SelectNextPage = num_choose_3;
			}
			else if (data->SelectButtonQuantity == 3)
			{
				num_choose_3 = ParseToInt(data->CsvFile.Items[Row][NEXT_SCENE_3]);
				SelectNextPage = num_choose_3;
			}
		}
		s_CurrentPage = SelectNextPage;
		Scene_SetNextScene(SCENE_MAIN);
	}
}

void render_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	Renderer_DrawImage(&data->BackGround, data->Back_X, data->Back_Y);
	Renderer_DrawImage(&data->SelectButton, data->Select_X, data->Select_Y);
	Renderer_DrawImage(&data->PointerButton, data->Pointer_X, data->Pointer_Y);
	Renderer_DrawImage(&data->TextUI, data->TextUI_X, data->TextUI_Y);

	/*for (int32 i = 0; i < TEXT_MAX_LINE; i++)
	{
		Renderer_DrawTextBlended(&data->TextLine, 60, 80, color);
	}*/

	//렌더새끼~
		if (!data->DeltaRun)
		{
			data->TextDeltaTime += (Timer_GetDeltaTime() + 0.1f);
		}
	//data->TextDeltaTime += (Timer_GetDeltaTime());
	SDL_Color color = { .a = 255, .r = 255 , .g = 255 , .b = 255 };
	for (int32 i = 0; i < data->TextDeltaTime; i++)
	{
		Renderer_DrawTextBlended(&data->TextLine[i], 60, 80 + (i * 30), color);
		if (i > 18)
		{
			data->DeltaRun = !data->DeltaRun;
			data->TextDeltaTime = 0.0f;

		}
	}
	if (data->DeltaRun)
	{
		for (int32 j = 0; j < TEXT_MAX_LINE; j++)
		{
			Renderer_DrawTextBlended(&data->TextLine[j], 60, 80 + +(j * 30), color);
		}
	}

}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;
	Image_FreeImage(&data->BackGround);
	Image_FreeImage(&data->SelectButton);
	Image_FreeImage(&data->TextUI);

	Text_FreeText(&data->TextLine);
	
	// Audio_FreeMusic(&data->BGM);
	Audio_FreeSoundEffect(&data->Effect);

	SafeFree(g_Scene.Data);
}
#pragma endregion

#pragma region EndingScene

typedef struct EndingSceneData
{
	float   ActiveTime;
	float   ImageLoadTime;
	int32   RenderMode;

	Image   EndingBackGround;
	int32   EndingBackGround_Alpha;
	Image   EndingText1;
	int32   EndingText1_Alpha;
	Image   EndingText2;
	int32   EndingText2_Alpha;
	Image   BlackScreen;
	int32   BlackAlpha;

} EndingSceneData;


float   EndingTime = 0.0f;


void init_Ending(void)
{
	g_Scene.Data = malloc(sizeof(EndingSceneData));
	memset(g_Scene.Data, 0, sizeof(EndingSceneData));
	EndingSceneData* data = (EndingSceneData*)g_Scene.Data;
	Image_LoadImage(&data->BlackScreen, "BlackScreen.png");
}

int32 end1count = 1;
int32 end2count = 1;
int32 end3count = 1;
int32 end4count = 1;
int32 end5count = 1;
int32 end6count = 1;
int32 end7count = 1;

void update_Ending(void)
{
	EndingSceneData* data = (EndingSceneData*)g_Scene.Data;
	data->ActiveTime += Timer_GetDeltaTime();

	data->ImageLoadTime += Timer_GetDeltaTime();
	EndingTime += Timer_GetDeltaTime();
	data->BlackAlpha = Clamp(0, data->BlackAlpha + 255, 255);
	if (0 <= data->ImageLoadTime && data->ImageLoadTime < 6 && end1count == 1)
	{
		Image_LoadImage(&data->EndingBackGround, "END(1).png");
		Image_LoadImage(&data->EndingText1, "END(2).png");
		end1count = 0;
	}

	if (6 <= data->ImageLoadTime && data->ImageLoadTime < 12 && end2count == 1)
	{
		Image_LoadImage(&data->EndingBackGround, "END(3).png");
		Image_LoadImage(&data->EndingText1, "END(4).png");
		end2count = 0;
	}

	if (12 <= data->ImageLoadTime && data->ImageLoadTime < 18 && end3count == 1)
	{
		Image_LoadImage(&data->EndingBackGround, "END(5).png");
		Image_LoadImage(&data->EndingText1, "END(6).png");
		end3count = 0;
	}

	if (18 <= data->ImageLoadTime && data->ImageLoadTime < 24 && end4count == 1)
	{
		Image_LoadImage(&data->EndingBackGround, "END(7).png");
		Image_LoadImage(&data->EndingText1, "END(8).png");
		end4count = 0;
	}
	if (24 <= data->ImageLoadTime && data->ImageLoadTime < 35 && end5count == 1)
	{
		Image_LoadImage(&data->EndingBackGround, "END(9).png");
		Image_LoadImage(&data->EndingText1, "END(11).png");
		end5count = 0;
	}

	if (0.1 > data->ActiveTime)                           //각 요소 초기 알파값 0
	{
		data->EndingBackGround_Alpha = Clamp(0, data->EndingBackGround_Alpha - 255, 255);
		data->EndingText1_Alpha = Clamp(0, data->EndingText1_Alpha - 255, 255);
		data->EndingText2_Alpha = Clamp(0, data->EndingText2_Alpha - 255, 255);
	}

	if (1 <= EndingTime && EndingTime < 2)
	{
		data->EndingBackGround_Alpha = Clamp(0, data->EndingBackGround_Alpha + 10, 255);
	}
	if (2 <= EndingTime && EndingTime < 3)
	{
		data->EndingText1_Alpha = Clamp(0, data->EndingText1_Alpha + 10, 255);
		if (26 <= data->ActiveTime && 3 <= EndingTime)
		{
			data->EndingText2_Alpha = Clamp(0, data->EndingText2_Alpha + 10, 255);
		}
	}

	if (5 <= EndingTime && EndingTime < 6)
	{
		if (data->ActiveTime < 25)
		{
			data->EndingBackGround_Alpha = Clamp(0, data->EndingBackGround_Alpha - 10, 255);
			data->EndingText1_Alpha = Clamp(0, data->EndingText1_Alpha - 10, 255);
		}
		if (5.9 <= EndingTime)
		{
			EndingTime = 0.0f;
		}
	}
	if (35 <= data->ActiveTime)
	{
		data->EndingBackGround_Alpha = Clamp(0, data->EndingBackGround_Alpha - 10, 255);
		data->EndingText1_Alpha = Clamp(0, data->EndingText1_Alpha - 10, 255);
		data->EndingText2_Alpha = Clamp(0, data->EndingText2_Alpha - 10, 255);
		EndingTime = 0.0f;
	}
	Image_SetAlphaValue(&data->BlackScreen, data->BlackAlpha);
	Image_SetAlphaValue(&data->EndingBackGround, data->EndingBackGround_Alpha);
	Image_SetAlphaValue(&data->EndingText1, data->EndingText1_Alpha);
	Image_SetAlphaValue(&data->EndingText2, data->EndingText2_Alpha);

	if (Input_GetKeyDown(VK_SPACE) || 36 < data->ActiveTime)                           //36초 경과, 혹은 스페이스 누르면 메인 타이틀로 돌아감
	{
		Scene_SetNextScene(SCENE_TITLE);
	}
}

void render_Ending(void)
{
	EndingSceneData* data = (EndingSceneData*)g_Scene.Data;

	Renderer_DrawImage(&data->BlackScreen, 0, 0);
	Renderer_DrawImage(&data->EndingBackGround, 0, 0);
	Renderer_DrawImage(&data->EndingText1, 0, 0);
	Renderer_DrawImage(&data->EndingText2, 0, 0);
}

void release_Ending(void)
{
	EndingSceneData* data = (EndingSceneData*)g_Scene.Data;
	SafeFree(g_Scene.Data);
}

#pragma endregion

bool Scene_IsSetNextScene(void)
{
	if (SCENE_NULL == s_nextScene)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Scene_SetNextScene(ESceneType scene)			// 다음 Scene 지정 함수 : Scene의 이동, if문을 사용한 전환 가능
{
	assert(s_nextScene == SCENE_NULL);
	assert(SCENE_NULL < scene&& scene < SCENE_MAX);

	s_nextScene = scene;
}

void Scene_Change(void)								// Scene 변경 함수 - Scene 추가시 switch에 Scene의 함수들 등록 필요.
{
	assert(s_nextScene != SCENE_NULL);

	if (g_Scene.Release)
	{
		g_Scene.Release();
	}

	switch (s_nextScene)
	{
	case SCENE_TITLE:
		g_Scene.Init = init_title;
		g_Scene.Update = update_title;
		g_Scene.Render = render_title;
		g_Scene.Release = release_title;
		break;
	case SCENE_MAIN:
		g_Scene.Init = init_main;
		g_Scene.Update = update_main;
		g_Scene.Render = render_main;
		g_Scene.Release = release_main;
		break;
	case SCENE_ENDING:
		g_Scene.Init = init_Ending;
		g_Scene.Update = update_Ending;
		g_Scene.Render = render_Ending;
		g_Scene.Release = release_Ending;
		break;
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}