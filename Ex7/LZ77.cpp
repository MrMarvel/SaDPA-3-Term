#include <fstream>
#include <vector>
#include <sstream>

namespace lz77 {
	using namespace std;
	struct code {
		int off;
		int len;
		char next_char;
	};
	#define MAX_WINDOW 10

	//These are handy bits that go in a header somewhere
	template<class e, class t, int N>
	std::basic_istream<e, t>& operator>>(std::basic_istream<e, t>& in, const e(&sliteral)[N]) {
		e buffer[N - 1] = {}; //get buffer
		in >> buffer[0]; //skips whitespace
		if (N > 2)
			in.read(buffer + 1, N - 2); //read the rest
		if (strncmp(buffer, sliteral, N - 1)) //if it failed
			in.setstate(std::ios::failbit); //set the state
		return in;
	}
	template<class e, class t>
	std::basic_istream<e, t>& operator>>(std::basic_istream<e, t>& in, const e& cliteral) {
		e buffer(0);  //get buffer
		in >> buffer; //read data
		if (buffer != cliteral) //if it failed
			in.setstate(std::ios::failbit); //set the state
		return in;
	}
	//redirect mutable char arrays to their normal function
	template<class e, class t, int N>
	std::basic_istream<e, t>& operator>>(std::basic_istream<e, t>& in, e(&carray)[N]) {
		return std::operator>>(in, carray);
	}

	string static stream_to_string(istream& in, const char delim = '\n') {
		stringstream lines;
		string line;
		for (bool o = false; !in.eof(); o = true) {
			getline(in, line, delim);
			if (o) lines << delim;
			lines << line;
		}
		return lines.str();
	}

	void static compress(istream& in, ostream& out) {
        printf("Compressing...\n");
		vector<code> vt;
		//string str = "abacdbacc";
		int window1, window2;
        const string str = stream_to_string(in);
		//Код 
		for (int i = 0; i < str.length(); i++) {
			if (i + 1 <= MAX_WINDOW) window1 = i; // Определяем размер окна запроса 
			else window1 = MAX_WINDOW;

			if (i + window1 < str.length()) window2 = window1;
			else window2 = str.length() - i;

			//printf("%d %d %d\n", window1, window2, i);
			string str1 = str.substr(i - window1, window1);
			string str2 = str.substr(i, window2);
			//cout << str1 << " : " << str2 << endl;

			int off = -1;
			while (true) {
				if (window2 == 0) break; // Пустая строка, прямой выход 
				string str3 = str2.substr(0, window2);
				off = str1.find(str3); // Не найдено, выкл. = -1 
				//cout << str3 << " :: " << off << " :: " << i + window2 << endl;
				if (off != -1) break; // Нашли, выходим 
				window2--;
				if (window2 <= 0) break;
			}

			if (off != -1) {
				code cd;
				cd.len = window2;
				cd.off = window1 - off;
				cd.next_char = str[i + window2];
				vt.push_back(cd);
				i += window2;
			} else {
				code cd;
				cd.len = 0;
				cd.off = 0;
				cd.next_char = str[i + window2];
				vt.push_back(cd);
			}
		}

		for (const auto& i : vt) { // Результат кодирования 
			out << "(" << i.off << " " << i.len << " " << i.next_char << ")";
		}
		printf("\n");
        printf("Complete!\n");
    }
    void static decompress(istream& in, ostream& out) {
        printf("Decompressing...\n");
		vector<code> vt;
		// декодирование
		int a;
		while (!in.eof()) {
			code cd{};
			in >> "(" >> cd.off >> cd.len >> cd.next_char;
			if (in.eof()) break;
			in >> ")";
			vt.push_back(cd);
		}
		string str_out;
		for (const auto& code : vt) {
			if (code.len == 0) {
				str_out += code.next_char;
			} else {
				int len = str_out.length();
				len -= code.off;
				string temp = str_out.substr(len, code.len);
				str_out += temp + code.next_char;
			}
		}
		out << str_out;
        printf("Complete!\n");
    }
}