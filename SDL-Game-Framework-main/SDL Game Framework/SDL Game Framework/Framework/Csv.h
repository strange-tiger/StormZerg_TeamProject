#pragma once

#include <wchar.h>
#include <Config.h>

typedef struct CsvItem
{
	char*		RawData;
} CsvItem;

typedef struct CsvFile
{
	int			ColumnCount;
	int			RowCount;
	CsvItem*	Items[MAXIMUM_ROW];
} CsvFile;

typedef enum EColumn
{
	SENCE_NUMBER,
	BACK_IMG_NAME,
	CHOOSE_IMG_NAME,
	IMG_OUTPUT_STYLE,
	SCENE_CHANGE_FADE,
	IMAGE_EFFECT_SELECT,
	IMAGE_EFFECT_START_TIME,
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
	NUMBER_OF_OPTIONS,
	NEXT_SCENE_1,
	NEXT_SCENE_2,
	NEXT_SCENE_3,
	PASSWORD_1,
	PASSWORD_2,
	MASTER_KEY
}EColumn;

void		CreateCsvFile(CsvFile* csvFile, const char* filename);
void		FreeCsvFile(CsvFile* csvFile);
int			ParseToInt(const CsvItem item);
char*		ParseToAscii(const CsvItem item);
wchar_t*	ParseToUnicode(const CsvItem item);