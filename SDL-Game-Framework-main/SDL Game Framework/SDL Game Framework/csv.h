#pragma once

#include <wchar.h>

#define MAXIMUM_ROW 200

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

void		CreateCsvFile(CsvFile* csvFile, const char* filename);
void		FreeCsvFile(CsvFile* csvFile);
int			ParseToInt(const CsvItem item);
char*		ParseToAscii(const CsvItem item);
wchar_t*	ParseToUnicode(const CsvItem item);