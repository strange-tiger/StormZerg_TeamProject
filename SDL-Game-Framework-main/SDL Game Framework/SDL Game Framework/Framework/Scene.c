#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"
#include "csv.h"
#include "TextList.h"
#include <crtdbg.h>
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

const wchar_t* str2[] = {
	L"2060년, 환경오염과 기후위기로 ",
	L"최악의 상황에 몰린 지구는",
	L"하루 하루 다르게 메말라 간다.",
	L"",
	L"소수의 특권층들은",
	L"안전한 삶을 영위하고 있지만,",
	L"과학의 발전으로 인해",
	L"대부분의 노동자들은",
	L"안드로이드에게 일자리를 뺏기고 ",
	L"힘들게 살아 가고 있다.",
	L"",

};

#define TEXTLINE_COUNT 20
#define CHOOSE_POSITION_TOP -160
#define CHOOSE_POSITION_MIDDLE -80
#define CHOOSE_POSITION_BOTTOM 0

typedef struct MainSceneData
{
	CsvFile		CsvFile;

	Text		TextLine[TEXTLINE_COUNT];
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

static int32 SelectButtonQuantity = 3;									//������ ��ư ��	// Null ���� ������ �� ���� ����� ����
static int32 s_CurrentPage = 1;
static int32 s_SelectNextPage = 1;								//��� ���� ��
void init_main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	CreateCsvFile(&data->CsvFile, "222.csv");

	/*wchar_t* str_text = ParseToUnicode(data->CsvFile.Items[s_CurrentPage][FULL_TEXT]);
	if (NULL != *str_text)
	{
		Text_CreateText(&data->TextLine[0], "d2coding.ttf", 16, str_text, wcslen(str_text));
	}*/

	// obt용 임시 조치
	int j = s_CurrentPage - 1;
	if (j == 260)
	{
		j = 31;
	}
	for (int i = 0; i < TEXTLINE_COUNT; i++)
	{
		if (NULL != strList[j][i])
		{
			Text_CreateText(&data->TextLine[i], "NotoSansKR-Bold.otf", 20, strList[j][i], wcslen(strList[j][i]));
		}
	}
	// obt용 임시 조치
	if (s_CurrentPage == 261)
	{
		s_CurrentPage = 68;
	}

	char* str_background = ParseToAscii(data->CsvFile.Items[s_CurrentPage][BACK_IMG_NAME]);
	if (NULL != *str_background)
	{
		Image_LoadImage(&data->BackGround, str_background);
	}

	char* str_choose = ParseToAscii(data->CsvFile.Items[s_CurrentPage][CHOOSE_IMG_NAME]);
	if (NULL != *str_choose)
	{
		Image_LoadImage(&data->SelectButton, str_choose);
	}
	Image_LoadImage(&data->PointerButton, "Pointer.png");

	char* str_bgm = ParseToAscii(data->CsvFile.Items[s_CurrentPage][SOUND_MUSIC_NAME]);
	if (NULL != *str_bgm)
	{
		Audio_LoadMusic(&data->BGM, str_bgm);
		Audio_HookMusicFinished(logOnFinished);
	}

	char* str_se = ParseToAscii(data->CsvFile.Items[s_CurrentPage][SOUND_EFFECT_NAME]);
	if (NULL != *str_se)
	{
		Audio_LoadSoundEffect(&data->Effect, str_se);
		Audio_HookSoundEffectFinished(log2OnFinished);
		Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);
	}

	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->Back_X = 0;
	data->Back_Y = 0;
	data->Select_X = 0;
	data->Select_Y = 0;

	char* num_choose_quantity = ParseToAscii(data->CsvFile.Items[s_CurrentPage][CHOOSE_QUANTITY]);
	SelectButtonQuantity = atoi(num_choose_quantity);
	switch (SelectButtonQuantity)
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

	data->Alpha = 255;
}

void update_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('E'))
	{
		Audio_PlaySoundEffect(&data->Effect, 1);
	}

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

	if (Input_GetKey('-'))
	{
		data->Volume -= 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey('+'))
	{
		data->Volume += 0.01f;
		Audio_SetVolume(data->Volume);
	}

	// ���� �ʿ�
	if (Input_GetKeyDown(VK_DOWN) && CHOOSE_POSITION_TOP <= data->Pointer_Y && data->Pointer_Y < CHOOSE_POSITION_BOTTOM)

	{
		data->Pointer_Y += 80;
	}
	if (Input_GetKeyDown(VK_UP) && CHOOSE_POSITION_MIDDLE < data->Pointer_Y && SelectButtonQuantity == 2)
	{
		data->Pointer_Y -= 80;
	}
	if (Input_GetKeyDown(VK_UP) && CHOOSE_POSITION_TOP < data->Pointer_Y && SelectButtonQuantity == 3)
	{
		data->Pointer_Y -= 80;
	}

	// ������ ����
	if (Input_GetKeyDown(VK_SPACE))
	{
		if (data->Pointer_Y == CHOOSE_POSITION_TOP)		//
		{
			char* num_choose_1 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_1]);
			s_SelectNextPage = atoi(num_choose_1);
		}
		else if (data->Pointer_Y == CHOOSE_POSITION_MIDDLE)	// ������ 2 ���� Scene
		{
			char* num_choose_2;
			if (SelectButtonQuantity == 2)
			{
				num_choose_2 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_1]);
				s_SelectNextPage = atoi(num_choose_2);
			}
			else if (SelectButtonQuantity == 3)
			{
				num_choose_2 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_2]);
				s_SelectNextPage = atoi(num_choose_2);
			}

		}
		else if (data->Pointer_Y == CHOOSE_POSITION_BOTTOM)	// ������ 3 ���� Scene
		{
			char* num_choose_3;
			if (SelectButtonQuantity == 1)
			{
				num_choose_3 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_1]);
				s_SelectNextPage = atoi(num_choose_3);
			}
			else if (SelectButtonQuantity == 2)
			{
				num_choose_3 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_2]);
				s_SelectNextPage = atoi(num_choose_3);
			}
			else if (SelectButtonQuantity == 3)
			{
				num_choose_3 = ParseToAscii(data->CsvFile.Items[s_CurrentPage][NEXT_SCENE_3]);
				s_SelectNextPage = atoi(num_choose_3);
			}
		}
		s_CurrentPage = s_SelectNextPage;
		Scene_SetNextScene(SCENE_MAIN);
	}

	if (Input_GetKey('W'))
	{
		data->BackGround.ScaleY -= 0.05f;
	}

	if (Input_GetKey('S'))
	{
		data->BackGround.ScaleY += 0.05f;
	}

	if (Input_GetKey('K'))
	{
		//data->Alpha = Clamp(0, data->Alpha - 1, 255);						//이거 존나중요함 페이드 인아웃.
		//Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}

	if (Input_GetKey('L'))
	{
		//data->Alpha = Clamp(0, data->Alpha + 1, 255);
		//Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}
}

void render_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	Renderer_DrawImage(&data->BackGround, data->Back_X, data->Back_Y);
	Renderer_DrawImage(&data->SelectButton, data->Select_X, data->Select_Y);
	Renderer_DrawImage(&data->PointerButton, data->Pointer_X, data->Pointer_Y);

	SDL_Color color = { .a = 255, .r = 255 , .g = 255 , .b = 255 };

	// Renderer_DrawTextSolid(&data->TextLine[0], 50, 140, color);

	// obt용 임시 조치
	for (int i = 0; i < TEXTLINE_COUNT; i++)
	{
		Renderer_DrawTextSolid(&data->TextLine[i], 30, 120 + 30 * i, color);
	}
}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < TEXTLINE_COUNT; ++i)
	{
		Text_FreeText(&data->TextLine);
	}
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