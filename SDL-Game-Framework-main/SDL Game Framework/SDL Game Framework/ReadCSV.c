#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ReadCSV.h"

SceneData p;

SceneData Read_CSV(char* csvName, int32 sceneDestNum)
{
	setlocale(LC_ALL, "kr_KR.utf8");

	wchar_t str[1024];
	FILE* pFile;

	if (0 == fopen_s(&pFile, "DB_project.csv", "rt")) {

		// sceneDestNum = 1;


		//맨 처음 줄 넘기기
		if (NULL != fgets(str, 1024, pFile)) {
			memset(str, 0, sizeof(str));
		}

        while (fscanf(pFile, "%[^\n] ", str) != EOF) {
            
            char* ptr = strtok(str, ",");
            WCHAR wtext[1024] = L"";
            MultiByteToWideChar(CP_ACP, NULL, p.text, -1, wtext, 1024);
            int count = 0;

            while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
            {
                switch (count)
                {
                case 0:
                    p.sceneNum = atoi(ptr);
                    break;
                case 1:
                    wcscpy(p.backImgName, ptr);
                    break;
                case 2:
                    wcscpy(p.imgName, ptr);
                    break;
                case 3:
                    p.imgOutputStyle = atoi(ptr);
                    break;
                case 4:
                    wcscpy(p.soundMusicName, ptr);
                    break;
                case 5:
                    p.soundMusicStyle = atoi(ptr);
                    break;
                case 6:
                    wcscpy(p.soundEffectName, ptr);
                    break;
                case 7:
                    p.soundEffectTime = atoi(ptr);
                    break;
                case 8:
                    p.soundEffectStyle = atoi(ptr);
                    break;
                case 9:
                    wcscpy(wtext, ptr);
                    break;
                case 10:
                    wcscpy(p.chooseText_1, ptr);
                    break;
                case 11:
                    wcscpy(p.chooseText_2, ptr);
                    break;
                case 12:
                    wcscpy(p.chooseText_3, ptr);
                    break;
                case 13:
                    p.nextScene_1 = atoi(ptr);
                    break;
                case 14:
                    p.nextScene_2 = atoi(ptr);
                    break;
                case 15:
                    p.nextScene_3 = atoi(ptr);
                    break;
                case 16:
                    p.parentScene = atoi(ptr);
                    break;
                case 17:
                    p.passwordExist_1 = atoi(ptr);
                    break;
                case 18:
                    p.passwordExist_2 = atoi(ptr);
                    break;
                case 19:
                    p.masterKey = atoi(ptr);
                    break;
                }

                if (sceneDestNum < p.sceneNum)
                {
                    //printf("%s\n", p.text);
                    break;
                }
                count++;
                ptr = strtok(NULL, ",");      // 다음 문자열을 잘라서 포인터를 반환
            }

			memset(str, 0, sizeof(str));

            
            // MultiByteToWideChar(CP_ACP, NULL, p.text, -1, wtext, 1024);

		}


	}
	fclose(pFile);

	return p;
}