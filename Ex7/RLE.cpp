#include <fstream>
#include <vector>
#include <string>


namespace rle {
    using namespace std;

    void static compress(istream& in, ostream& out, bool grouping = false) {
        printf("Compressing...\n");
        bool isUniquePrev = false;
        bool isUnique = isUniquePrev;
        int col = 0;
        vector<char> chars;
        for (; !in.eof(); isUniquePrev = isUnique) {
	        const char ch = in.get();
            chars.push_back(ch);
            col++;
            if (ch != in.peek()) {//UNIQUE
                isUnique = true;
                if (!grouping) {
                    out << col << string(chars.data(), chars.size());
                    chars.clear();
                    col = 0;
                    continue;
                }
                if (!isUniquePrev) {
                    out << col << string(chars.data(), chars.size());
                    chars.clear();
                    col = 0;
                }
                continue;
            } else {//NOT UNIQUE
                isUnique = false;
                if (!grouping) {
                    chars.clear();
                    continue;
                }
                if (isUniquePrev) {
                    if (grouping) out << "-";
                    out << col << string(chars.data(), chars.size());
                    chars.clear();
                    col = 0;
                }
                continue;
            }
            if (chars.size() >= 127 + (isUniquePrev ? 1 : 0)) {
                if (isUniquePrev) {
                    out << "-";
                }
                out << col << string(chars.data(), chars.size());
                chars.clear();
                col = 0;
                continue;
            }
        }
        if (!chars.empty()) {
            if (isUnique) {
                out << "-";
            }
            out << col << string(chars.data(), chars.size());
            chars.clear();
            col = 0;
        }
        printf("Complete!\n");
    }
    void static decompress(istream& in, ostream& out) {
        printf("Decompressing...\n");
        int num = 0;
        bool isUniqueGroup = false;
        while (!in.eof()) {
            char ch = in.peek();
            if (isdigit(ch)) {
                num = num * 10 + (ch - '0');
                in.get();
                continue;
            } 
            if (ch == '-') {
                isUniqueGroup = true;
                in.get();
                continue;
            }
            if (isUniqueGroup) {
                for (size_t i = abs(num); i > 0; i--) {
                    char c = in.get();
                    out << c;
                }
                num = 0;
                isUniqueGroup = false;
                continue;
            }
           for (size_t i = abs(num); i > 0; i--) {
               out << ch;
           }
           num = 0;
           in.get();
        }
        printf("Complete!\n");
    }
}