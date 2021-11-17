#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define PHRASE_END 0
#define PHRASE_EMPTY 1
#define PHRASE_1ST 2
#include <string>

/*
	Упаковка/распаковка файлов по алгоритму LZ78
*/

#include <cstdio>
#include "tchar.h"
#include <cstdlib>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <sys/stat.h>

using namespace std;

namespace lz78 {

	struct dict_entry {
		char* data; // данные
		int len; // длина
	public:
		dict_entry(dict_entry* src, char c);
		dict_entry(char c);
		~dict_entry();
		bool match(dict_entry* entry, char c); // поиск
	};
	//---------------------------------------------------------
	// главная процедура
	int _tmain(string fin, string fon, int mode = 1);
}