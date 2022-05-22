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
void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	CreateCsvFile(&data->CsvFile, "DB_project.CSV");

	data->IsText = false;

	// obt용 임시 조치
	switch(s_CurrentPage)
	{
	case 261:
		// s_CurrentPage = 68;
		break;
	case 391:
		// s_CurrentPage = 69;
		break;
	case 411:
		// s_CurrentPage = 70;
		break;
	case 451:
		// s_CurrentPage = 71;
		break;
	}

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
	if (NULL != *str_bgm)
	{
		Audio_LoadMusic(&data->BGM, str_bgm);
		Audio_HookMusicFinished(logOnFinished);
	}

	char* str_se = ParseToAscii(data->CsvFile.Items[Row][SOUND_EFFECT_NAME]);
	if (NULL != *str_se)
	{
		// Audio_LoadSoundEffect(&data->Effect, str_se);
		Audio_HookSoundEffectFinished(log2OnFinished);
	}
	// Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);
	// Audio_PlaySoundEffect(&data->Effect, 0);
	
	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->Back_X = 0;
	data->Back_Y = 0;
	data->Select_X = 0;
	data->Select_Y = 0;
	data->TextUI_X = 0;
	data->TextUI_Y = 0;
	data->Alpha = 255;

	data->SelectButtonQuantity = ParseToInt(data->CsvFile.Items[Row][NUMBER_OF_OPTIONS]);
	switch (data->SelectButtonQuantity)
	{
	case 1:
		data->Pointer_X = 0;
		data->Pointer_Y = CHOOSE_POSITION_BOTTOM;
		break;
	case 2:
		data->Pointer_X = 0;
		data->Pointer_Y = CHOOSE_POSITION_MIDDLE;
		break;
	case 3:
		data->Pointer_X = 0;
		data->Pointer_Y = CHOOSE_POSITION_TOP;
		break;
	}

	//FreeCsvFile(&data->CsvFile);

}

void update_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	char* str_text = ParseToUnicode(data->CsvFile.Items[s_CurrentPage][FULL_TEXT]);
	TextTyping(&data->TextLine, str_text, &data->IsText, &data->TextLength, DEFAULT_FONT, DEFAULT_FONT_SIZE);

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

	SDL_Color color = { .a = 255, .r = 255 , .g = 255 , .b = 255 };

	for (int32 i = 0; i < TEXT_MAX_LINE; i++)
	{
		Renderer_DrawTextBlended(&data->TextLine, 60, 80, color);
	}

}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	Text_FreeText(&data->TextLine);
	
	Audio_FreeMusic(&data->BGM);
	Audio_FreeSoundEffect(&data->Effect);

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
		/*case SCENE_TEST:
			g_Scene.Init = init_test;
			g_Scene.Update = update_test;
			g_Scene.Render = render_test;
			g_Scene.Release = release_test;
			break;*/
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}