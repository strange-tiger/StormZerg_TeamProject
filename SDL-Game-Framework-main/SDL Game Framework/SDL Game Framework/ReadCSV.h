#pragma once

#include "Type.h"

// 씬 데이터 구조체
typedef struct Scene_Data
{
    int32       sceneNum;
    char     backImgName[128];
    char     imgName[128];
    int32       imgOutputStyle;

    char     soundMusicName[128];
    int32       soundMusicStyle;
    char     soundEffectName[128];
    int32       soundEffectTime;
    int32       soundEffectStyle;
    wchar_t     text[1024];

    wchar_t     chooseText_1[128];
    wchar_t     chooseText_2[128];
    wchar_t     chooseText_3[128];
    int32       nextScene_1;
    int32       nextScene_2;
    int32       nextScene_3;
    int32       parentScene;

    bool        passwordExist_1;
    bool        passwordExist_2;
    bool        masterKey;
}SceneData;

SceneData Read_CSV(char* csvName, int32 sceneDestNum);