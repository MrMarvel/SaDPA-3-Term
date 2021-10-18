#pragma once
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
inline std::vector<std::string> split(const std::string& s, char delim) {
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
inline int getNextPrime(int n) {
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
	int id = -1; //����
	string name = ""; //��������
	Node() {}
	Node(int i, string n) {
		id = i;
		name = n;
	}
	void output(bool show = true) {
		if (id < 0) {
			if (show) printf("(�����)");
			return;
		}
		if (isDeleted) {
			if (show) printf("(�������)");
			return;
		}
		if (show) printf("(����: %d, ��������: ", id);
		if (show) cout << name;
		if (show) cout << ")";
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

class HashMap {
	//�������� ���������
	int size = 11; //������ �������
	int amountItemsExisting = 0; //���������� ��������� � �������
	int amountItemsDeleted = 0; //���-�� �������� ��������� � �������
	int step = 1; //��������
	vector<Node> table;
public:
	//������������� �������
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
	HashMap() {
		table.resize(this->size);
	}
	//�������� � ������� 
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
	//������� �� �������
	void pop(Node node) {
		int key = getHash(node);
		if (key < 0 || key >= size) return;
		Node& tableNode = table[key];
		tableNode.isDeleted = true;

		amountItemsExisting--;
		amountItemsDeleted++;
	}
	//��������������� ����� ������� � �������
	void output(bool show = true) {
		int tableSizeNumbers = log10(table.size());
		for (auto i = 0; i < this->table.size(); i++) {
			Node node = table[i];
			if (show) printf(("%3d "), i);
			table[i].output(show);
			if (show) cout << endl;
		}
	}
	//����� ������ �� �����
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

	~HashMap() {}

private:
	//�������� �� ���������������
	bool isOverflow() {
		return ((double)amountItemsExisting + amountItemsDeleted) / size > 0.75 ? true : false;
	}
	//���-�������(�� ������ � �������� �� �����)
	int getHash(Node node) {
		return node.id % size;
	}
	//���-�������(�� ������ � �������� �� �����)
	int getHash(int id) {
		return id % size;
	}

};