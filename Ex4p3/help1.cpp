
#include "help1.h"

BalancedSearchBinaryTree<size_t, string> help1::bsbt;
SearchBinaryTree<size_t, string> help1::sbt;
HashMap help1::hm;

void help1::fillFromFile(string file) {
    ifstream ifs(file);
    string line;
    int pos;
    vector<Pair<int, string>> pairs;
    while (!ifs.eof()) {
        pos = ifs.tellg();
        getline(ifs, line);
        pairs.push_back(Pair<int, string>(pos, line));
    }
    vector<Pair<size_t, size_t>> timeSpentPairs = insert(pairs);
    printf("Вставка заняло время:\n");
    for (size_t i = 0; i < timeSpentPairs.size(); i++) {
        auto pair = timeSpentPairs[i];
        printStructureType(i + 1);
        printf("%dms\n", pair.second - pair.first);
    }
}

void help1::clear() {
    bsbt = BalancedSearchBinaryTree<size_t, string>();
    sbt = SearchBinaryTree<size_t, string>();
    hm = HashMap();
}

void help1::printStructureType(int type) {
    switch (type) {
    case 1:
        printf("Хеш-таблица: ");
        break;
    case 2:
        printf("Бинарное Дерево Поиска: ");
        break;
    case 3:
        printf("Сбалансированное Дерево Поиска: ");
        break;
    }
}

void help1::printTimeInsert(vector<Pair<int, string>> pairs) {
    auto timeSpentPairs = insert(pairs);
    for (size_t i = 0; i < timeSpentPairs.size(); i++) {
        auto pair = timeSpentPairs[i];
        printStructureType(i + 1);
        printf("%dms\n", pair.second - pair.first);
    }
}

void help1::printTimePrint(bool isLargeFile) {
    auto timeSpentPairs = printObject(isLargeFile);
    for (size_t i = 0; i < timeSpentPairs.size(); i++) {
        auto pair = timeSpentPairs[i];
        printStructureType(i + 1);
        printf("%dms\n", pair.second - pair.first);
    }
}

void help1::printTimeRemove(int x) {
    auto timeSpentPairs = remove(x);
    for (size_t i = 0; i < timeSpentPairs.size(); i++) {
        auto pair = timeSpentPairs[i];
        printStructureType(i + 1);
        printf("%dms\n", pair.second - pair.first);
    }
}

shared_ptr<string> help1::printTimeFind(int x) {
    auto result = find(x);
    auto timeSpentPairs = result.second;
    shared_ptr<string> value = result.first;
    for (size_t i = 0; i < timeSpentPairs.size(); i++) {
        auto pair = timeSpentPairs[i];
        printStructureType(i + 1);
        printf("%dms\n", pair.second - pair.first);
    }
    return value;
}

Pair<shared_ptr<string>, vector<Pair<size_t, size_t>>> help1::find(int x) {
    vector<Pair<size_t, size_t>> timeSpentPairs;
    size_t startTimeMS, endTimeMS, diffTimeMS;
    shared_ptr<string> result;
    for (size_t cS = 1; cS <= 3; cS++) {
        startTimeMS = clock();
        switch (cS) {
        case 1:
            result = shared_ptr<string>(new string(hm.find(x).name));
            break;
        case 2:
            result = sbt.searchValueByKey(x);
            break;
        case 3:
            result = bsbt.searchValueByKey(x);
            break;
        }
        endTimeMS = clock();
        timeSpentPairs.push_back(Pair<size_t, size_t>(startTimeMS, endTimeMS));
    }
    return Pair<shared_ptr<string>, vector<Pair<size_t, size_t>>>(result, timeSpentPairs);
}

vector<Pair<size_t, size_t>> help1::remove(int x) {
    vector<Pair<size_t, size_t>> timeSpentPairs;
    size_t startTimeMS, endTimeMS, diffTimeMS;
    for (size_t cS = 1; cS <= 3; cS++) {
        startTimeMS = clock();
        switch (cS) {
        case 1:
            hm.pop(hm.find(x));
            break;
        case 2:
            sbt.deleteNode(x);
            break;
        case 3:
            bsbt.deleteNode(x);
            break;
        }
        endTimeMS = clock();
        timeSpentPairs.push_back(Pair<size_t, size_t>(startTimeMS, endTimeMS));
    }
    return timeSpentPairs;
}

vector<Pair<size_t, size_t>> help1::insert(vector<Pair<int, string>> pairs) {
    vector<Pair<size_t, size_t>> timeSpentPairs;
    size_t startTimeMS, endTimeMS, diffTimeMS;
    for (size_t cS = 1; cS <= 3; cS++) {
        startTimeMS = clock();
        for (size_t i = 0; i < pairs.size(); i++) {
            Pair<int, string> pair = pairs[i];
            switch (cS) {
            case 1:
                hm.push(Node(pair.first, pair.second));
                break;
            case 2:
                bsbt.insert(pair.first, pair.second);
                break;
            case 3:
                sbt.insert(pair.first, pair.second);
                break;
            }
        }
        endTimeMS = clock();
        timeSpentPairs.push_back(Pair<size_t, size_t>(startTimeMS, endTimeMS));
    }
    return timeSpentPairs;
}

vector<Pair<size_t, size_t>> help1::insert(int x, string s) {
    vector<Pair<int, string>> pairs;
    pairs.push_back(Pair<int, string>(x, s));
    return insert(pairs);
}

vector<Pair<size_t, size_t>> help1::printObject(bool isLargeFile) {
    vector<Pair<size_t, size_t>> timeSpentPairs;
    size_t startTimeMS, endTimeMS, diffTimeMS;
    for (size_t cS = 1; cS <= 3; cS++) {
        startTimeMS = clock();
        printStructureType(cS);
        if (isLargeFile) printf(" СЛИШКОМ БОЛЬШАЯ СТРУКТУРА");
        printf("\n");
        switch (cS) {
        case 1:
            hm.output(!isLargeFile);
            break;
        case 2:
            sbt.displayBinTree(!isLargeFile);
            break;
        case 3:
            bsbt.displayBinTree(!isLargeFile);
            break;
        }
        endTimeMS = clock();
        timeSpentPairs.push_back(Pair<size_t, size_t>(startTimeMS, endTimeMS));
    }
    return timeSpentPairs;
}
