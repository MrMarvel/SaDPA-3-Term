#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <windows.h>
#include <boost/format.hpp>

using namespace std;
std::vector<std::string> split(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	if (s.length() < 1) return elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}
int getNextPrime(int n) {
	while (1) {
		int c = 1;
		n++;
		for (int i = 2; i <= n; i++)
			if (n % i == 0)
				c++;
		if (c == 2)
			return n;
	}
}
struct Node {
	int id = -1; //номер читательского
	string name = ""; //фио
	string adress = ""; //адрес
	void output() {
		if (id < 0) {
			printf("(Пусто)");
			return;
		}
		if (isDeleted) {
			printf("(УДАЛЕНО)");
			return;
		}
		printf("(Номер читательского: %d, ФИО: ", id);
		cout << name;
		cout << ", Адрес: ";
		cout << adress;
		cout << ")";
		cout << " Позиция в файле: ";
		cout << position;
	}
	int position = -1;
	bool isFree = true;
	bool isDeleted = false;

	friend bool operator!= (Node n1, Node n2) {
		return n1.id != n2.id;
	}
	friend bool operator== (Node n1, Node n2) {
		return n1.id == n2.id;
	}
};

class HashTable {
	//открытая адресация
	int size = 11; //размер таблицы
	int amountItemsExisting = 0; //количество элементов в таблице
	int amountItemsDeleted = 0; //кол-во удалённых элементов в таблице
	int step = 1; //смещение
	vector<Node> table;
public:
	//рехэширование таблицы
	void rehash() {
		amountItemsDeleted = 0;
		if (isOverflow()) size *= 2;
		amountItemsExisting = 0;
		vector<Node> temp(table);
		table.clear();
		table.resize(size);
		while (!(temp.empty())) {
			Node node = temp.back();
			temp.pop_back();
			if (node.isFree) continue;
			if (node.isDeleted) continue;
			push(node);
		}
	}
private:
public:
	HashTable() {
		table.resize(this->size);
	}
	//добавить в таблицу 
	void push(Node node) {
		if (node.id < 0) return;
		int key = getHash(node);
		int tableSize = table.size();
		while (!table[key].isFree) {
			key += step;
			key %= tableSize;
		}
		node.isFree = false;
		node.isDeleted = false;
		table[key] = node;
		amountItemsExisting++;
		if (isOverflow()) {
			rehash();
		}
	}
	//удалить из таблицы
	void pop(Node node) {
		int key = getHash(node);
		if (key < 0 || key >= size) return;
		Node& tableNode = table[key];
		tableNode.isDeleted = true;

		amountItemsExisting--;
		amountItemsDeleted++;
	}
	//форматированный вывод таблицы в консоль
	void output() {
		int tableSizeNumbers = log10(table.size());
		for (auto i = 0; i < this->table.size(); i++) {
			Node node = table[i];
			printf(("%3d "), i);
			table[i].output();
			cout << endl;
		}
	}
	//найти запись по ключу
	Node find(int id) {
		int key = getHash(id);
		for (auto i = key; i < table.size(); i++) {
			Node& selNode = table[i];
			if (selNode.isDeleted) continue;
			if (selNode.isFree) break;
			if (selNode.id == id) return selNode;
		}
		Node badNode = Node();
		badNode.id = -1;
		return badNode;
	}

	~HashTable() {}

private:
	//проверка на переполненность
	bool isOverflow() {
		return ((double)amountItemsExisting + amountItemsDeleted) / size > 0.75 ? true : false;
	}
	//хэш-функция(по записи и отдельно по ключу)
	int getHash(Node node) {
		return node.id % size;
	}
	//хэш-функция(по записи и отдельно по ключу)
	int getHash(int id) {
		return id % size;
	}

};

class File {
	fstream file;
	ofstream O2File;
	HashTable* hashTable; //хэш-таблица
	vector<int> freeFields;
	string path;
	static const int LINE_LENGTH = 128;
public:
	void rehash() {
		hashTable->rehash();
	}
	File(string path) {
		this->path = path;
		file.open(path, fstream::out | fstream::in);
		if (!file.is_open()) throw new exception();
		//O2File.open("TEMP." + path, ofstream::out | ofstream::in);
		hashTable = new HashTable();
		this->fillTable();
	}
	//поиск записи по ключу
	Node find(int key) {
		return this->hashTable->find(key);
	}

	void output() {
		this->hashTable->output();
	}

	void add(Node newNode) {
		file.clear();
		if (freeFields.size() > 0) {
			file.seekp(freeFields.front(), ios_base::beg);
			freeFields.erase(freeFields.begin());
		} else {
			file.seekp(0, ios_base::end);
			file << endl;
		}
		newNode.position = file.tellg();
		string msg = (boost::format(string("%-" + to_string(LINE_LENGTH) + "s")) % (boost::format("%d;%s;%s") % newNode.id % newNode.name % newNode.adress)).str();
		file << msg;
		file.flush();
		push(newNode);
	}
	void pop(int key) {
		Node node = hashTable->find(key);
		if (node.id < 0) return;
		hashTable->pop(node);
		clearLine(node.position);
	}

	~File() {
		delete hashTable;
		file.close();
		O2File.close();
	}

private:
	//всорптавка записи в таблицу
	void push(Node newNode) {
		hashTable->push(newNode);
	}
	//заполнение таблицы из файла
	void clearLine(int pos) {
		file.clear();
		if (pos < 0) return;
		file.seekg(pos, ios::beg);
		string line;
		getline(file, line);
		file.clear();
		file.seekp(pos, ios::beg);
		file << string("                                                                                                                                  ").substr(0, line.length());
		file.flush();
		if (line.length() < 128) return;
		freeFields.push_back(pos);
	}
	void fillTable() {
		string line;
		while (!file.eof() && !file.bad() && !file.fail() && file.good()) {
			//file.clear();
			auto pos = file.tellg();
			file.sync();
			getline(file, line);
			vector<string> elems = split(line, ';');
			if (elems.size() < 3) {
				auto lastPos = file.tellg();
				clearLine(pos);
				file.seekg(lastPos);
				continue;
			}
			Node* node = new Node();
			int id = stoi(elems[0]);
			string name = elems[1];
			string adress = elems[2];

			node->id = id;
			node->name = name;
			node->adress = adress;
			//node->code = buff;
			node->position = pos;
			this->hashTable->push(*node);
		}
	}
};

void main() {
	setlocale(LC_ALL, "Russian");
	File* file = new File("database.txt");
	file->output();

	cout << "обновление новой записи" << endl;
	int deletingNumber = 179815;
	Node node = { deletingNumber, "Шампунь", "Вернский, 4" };
	file->add({ 12321+rand()%10, "Вадим", "Горский, 5" });
	file->add(node);
	file->output();
	cout << (boost::format("Удаление записи с номером %d:\n\t") % deletingNumber).str();
	file->find(deletingNumber).output();
	file->pop(deletingNumber);
	cout << endl;
	file->output();
	cout << "Рехеширование:\n";
	file->rehash();
	file->output();
	system("pause");
}
