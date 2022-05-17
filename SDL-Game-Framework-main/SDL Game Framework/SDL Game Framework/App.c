#include "stdafx.h"
#include "App.h"

#include "Framework.h"
#include "Framework/Scene.h"
#include "Framework/Window.h"

App g_App;

bool App_Init(void)
{
	if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))					// SDL - ���� ���̷�Ʈ�̵�� ���̾�, ���� �� ������� �ʱ�ȭ �� ���� ó��
	{
		LogWithErrorCode("Fail to initialize SDL", SDL_GetError());

		return false;
	}

	if (NULL == Window_Init())											// Window �ʱ�ȭ �� ���� ó��
	{																	// Window - SDL_CreateWinodw �� �������� ���� �� ����
		LogWithErrorCode("Fail to intialize window", SDL_GetError());

		return false;
	}

	if (NULL == Renderer_Init())										// Renderer �ʱ�ȭ �� ���� ó��
	{																	// Renderer - ��� ó��
		LogWithErrorCode("Fail to initialize renderer", SDL_GetError());

		return false;
	}

	if (false == Image_Init())											// Image �ʱ�ȭ �� ���� ó��
	{																	// Image - �̹��� ������ �ε� �� �Ӽ� ����
		LogWithErrorCode("Fail to initalize image library", IMG_GetError());

		return false;
	}

	Random_Init();														// Random �ʱ�ȭ

	if (false == Audio_Init())											// Audio �ʱ�ȭ �� ���� ó��
	{																	// Audio - ����� ���� �ε� �� ��� ó�� (���� ����, ȿ��, ȿ���� ���� ��)
		LogWithErrorCode("Fail to intialize audio library", Mix_GetError());

		return false;
	}

	if (false == Text_Init())											// Text �ʱ�ȭ �� ���� ó��
	{																	// Text - �����ڵ� ���ڿ��� ��� �� ����
		LogWithErrorCode("Fail to initialize font library", TTF_GetError());

		return false;
	}

	return true;
}

void cleanup(void)														// ���̺귯���� ����
{
	Text_Cleanup();
	Audio_Cleanup();
	Image_Cleanup();
	Renderer_Cleanup();
	Window_Cleanup();
	SDL_Quit();
}

void processInput(void)													// Input ������Ʈ (���� ����)
{
	Input_Update();
}

void update(void)														// �� ������Ʈ (���� ����)
{
	g_Scene.Update();
}

void render(void)														// ��� ó�� - �� ����, ���� ���۸�
{
	g_Scene.Render();
	Renderer_Flip();
}

int32 App_Run(void)														// �� �����
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