#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"

Scene g_Scene;
bool BackGroundColor = 0;
bool BackGroundFadeoutCount = 0;

static ESceneType s_nextScene = SCENE_NULL;
 
#pragma region TitleScene

#define SOLID 0
#define SHADED 1
#define BLENDED 2

const wchar_t* str[] = {
	L"¿©±â´Â Å¸ÀÌÆ²¾ÀÀÔ´Ï´Ù. ÅØ½ºÆ®¿Í °ü·ÃµÈ ¿©·¯°¡Áö¸¦ Å×½ºÆ®ÇØº¾½Ã´Ù.",
	L"BÅ°¸¦ ´©¸£¸é ÆùÆ®°¡ ±½°Ô º¯ÇÕ´Ï´Ù.",
	L"IÅ°¸¦ ´©¸£¸é ÆùÆ®°¡ ÀÌÅÅ¸¯Ã¼·Î º¯ÇÕ´Ï´Ù.",
	L"UÅ°¸¦ ´©¸£¸é ÅØ½ºÆ®¿¡ ¹ØÁÙÀÌ »ý±é´Ï´Ù.",
	L"SÅ°¸¦ ´©¸£¸é ÅØ½ºÆ®¿¡ Ãë¼Ò¼±ÀÌ »ý±é´Ï´Ù.",
	L"NÅ°¸¦ ´©¸£¸é ´Ù½Ã ¿ø·¡´ë·Î µ¹¾Æ¿É´Ï´Ù.",
	L"CÅ°¸¦ ´©¸£¸é ·»´õ ¸ðµå°¡ ¹Ù²ò´Ï´Ù. (Solid -> Shaded -> Blended)",
	L"1Å°¸¦ ´©¸£¸é ÅØ½ºÆ®°¡ ÀÛ¾ÆÁý´Ï´Ù.",
	L"2Å°¸¦ ´©¸£¸é ÅØ½ºÆ®°¡ Ä¿Áý´Ï´Ù.",
	L"½ºÆäÀÌ½º Å°¸¦ ´©¸£¸é ´ÙÀ½ ¾ÀÀ¸·Î ³Ñ¾î°©´Ï´Ù."
};

typedef struct TitleSceneData
{
	Text	GuideLine[10];
	Text	TestText;
	int32	FontSize;
	int32	RenderMode;
	Image	TestImage;
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
	Text_CreateText(&data->TestText, "d2coding.ttf", data->FontSize, L"ÀÌ ÅØ½ºÆ®°¡ º¯ÇÕ´Ï´Ù.", 13);

	data->RenderMode = SOLID;

	Image_LoadImage(&data->TestImage, "Background.jfif");
}

void update_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('B'))
	{
		Text_SetFontStyle(&data->TestText, FS_BOLD);
	}

	if (Input_GetKeyDown('I'))
	{
		Text_SetFontStyle(&data->TestText, FS_ITALIC);
	}

	if (Input_GetKeyDown('U'))
	{
		Text_SetFontStyle(&data->TestText, FS_UNDERLINE);
	}

	if (Input_GetKeyDown('S'))
	{
		Text_SetFontStyle(&data->TestText, FS_STRIKETHROUGH);
	}

	if (Input_GetKeyDown('N'))
	{
		Text_SetFontStyle(&data->TestText, FS_NORMAL);
	}

	if (Input_GetKeyDown('C'))
	{
		data->RenderMode = (data->RenderMode + 1) % 3;
	}

	if (Input_GetKey('1'))
	{
		--data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
	}

	if (Input_GetKey('2'))
	{
		++data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
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

int32 SelectButtonQuantity = 1;									//¼±ÅÃÁö ¹öÆ° ¼ö
int32 SelectNextScene;											//´ÙÀ½ ¼±ÅÃ ¾À

const wchar_t* str2[] = {
	L"2060³â, È¯°æ¿À¿°°ú ±âÈÄÀ§±â·Î ",
	L"ÃÖ¾ÇÀÇ »óÈ²¿¡ ¸ô¸° Áö±¸´Â",
	L"ÇÏ·ç ÇÏ·ç ´Ù¸£°Ô ¸Þ¸»¶ó °£´Ù.",
	L"",
	L"¼Ò¼öÀÇ Æ¯±ÇÃþµéÀº",
	L"¾ÈÀüÇÑ »îÀ» ¿µÀ§ÇÏ°í ÀÖÁö¸¸,",
	L"°úÇÐÀÇ ¹ßÀüÀ¸·Î ÀÎÇØ",
	L"´ëºÎºÐÀÇ ³ëµ¿ÀÚµéÀº",
	L"¾Èµå·ÎÀÌµå¿¡°Ô ÀÏÀÚ¸®¸¦ »¯±â°í",
	L"Èûµé°Ô »ì¾Æ °¡°í ÀÖ´Ù.",
	L"",
	L"",
	L"",
	L"",
<<<<<<< Updated upstream
=======
	L"ì†Œìˆ˜ì˜ íŠ¹ê¶Œì¸µë“¤ì€",
	L"ì•ˆì „í•œ ì‚¶ì„ ì˜ìœ„í•˜ê³  ìžˆì§€ë§Œ,",
	L"ê³¼í•™ì˜ ë°œì „ìœ¼ë¡œ ì¸í•´",
	L"ëŒ€ë¶€ë¶„ì˜ ë…¸ë™ìžë“¤ì€",
	L"ì•ˆë“œë¡œì´ë“œì—ê²Œ ì¼ìžë¦¬ë¥¼ ëºê¸°ê³  ",
	L"íž˜ë“¤ê²Œ ì‚´ì•„ ê°€ê³  ìžˆë‹¤.",
>>>>>>> Stashed changes
	L"",
};

#define GUIDELINE_COUNT 15

typedef struct MainSceneData
{
	Text		GuideLine[GUIDELINE_COUNT];
	Music		BGM;
	float		Volume;
	SoundEffect Effect;
	float		Speed;

	Image		BlackBackGround;
	int32		BlackBack_X;
	int32		BlackBack_Y;

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

void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

<<<<<<< Updated upstream
	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 30, str2[i], wcslen(str2[i]));
	}
	
	Image_LoadImage(&data->BackGround, "1_Image.png");
	Image_LoadImage(&data->SelectButton, "1_Select.png");
=======
	CreateCsvFile(&data->CsvFile, "DB_projectver1.csv");

	wchar_t* str_text = ParseToUnicode(data->CsvFile.Items[s_CurrentPage][9]);
	Text_CreateText(&data->TextLine, "d2coding.ttf", 16, str_text, wcslen(str_text));
	Image_LoadImage(&data->BlackBackGround, "BlackBackGround.png");
	//data->Alpha = Clamp(0, data->Alpha + 20, 255);
	//Image_SetAlphaValue(&data->BlackBackGround, data->Alpha);
	char* str_background = ParseToAscii(data->CsvFile.Items[s_CurrentPage][1]);
	Image_LoadImage(&data->BackGround, str_background);
	char* str_choose = ParseToAscii(data->CsvFile.Items[s_CurrentPage][2]);
	Image_LoadImage(&data->SelectButton, str_choose);
>>>>>>> Stashed changes
	Image_LoadImage(&data->PointerButton, "Pointer.png");

	Audio_LoadMusic(&data->BGM, "powerful.mp3");
	Audio_HookMusicFinished(logOnFinished);
	Audio_LoadSoundEffect(&data->Effect, "effect2.wav");
	Audio_HookSoundEffectFinished(log2OnFinished);
	Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);

	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->BlackBack_X = 0;
	data->BlackBack_Y = 0;
	data->Back_X = 0;
	data->Back_Y = 0;
	data->Select_X = 0;
	data->Select_Y = 0;
	switch (SelectButtonQuantity)
	{
		case 1:
			data->Pointer_X = 0;
			data->Pointer_Y = 0;
			break;
		case 2:
			data->Pointer_X = 0;
			data->Pointer_Y = -80;
			break;
		case 3:
			data->Pointer_X = 0;
			data->Pointer_Y = -160;
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
	if (Input_GetKeyDown(VK_DOWN) && -160 <= data->Pointer_Y && data->Pointer_Y < 0)
	{
		data->Pointer_Y += 80;
	}
	if (Input_GetKeyDown(VK_UP) && data->Pointer_Y > -80 && SelectButtonQuantity == 2)
	{
		data->Pointer_Y -= 80;
	}
	if (Input_GetKeyDown(VK_UP) && data->Pointer_Y > -160 && SelectButtonQuantity == 3)
	{
		data->Pointer_Y -= 80;
	}
	if (Input_GetKeyDown(VK_SPACE))
	{
		if (data->Pointer_Y == -160)
		{
			SelectNextScene = 1;
		}
		if (data->Pointer_Y == -80)
		{
			SelectNextScene = 2;
		}
		if (data->Pointer_Y == 0)
		{
			SelectNextScene = 3;
		}
		BackGroundFadeoutCount = 1;
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
		//data->Alpha = Clamp(0, data->Alpha - 1, 255);						//ÀÌ°Å Á¸³ªÁß¿äÇÔ ÆäÀÌµå ÀÎ¾Æ¿ô.
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
	//data->Alpha = Clamp(0, data->Alpha - 20, 255);
	//Image_SetAlphaValue(&data->BackGround, data->Alpha);

	Renderer_DrawImage(&data->BlackBackGround, data->Back_X, data->Back_Y);
	Renderer_DrawImage(&data->BackGround, data->Back_X, data->Back_Y);
	Renderer_DrawImage(&data->SelectButton, data->Select_X, data->Select_Y);
	Renderer_DrawImage(&data->PointerButton, data->Pointer_X, data->Pointer_Y);

	for (int32 i = 0; i < GUIDELINE_COUNT; i++)
	{
		SDL_Color color = { .a = 255, .r =255 , .g = 255 , .b = 255 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 50, 100 + (i * 40), color);
	}
}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 10; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
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

void Scene_SetNextScene(ESceneType scene)
{
	assert(s_nextScene == SCENE_NULL);
	assert(SCENE_NULL < scene&& scene < SCENE_MAX);

	s_nextScene = scene;
}

void Scene_Change(void)
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
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}