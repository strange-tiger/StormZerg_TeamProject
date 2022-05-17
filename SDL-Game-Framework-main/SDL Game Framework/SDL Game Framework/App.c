#include "stdafx.h"
#include "App.h"

#include "Framework.h"
#include "Framework/Scene.h"
#include "Framework/Window.h"

App g_App;

bool App_Init(void)
{
	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))					// SDL - 심플 다이렉트미디어 레이어, 비디오 및 오디오의 초기화 및 예외 처리
	{
		LogWithErrorCode("Fail to initialize SDL", SDL_GetError());

		return false;
	}

	if (NULL == Window_Init())											// Window 초기화 및 예외 처리
	{																	// Window - SDL_CreateWinodw 등 윈도우의 생성 및 정리
		LogWithErrorCode("Fail to intialize window", SDL_GetError());

		return false;
	}

	if (NULL == Renderer_Init())										// Renderer 초기화 및 예외 처리
	{																	// Renderer - 출력 처리
		LogWithErrorCode("Fail to initialize renderer", SDL_GetError());

		return false;
	}

	if (false == Image_Init())											// Image 초기화 및 예외 처리
	{																	// Image - 이미지 파일의 로드 및 속성 조정
		LogWithErrorCode("Fail to initalize image library", IMG_GetError());

		return false;
	}

	Random_Init();														// Random 초기화

	if (false == Audio_Init())											// Audio 초기화 및 예외 처리
	{																	// Audio - 오디오 파일 로드 및 출력 처리 (볼륨 조절, 효과, 효과음 설정 등)
		LogWithErrorCode("Fail to intialize audio library", Mix_GetError());

		return false;
	}

	if (false == Text_Init())											// Text 초기화 및 예외 처리
	{																	// Text - 유니코드 문자열의 출력 및 저장
		LogWithErrorCode("Fail to initialize font library", TTF_GetError());

		return false;
	}

	return true;
}

void cleanup(void)														// 라이브러리의 정리
{
	Text_Cleanup();
	Audio_Cleanup();
	Image_Cleanup();
	Renderer_Cleanup();
	Window_Cleanup();
	SDL_Quit();
}

void processInput(void)													// Input 업데이트 (정보 갱신)
{
	Input_Update();
}

void update(void)														// 씬 업데이트 (정보 갱신)
{
	g_Scene.Update();
}

void render(void)														// 출력 처리 - 씬 렌더, 더블 버퍼링
{
	g_Scene.Render();
	Renderer_Flip();
}

int32 App_Run(void)														// 앱 실행부
{
	atexit(cleanup);

	Timer_Init(FPS);

	Scene_SetNextScene(SCENE_TITLE);

	SDL_Event event;
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}
		else
		{
			if (Scene_IsSetNextScene())
			{
				Scene_Change();
			}

			if (Timer_Update())
			{
				processInput();
				update();
				render();
			}
		}
	}


	return 0;
}