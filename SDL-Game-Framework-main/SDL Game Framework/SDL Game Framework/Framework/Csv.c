#include "stdafx.h"

#include "Csv.h"

static byte* s_Buffer;
static byte* s_BufferPointer;
static char s_Path[MAX_PATH];

#define DELIMITER '|'

void readFileToBuffer(const char* filename)
{
	sprintf_s(s_Path, sizeof(s_Path), "%s/%s", DATA_DIRECTORY, filename);

	FILE* fp;
	if (0 != fopen_s(&fp, s_Path, "r"))
	{
		return;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);

	s_Buffer = malloc(sizeof(char) * (fileSize + 1));
	memset(s_Buffer, 0, sizeof(char) * (fileSize + 1));

	fseek(fp, 0, SEEK_SET);
	fread(s_Buffer, fileSize, 1, fp);

	fclose(fp);
}

int countCategory(const char* firstLine)
{
	int result = 1;
	while (*firstLine != '\n')
	{
		if (*firstLine == DELIMITER)
		{
			++result;
		}

		++firstLine;
	}

	return result;
}

void CreateCsvFile(CsvFile* csvFile, const char* filename)
{
	readFileToBuffer(filename);

	csvFile->ColumnCount = countCategory(s_Buffer);
	for (int i = 0; i < MAXIMUM_ROW; ++i)
	{
		csvFile->Items[i] = (CsvItem*)malloc(sizeof(CsvItem) * csvFile->ColumnCount);
	}

	s_BufferPointer = s_Buffer;
	while (*s_BufferPointer != '\0')
	{
		int row = csvFile->RowCount;

		// 한 줄을 읽어들인다.
		int commaCount = 0;
		const char* lineStart = s_BufferPointer;
		const char* lineEnd = lineStart;
		while (true)
		{
			if (csvFile->ColumnCount - 1 == commaCount && '\n' == *lineEnd)
			{
				break;
			}

			if (*lineEnd == DELIMITER)
			{
				++commaCount;
			}

			++lineEnd;
		}

		// 콤마 분류
		const char* recordStart = lineStart;
		const char* recordEnd = recordStart;
		for (int i = 0; i < csvFile->ColumnCount; ++i)
		{
			while (*recordEnd != DELIMITER && recordEnd != lineEnd)
			{
				++recordEnd;
			}

			int size = recordEnd - recordStart;
			csvFile->Items[row][i].RawData = (char*)malloc(sizeof(char) * (size + 1));
			memcpy(csvFile->Items[row][i].RawData, recordStart, size);
			csvFile->Items[row][i].RawData[size] = '\0';

			recordStart = recordEnd + 1;
			recordEnd = recordStart;
		}

 		++csvFile->RowCount;

		s_BufferPointer = lineEnd + 1;
	}

	free(s_Buffer);
	s_Buffer = NULL;
	s_BufferPointer = NULL;
}

void FreeCsvFile(CsvFile* csvFile)
{
	for (int r = 0; r < MAXIMUM_ROW; ++r)
	{
		if (r < csvFile->RowCount)
		{
			for (int c = 0; c < csvFile->ColumnCount; ++c)
			{
				free(csvFile->Items[r][c].RawData);
				//csvFile->Items[r][c].RawData = NULL;
			}
		}

		free(csvFile->Items[r]);
		csvFile->Items[r] = NULL;
	}
}

int ParseToInt(const CsvItem item)
{
	char* end;
	return strtol(item.RawData, &end, 10);
}

char* ParseToAscii(const CsvItem item)
{
	int size = strlen(item.RawData);
	char* result = malloc(size + 1);
	memset(result, 0, size + 1);
	if (item.RawData[0] == '"' && item.RawData[size - 1] == '"')
	{
		memcpy(result, &item.RawData[1], size - 2);
	}
	else
	{
		memcpy(result, item.RawData, size);
	}

	return result;
}

wchar_t* ParseToUnicode(const CsvItem item)
{
	int32 size = strlen(item.RawData);
	int32 wideLen = MultiByteToWideChar(CP_ACP, NULL, item.RawData, -1, NULL, 0);
	wchar_t* result = (wchar_t*)malloc(sizeof(wchar_t) * wideLen);
	memset(result, 0, sizeof(wchar_t) * wideLen);

	if (item.RawData[0] == '"' && item.RawData[size - 1] == '"')
	{
		MultiByteToWideChar(CP_ACP, NULL, &item.RawData[1], -1, result, wideLen - 3);
	}
	else
	{
		MultiByteToWideChar(CP_ACP, NULL, item.RawData, -1, result, wideLen);
	}

	return result;
}
