#include <fstream>


namespace rle {
    using namespace std;

    void static compress(istream& in, ostream& out) {
        printf("Compressing...\n");
        int     num(0);
        while (!in.eof()) {
	        const char ch = in.get();
            num++;
            if (ch != in.peek()) {
                num > 1 ? out << static_cast<char>(num - 128) << ch : out << ch;
                num = 0;
            } else if (num == 127) {
                out << static_cast<char>(num - 128) << ch;
                num = 0;
            }
        }
        printf("Complete!\n");
    }
    void static decompress(istream& in, ostream& out) {
        printf("Decompressing...\n");
        while (!in.eof()) {
            char ch = in.get();
            if (static_cast<int>(ch) < 0) {
	            const int num = static_cast<int>(ch) + 128;
               in >> ch;
               for (int i(0); i < num; i++)
                   if (in.peek() != '\0') out << ch;
            } else if (in.peek() != '\0') out << ch;
        }
        printf("Complete!\n");
    }
}