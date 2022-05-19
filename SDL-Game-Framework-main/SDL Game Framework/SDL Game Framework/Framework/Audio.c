#include "stdafx.h"
#include "Audio.h"

#define ALL_CHANNEL -1

static float s_MusicVolume = 0.0f;
static char s_path[MAX_PATH];

bool Audio_Init(void)								// Audio 초기화
{
	int flags = MIX_INIT_FLAC | MIX_INIT_MP3;		// 사용 오디오 확장자의 결정
	int initResult = Mix_Init(flags);				// 믹스 라이브러리의 초기화		
	if (flags != (flags & initResult))				// 예외 처리
	{
		return false;
	}
	// 오디오 열기 및 예외 처리
	if (0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096))
	{
		return false;
	}

	Audio_SetVolume(0.5f);							// 초기 볼륨 세팅

	return true;
}

void Audio_Cleanup(void)
{
	Mix_CloseAudio();								// 음악 닫기
	Mix_Quit();
}

void Audio_SetVolume(float volume)					// 음악 볼륨 조절
{
	s_MusicVolume = Clamp(0.0f, volume, 1.0f);		// Clamp - value를 [minValue, maxValue]로 만든다.
	
	int32 realVolume = 0 + s_MusicVolume * MIX_MAX_VOLUME;
	Mix_VolumeMusic(realVolume);
}

float Audio_GetVolume(void)							// Audio_SetVolume에서 결정한 볼륨을 받는다.
{
	return s_MusicVolume;
}

void Audio_Pause(void)								// 음악 재생 정지.
{
	Mix_PauseMusic();
}

void Audio_Resume(void)								// 음악 재생 재개.
{
	Mix_ResumeMusic();
}

void Audio_Rewind(void)								// 음악 되감기.
{
	Mix_RewindMusic();
}

void Audio_Play(const Music* music, int32 loops)	// 음악 재생
{
	Mix_PlayMusic(music->Music, loops);
}

void Audio_PlayFadeIn(const Music* music, int32 loops, int32 milliseconds)	// 음악 효과 - 음악 서서히 재생
{
	Mix_FadeInMusic(music->Music, loops, milliseconds);
}

void Audio_Stop(void)								// 음악 정지
{
	Mix_HaltMusic();
}

void Audio_FadeOut(int32 milliseconds)				// 음악 효과 - 음악 서서히 정지
{
	Mix_FadeOutMusic(milliseconds);
}

void Audio_HookMusicFinished(void(*callback)(void))	// 음악 재생이 끝날 때 콜백을 추가한다. - 일시정지, 재개 시 콜백부터.
{
	Mix_HookMusicFinished(callback);
}

bool Audio_IsMusicPlaying(void)						// 음악이 현재 재생 중인지 판별.
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

bool Audio_IsMusicPaused(void)						// 음악이 현재 멈췄는지 판별.
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

EMusicFading Audio_IsMusicFading(void)				// 음악이 효과 - 페이드 중인지 판별.
{
	Mix_Fading result = Mix_FadingMusic();

	return result;
}

void Audio_LoadMusic(Music* music, const char* filename)	// 음악을 로드.
{
	LogInfo("Sound Loading... %s", filename);

	sprintf_s(s_path, sizeof(s_path), "%s/%s", SOUND_DIRECTORY, filename);
	music->Music = Mix_LoadMUS(s_path);
}

void Audio_FreeMusic(Music* music)							// 음악 객체 관련 자원을 해제. 반드시 Release()에 등록.
{
	Mix_FreeMusic(music->Music);
	music->Music = NULL;
}

void Audio_LoadSoundEffect(SoundEffect* soundEffect, const char* filename)	// SE를 로드.
{
	LogInfo("Sound Effect Loading... %s", filename);

	sprintf_s(s_path, sizeof(s_path), "%s/%s", SOUND_DIRECTORY, filename);
	soundEffect->Chunk = Mix_LoadWAV(s_path);
	Audio_SetEffectVolume(soundEffect, 1.0f);
}

void Audio_FreeSoundEffect(SoundEffect* soundEffect)		// SE 객체 관련 자원을 해제. 반드시 Release()에 등록.
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

void Audio_PlaySoundEffect(const SoundEffect* soundEffect, int32 loops)	// SE 재생
{
	Mix_PlayChannel(-1, soundEffect->Chunk, loops);
}

void Audio_PlaySoundEffectFadeIn(SoundEffect* soundEffect, int32 loops, int32 milliseconds)	// SE 효과 - 서서히 재생
{
	Mix_FadeInChannel(-1, soundEffect->Chunk, loops, milliseconds);
}

void Audio_PauseSoundEffect(void)										// SE 재생 정지
{
	Mix_Pause(-1);
}

void Audio_ResumeSoundEffect(void)										// SE 재생 재개
{
	Mix_Resume(-1);
}

void Audio_StopSoundEffect(void)										// SE 정지
{
	Mix_HaltChannel(-1);
}

void Audio_FadeOutSoundEffect(int32 milliseconds)						// SE 효과 - 서서히 정지
{
	Mix_FadeOutChannel(-1, milliseconds);
}

void Audio_HookSoundEffectFinished(void(*callback)(int32))				// SE 재생이 끝날 때 콜백을 추가한다. - 일시정지, 재개 시 콜백부터.
{
	Mix_ChannelFinished(callback);
}

bool Audio_IsSoundEffectPlaying(void)									// SE가 현재 재생 중인지 판별.
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

bool Audio_IsSoundEffectPaused(void)									// SE가 현재 멈췄는지 판별.
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


// 가능 기능
// 
// 음악 로드			Load
// 음악 볼륨 설정		SetVolume
// 음악 볼륨 받기		GetVolume
// 음악 재생			Play
// 음악 되감기		Rewind
// 음악 정지			Stop
// 음악 일시 정지		Paused
// 음악 콜백 등록		Hook
// 음악 재생 재개		Resume
// 음악 페이드 인		PlayFadeIn
// 음악 페이드 아웃	FadeOut
// 음악 재생중 판별	IsPlaying
// 음악 정지중 판별	IsPaused 
// 음악 자원 해제		FreeMusic
// 
// SE 로드			LoadSoundEffect
// SE 볼륨 설정		SetEffectVolume
// SE 볼륨 받기		GetEffectVolume
// SE 재생			PlaySoundEffect
// SE 정지			StopSoundEffect
// SE 일시 정지		PausedSoundEffect
// SE 콜백 등록		HookSoundEffect
// SE 재생 재개		ResumeSoundEffect
// SE 페이드 인		PlayFadeInSoundEffect
// SE 페이드 아웃	FadeOutSoundEffect
// SE 재생중 판별	IsPlayingSoundEffect
// SE 정지중 판별	IsPausedSoundEffect
// SE 자원 해제		FreeSoundEffect
// 