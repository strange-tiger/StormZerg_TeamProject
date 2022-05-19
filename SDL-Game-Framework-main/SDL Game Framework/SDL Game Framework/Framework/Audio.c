#include "stdafx.h"
#include "Audio.h"

#define ALL_CHANNEL -1

static float s_MusicVolume = 0.0f;
static char s_path[MAX_PATH];

bool Audio_Init(void)								// Audio �ʱ�ȭ
{
	int flags = MIX_INIT_FLAC | MIX_INIT_MP3;		// ��� ����� Ȯ������ ����
	int initResult = Mix_Init(flags);				// �ͽ� ���̺귯���� �ʱ�ȭ		
	if (flags != (flags & initResult))				// ���� ó��
	{
		return false;
	}
	// ����� ���� �� ���� ó��
	if (0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096))
	{
		return false;
	}

	Audio_SetVolume(0.5f);							// �ʱ� ���� ����

	return true;
}

void Audio_Cleanup(void)
{
	Mix_CloseAudio();								// ���� �ݱ�
	Mix_Quit();
}

void Audio_SetVolume(float volume)					// ���� ���� ����
{
	s_MusicVolume = Clamp(0.0f, volume, 1.0f);		// Clamp - value�� [minValue, maxValue]�� �����.
	
	int32 realVolume = 0 + s_MusicVolume * MIX_MAX_VOLUME;
	Mix_VolumeMusic(realVolume);
}

float Audio_GetVolume(void)							// Audio_SetVolume���� ������ ������ �޴´�.
{
	return s_MusicVolume;
}

void Audio_Pause(void)								// ���� ��� ����.
{
	Mix_PauseMusic();
}

void Audio_Resume(void)								// ���� ��� �簳.
{
	Mix_ResumeMusic();
}

void Audio_Rewind(void)								// ���� �ǰ���.
{
	Mix_RewindMusic();
}

void Audio_Play(const Music* music, int32 loops)	// ���� ���
{
	Mix_PlayMusic(music->Music, loops);
}

void Audio_PlayFadeIn(const Music* music, int32 loops, int32 milliseconds)	// ���� ȿ�� - ���� ������ ���
{
	Mix_FadeInMusic(music->Music, loops, milliseconds);
}

void Audio_Stop(void)								// ���� ����
{
	Mix_HaltMusic();
}

void Audio_FadeOut(int32 milliseconds)				// ���� ȿ�� - ���� ������ ����
{
	Mix_FadeOutMusic(milliseconds);
}

void Audio_HookMusicFinished(void(*callback)(void))	// ���� ����� ���� �� �ݹ��� �߰��Ѵ�. - �Ͻ�����, �簳 �� �ݹ����.
{
	Mix_HookMusicFinished(callback);
}

bool Audio_IsMusicPlaying(void)						// ������ ���� ��� ������ �Ǻ�.
{
	if (0 == Mix_PlayingMusic())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Audio_IsMusicPaused(void)						// ������ ���� ������� �Ǻ�.
{
	if (0 == Mix_PausedMusic())
	{
		return false;
	}
	else
	{
		return true;
	}
}

EMusicFading Audio_IsMusicFading(void)				// ������ ȿ�� - ���̵� ������ �Ǻ�.
{
	Mix_Fading result = Mix_FadingMusic();

	return result;
}

void Audio_LoadMusic(Music* music, const char* filename)	// ������ �ε�.
{
	LogInfo("Sound Loading... %s", filename);

	sprintf_s(s_path, sizeof(s_path), "%s/%s", SOUND_DIRECTORY, filename);
	music->Music = Mix_LoadMUS(s_path);
}

void Audio_FreeMusic(Music* music)							// ���� ��ü ���� �ڿ��� ����. �ݵ�� Release()�� ���.
{
	Mix_FreeMusic(music->Music);
	music->Music = NULL;
}

void Audio_LoadSoundEffect(SoundEffect* soundEffect, const char* filename)	// SE�� �ε�.
{
	LogInfo("Sound Effect Loading... %s", filename);

	sprintf_s(s_path, sizeof(s_path), "%s/%s", SOUND_DIRECTORY, filename);
	soundEffect->Chunk = Mix_LoadWAV(s_path);
	Audio_SetEffectVolume(soundEffect, 1.0f);
}

void Audio_FreeSoundEffect(SoundEffect* soundEffect)		// SE ��ü ���� �ڿ��� ����. �ݵ�� Release()�� ���.
{
	Mix_FreeChunk(soundEffect->Chunk);
	soundEffect->Chunk = NULL;
}

void Audio_SetEffectVolume(SoundEffect* soundEffect, float volume)
{
	soundEffect->Volume = Clamp(0.0f, volume, 1.0f);

	int32 realVolume = 0 + soundEffect->Volume * MIX_MAX_VOLUME;
	Mix_VolumeChunk(soundEffect->Chunk, realVolume);
}

void Audio_PlaySoundEffect(const SoundEffect* soundEffect, int32 loops)	// SE ���
{
	Mix_PlayChannel(-1, soundEffect->Chunk, loops);
}

void Audio_PlaySoundEffectFadeIn(SoundEffect* soundEffect, int32 loops, int32 milliseconds)	// SE ȿ�� - ������ ���
{
	Mix_FadeInChannel(-1, soundEffect->Chunk, loops, milliseconds);
}

void Audio_PauseSoundEffect(void)										// SE ��� ����
{
	Mix_Pause(-1);
}

void Audio_ResumeSoundEffect(void)										// SE ��� �簳
{
	Mix_Resume(-1);
}

void Audio_StopSoundEffect(void)										// SE ����
{
	Mix_HaltChannel(-1);
}

void Audio_FadeOutSoundEffect(int32 milliseconds)						// SE ȿ�� - ������ ����
{
	Mix_FadeOutChannel(-1, milliseconds);
}

void Audio_HookSoundEffectFinished(void(*callback)(int32))				// SE ����� ���� �� �ݹ��� �߰��Ѵ�. - �Ͻ�����, �簳 �� �ݹ����.
{
	Mix_ChannelFinished(callback);
}

bool Audio_IsSoundEffectPlaying(void)									// SE�� ���� ��� ������ �Ǻ�.
{
	if (0 == Mix_Playing(-1))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Audio_IsSoundEffectPaused(void)									// SE�� ���� ������� �Ǻ�.
{
	if (0 == Mix_Paused(-1))
	{
		return false;
	}
	else
	{
		return true;
	}
}


// ���� ���
// 
// ���� �ε�			Load
// ���� ���� ����		SetVolume
// ���� ���� �ޱ�		GetVolume
// ���� ���			Play
// ���� �ǰ���		Rewind
// ���� ����			Stop
// ���� �Ͻ� ����		Paused
// ���� �ݹ� ���		Hook
// ���� ��� �簳		Resume
// ���� ���̵� ��		PlayFadeIn
// ���� ���̵� �ƿ�	FadeOut
// ���� ����� �Ǻ�	IsPlaying
// ���� ������ �Ǻ�	IsPaused 
// ���� �ڿ� ����		FreeMusic
// 
// SE �ε�			LoadSoundEffect
// SE ���� ����		SetEffectVolume
// SE ���� �ޱ�		GetEffectVolume
// SE ���			PlaySoundEffect
// SE ����			StopSoundEffect
// SE �Ͻ� ����		PausedSoundEffect
// SE �ݹ� ���		HookSoundEffect
// SE ��� �簳		ResumeSoundEffect
// SE ���̵� ��		PlayFadeInSoundEffect
// SE ���̵� �ƿ�	FadeOutSoundEffect
// SE ����� �Ǻ�	IsPlayingSoundEffect
// SE ������ �Ǻ�	IsPausedSoundEffect
// SE �ڿ� ����		FreeSoundEffect
// 