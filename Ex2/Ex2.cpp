#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <list>
using namespace std;

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
	string name = ""; //фио
	int id; //номер читательского
	string adress = ""; //адрес
	string output() {
		printf("(Номер читательского: %d, ФИО: %s, Адрес: %s", id, name, adress);
	}
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
private:
	int getHash(Node);
	int getHash(int);
public:
	HashTable() {
		table.resize(this->size);
	}
	//добавить в таблицу 
	void push(Node node) {
		int key = getHash(node);
		while (!table[key].isFree) key += step;
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
		node.isFree = true;
		node.isDeleted = true;

		amountItemsExisting--;
		amountItemsDeleted++;
	}
	//форматированный вывод таблицы в консоль
	void output() {
		for (auto i = 0; i < this->table.size(); i++) {
			printf("%d ");
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

	~HashTable() {
	}

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

	//рехэширование таблицы
	void rehash() {
		amountItemsDeleted = 0;
		if (isOverflow()) size *= 2;
		amountItemsExisting = 0;
		vector<Node> temp(table);
		table.clear();
		table.resize(size);
		while (!(temp.empty())) {
			push(temp.back());
			temp.pop_back();
		}
	}
};

/*class File {
	ifstream IFile; //поток чтения из файла
	ofstream OFile; //поток записи в файл
	HashTable* hashTable; //хэш-таблица
	int position; //текущая позиция
public:
	File(string path) {
		IFile.open(path);
		OFile.open(path, ios::app);
		hashTable = new HashTable();
		this->fillTable();
	}
	//поиск записи по ключу
	Node find(int key) {
		return this->hashTable->find(key);
	}
	//всорптавка записи в таблицу и файл
	void push(Node newNode) {
		this->position++;
		newNode.position = position;
		this->hashTable->push(newNode);
		OFile << endl;
		OFile << newNode.name << " "
			<< newNode.code;
	}

	void output() {
		this->hashTable->output();
	}


	void pop(int key) {
		hashTable->pop(key);
	}

	~File() {
		delete hashTable;
		IFile.close();
		OFile.close();
		position = 0;
	}

private:
	//заполнение таблицы из файла
	void fillTable() {
		int buff, pos;
		string name;
		while (IFile >> name) {
			Node* node = new Node();
			node->name = name;
			IFile >> buff;
			node->code = buff;
			node->position = position;
			position++;
			this->hashTable->push(*node);
		}
	}
};*/

void main() {
	setlocale(LC_ALL, "RUS");
	File* file = new File("database.txt");
	file->output();

	cout << "обновление новой записи" << endl;
	Node node = { "Шампунь", 179865 };
	file->push(node);
	file->output();

	cout << "Удаление по ключу: " << endl;
	file->find(179865).output();
	cout << endl;
	file->pop(789456);
	file->output();
	system("pause");
}
