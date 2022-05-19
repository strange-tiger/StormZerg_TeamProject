#pragma once

#include <wchar.h>

#define MAXIMUM_ROW 800

typedef struct CsvItem
{
	char* RawData;
} CsvItem;

typedef struct CsvFile
{
	int			ColumnCount;
	int			RowCount;
	CsvItem* Items[MAXIMUM_ROW];
} CsvFile;

typedef enum CsvList
{
	SENCE_NUMBER,
	BACK_IMG_NAME,
	CHOOSE_IMG_NAME,
	IMG_OUTPUT_STYLE,
	SOUND_MUSIC_NAME,
	SOUND_MUSIC_STYLE,
	SOUND_EFFECT_NAME,
	SOUND_EFFECT_PLAY_TIME,
	SOUND_EFFECT_STYLE,
	TEXT_NAME,
	FULL_TEXT,
	CHOOSE_TEXT_1,
	CHOOSE_TEXT_2,
	CHOOSE_TEXT_3,
	SAVE_POINT_SCENE,
	NEXT_SCENE_1,
	NEXT_SCENE_2,
	NEXT_SCENE_3,
	PASSWORD_1,
	PASSWORD_2,
	MASTER_KEY,
	CHOOSE_QUANTITY
};

void		CreateCsvFile(CsvFile* csvFile, const char* filename);
void		FreeCsvFile(CsvFile* csvFile);
int			ParseToInt(const CsvItem item);
char*		ParseToAscii(const CsvItem item);
wchar_t*	ParseToUnicode(const CsvItem item);