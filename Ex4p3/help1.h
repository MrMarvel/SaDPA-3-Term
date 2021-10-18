#pragma once
#include "BalancedSearchBinaryTree.h"
#include "SearchBinaryTree.h"
#include "HashMap.h"
#include "Pair.h"

namespace help1 {
	extern BalancedSearchBinaryTree<size_t, string> bsbt;
	extern SearchBinaryTree<size_t, string> sbt;
	extern HashMap hm;

	void fillFromFile(string);
	void clear();
	void printStructureType(int);
	void printTimeInsert(vector<Pair<int, string>>);
	void printTimePrint(bool = false);
	void printTimeRemove(int);
	shared_ptr<string> printTimeFind(int);
	Pair<shared_ptr<string>, vector<Pair<size_t, size_t>>> find(int);
	vector<Pair<size_t, size_t>> remove(int);
	vector<Pair<size_t, size_t>> insert(int, string);
	vector<Pair<size_t, size_t>> insert(vector<Pair<int, string>>);
	vector<Pair<size_t, size_t>> printObject(bool = false);
}