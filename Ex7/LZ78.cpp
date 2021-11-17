// LZ78.cpp : Defines the entry point for the console application.
//
#include "lz78.h"

namespace lz78 {
	using namespace std;

#define OFFS_LN 8
#define BYTE_LN 8

	//#define DICT_SIZE 254
#define DICT_SIZE ((1 << OFFS_LN) - 2)
//=========================================================
// Класс для ввода-вывода
	static class TPackedFile {
		int handle;
		unsigned char buffer, mask;
	public:
		void assign_read(int h);  // связывание для чтения
		void assign_write(int h); // связываение для записи
		void flush(); // заполнение нулями

		void putbit(bool val); // запись бита
		bool getbit(); //чтенние бита

		void putbits(int val, int n); // запись n битов
		int getbits(int n); // чтение n битов
	};
	//---------------------------------------------------------
	// заполнение нулями
	void TPackedFile::flush() {
		for (int i = 0; i < 7; i++) putbit(0);
	}
	//---------------------------------------------------------
	// связываение для записи
	void TPackedFile::assign_write(int h) {
		handle = h;
		buffer = 0;
		mask = 128;
	}
	//---------------------------------------------------------
	// связывание для чтения	
	void TPackedFile::assign_read(int h) {
		handle = h;
		buffer = 0;
		mask = 0;
	}
	//---------------------------------------------------------
	//чтенние бита
	bool TPackedFile::getbit() {
		if ((mask >>= 1) == 0) {
			_read(handle, &buffer, 1);
			mask = 128;//двоичное 10000000
		}
		return (buffer & mask) != 0;
	}
	//---------------------------------------------------------
	// запись бита
	void TPackedFile::putbit(bool val) {
		if (val) buffer |= mask;
		if ((mask >>= 1) == 0) {
			_write(handle, &buffer, 1);
			buffer = 0;
			mask = 128; //двоичное 10000000
		}
	}
	//---------------------------------------------------------
	// запись n битов
	void TPackedFile::putbits(int val, int n) {
		int m = 1;
		for (int i = 0; i < n; i++) {
			putbit((val & m) != 0);
			m <<= 1;
		}
	}
	//---------------------------------------------------------
	// чтение n битов
	int TPackedFile::getbits(int n) {
		int result = 0;
		for (int i = 0; i < n; i++)
			result |= getbit() << i;
		return result;
	}
	//=========================================================

	/*
		0 - конец файла, 1 - пустая фраза
		==> первая фраза имеет номер 2
	*/

	static int in_file; // входной файл
	static int out_file; // выходной файл
	static int dict_pos = 0; // текущая позиция в словаре

	static TPackedFile archive;
	//=========================================================
	// структура для словаря
	//---------------------------------------------------------
	dict_entry::dict_entry(char c) {
		data = (char*)malloc(1);
		*data = c;
		len = 1;
	}
	//---------------------------------------------------------
	dict_entry::dict_entry(dict_entry* entry, char c) {
		if (!entry) return;
		len = entry->len + 1;
		data = (char*)malloc(len);
		memcpy(data, entry->data, entry->len);
		data[entry->len] = c;
	}
	//---------------------------------------------------------
	dict_entry::~dict_entry() {
		if (!data) return;
		free(data);
	}
	//---------------------------------------------------------
	bool dict_entry::match(dict_entry* entry, char c) {
		if (len != (entry->len + 1)) return false;
		return data[entry->len] == c &&
			memcmp(entry->data, data, entry->len) == 0;
	}
	//=========================================================
	dict_entry* dict[DICT_SIZE];

	//---------------------------------------------------------
	// очистка словаря
	static void free_dict() {
		for (int i = 0; i < DICT_SIZE; i++)
			if (dict[i]) {
				delete dict[i];
				dict[i] = 0;
			}
	}
	//---------------------------------------------------------
	// поиск фразы
	static int find_phrase(int val, char c) {
		for (int i = 0; i < dict_pos; i++)
			if ((val >= PHRASE_1ST && dict[i]->match(dict[val
													 - PHRASE_1ST], c))
				|| (val == PHRASE_EMPTY && dict[i]->len
					== 1 && *dict[i]->data == c))
				return i + PHRASE_1ST;
		return PHRASE_EMPTY;
	}
	//---------------------------------------------------------
	// добавление фразы в словарь
	static void add_phrase(int n, char c) {
		if (dict_pos < DICT_SIZE) {
			if (n == PHRASE_EMPTY)
				dict[dict_pos] = new dict_entry(c);
			else dict[dict_pos] =
				new dict_entry(dict[n - PHRASE_1ST], c);
			dict_pos++;
		}
	}
	//---------------------------------------------------------
	// компрессия
	static void encode() {
		char c;
		int num = PHRASE_EMPTY;
		while (!_eof(in_file)) {
			_read(in_file, &c, 1);
			int val = find_phrase(num, c);
			if (val != PHRASE_EMPTY) num = val;
			else {
				archive.putbits(num, OFFS_LN);
				archive.putbits(c, BYTE_LN);
				add_phrase(num, c);
				num = PHRASE_EMPTY;
			}
		}
		if (num != PHRASE_EMPTY)
			for (int i = 0; i < dict[num - PHRASE_1ST]->len; i++) {
				archive.putbits(PHRASE_EMPTY, OFFS_LN);
				archive.putbits(dict[num - PHRASE_1ST]->data[i],
								BYTE_LN);
			}
		//write(out_file, "\0", 1);
		archive.putbits(PHRASE_END, OFFS_LN);
		archive.flush();
	}
	//---------------------------------------------------------
	// декомпрессия
	static void decode() {
		unsigned char num;
		char c;
		for (;;) {
			num = archive.getbits(OFFS_LN);
			if (num == PHRASE_END) break;
			c = archive.getbits(BYTE_LN);
			if (num >= PHRASE_1ST) {
				if (dict[num-PHRASE_1ST])
					_write(out_file, dict[num - PHRASE_1ST]->data,
					   dict[num - PHRASE_1ST]->len);
			}
			_write(out_file, &c, 1);
			add_phrase(num, c);
		}
	}
	//---------------------------------------------------------
	// главная процедура
	int _tmain(string fin, string fon, int mode) {
		// открытие входного и выходного файлов
		in_file = _open(fin.c_str(), _O_BINARY | _O_RDWR,
						_S_IREAD | _S_IWRITE);
		out_file = _open(fon.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT
						 | _O_TRUNC, _S_IREAD | _S_IWRITE);


		if (mode == 1) // компрессия
		{
			archive.assign_write(out_file);
			encode();
		} else // декомпрессия
		{
			archive.assign_read(in_file);
			decode();
		}

		free_dict();
		_close(in_file);
		_close(out_file);

		return 0;
	}

}