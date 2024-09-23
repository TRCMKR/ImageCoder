#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <ostream>
#include <queue>
#include "BST.h"
#include "../CImg-3.3.5/CImg.h"

#include "suppFuncs.h"
#include <filesystem>

using std::cin;
using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::string;
using std::pair;
using std::initializer_list;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::to_string;

using namespace std::filesystem;
using namespace cimg_library;

struct LFSR {
    vector<pair<int, int>> data;
    int size() { return data.size(); };

    int operator>>(int) {
        int res = data[size() - 1].first;
        int curSum = data[size() - 1].first * data[size() - 1].second;
        for (int pos = size() - 2; pos >= 0; --pos) {
            data[pos + 1].first = data[pos].first;
            curSum ^= data[pos].first * data[pos].second;
        }
        data[0].first = curSum;

        return res;
    }

    LFSR(): data() { };
    LFSR(string line): data(line.size() / 2) {
        int i = 0;
        int j = 0;
        for (auto lex: line) {
            if (!j) {
                data[i].first = lex - '0';
                ++j;
            }
            else {
                data[i].second = lex - '0';
                --j;
                ++i;
            }
        }
    };

    LFSR(initializer_list<pair<int, int>> ilst): data(ilst) { };

    using iterator = std::vector<pair<int, int>>::iterator;

    iterator begin() { return data.begin(); };
    iterator end() { return data.end(); };

    void printLn() {
        auto beg = begin();
        while (beg != end())
            cout << (*beg).first << '(' << (*beg++).second << ')' << ' ';

        cout << '\n';
    };

    void printLnV() {
        auto beg = begin();
        while (beg != end())
            cout << (*beg++).first << ' ';

        cout << '\n';
    };

    void printLnC() {
        auto beg = begin();
        while (beg != end())
            cout << (*beg++).second << ' ';

        cout << '\n';
    };
};

struct LFSRCypher {
    vector<LFSR> regs;

    int operator>>(int) {
        int res = 1;
        for (auto & reg: regs) {
            res ^= reg >> 1;
        }

        return res;
    };

    LFSRCypher(): regs() { };
    LFSRCypher(string name): regs() {
        ifstream file(name);
        string line = "";
        for (int i = 0; getline(file, line); ++i) {
            regs.push_back(LFSR(line));
        }
    };
    LFSRCypher(initializer_list<LFSR> ilst): LFSRCypher() { for (auto reg: ilst) regs.push_back(reg); };

    LFSR& operator[](int i) { return regs[i]; }

    void operator()(const string & name) {
        ifstream file(name);
        string line = "";
        regs.clear();
        for (int i = 0; getline(file, line); ++i) {
            regs.push_back(LFSR(line));
        }
    }

    void printLn() {
        for (auto reg: regs) reg.printLn();
        cout << '\n';
    };

    void printLnV() {
        for (auto reg: regs) reg.printLnV();
        cout << '\n';
    };

    void printLnC() {
        for (auto reg: regs) reg.printLnC();
        cout << '\n';
    };
};

string encrypt(const string & data, LFSRCypher cypher) {
    string res = "";
    for (int i = 0; i < data.size(); ++i) res += std::to_string((data[i] - '0') ^ (cypher >> 1));
    return res;
}

string encryptFile(const string & name, LFSRCypher cypher, bool isResult = false) {
    ifstream ifile(name);

    std::filesystem::path decrP(name);
    string decrName = decrP.stem().string() + "_decrypted" + decrP.extension().string();
    decrName = decrP.remove_filename().string() + decrName;
    remove(decrName.c_str());
    std::filesystem::path p(name);
    string newName = p.stem().string() + "_encrypted" + p.extension().string();
    newName = p.remove_filename().string() + newName;
    ofstream ofile(newName);
    string line;


    if (isResult) getline(ifile, line);
    while (getline(ifile, line)) {
        for (int i = 0; i < line.size(); ++i) {
            ofile << ((line[i] - '0') ^ (cypher >> 1));
        }
    }

    cout << newName << '\n';
    return newName;
}

string decrypt(const string & data, LFSRCypher cypher) {
    string res = "";
    for (int i = 0; i < data.size(); ++i) res += (char)(((data[i] - '0') ^ (cypher >> 1)) + '0');
    return res;
}

string decryptFile(const string & name, LFSRCypher cypher) {
    ifstream file(name);
    string buffer = "";
    string line = "";
    std::filesystem::path p(name);
    auto pos = p.stem().string().find("_encrypted");
    string newName = p.stem().string().substr(0, pos) + "_decrypted" + p.extension().string();
    newName = p.remove_filename().string() + newName;

    while (getline(file, line)) {
        for (int i = 0; i < line.size(); ++i) {
            buffer += (char)(((line[i] - '0') ^ (cypher >> 1)) + '0');
        }
    }
//    cout << buffer << '\n';
//    file.clear(ios::beg);
    ofstream ofile(name);
    ofile << buffer;
    cout << newName << '\n';
    std::rename(name.c_str(), newName.c_str());
    return newName;
}

vector<pair<int, string>> compareCypher(const string & name, LFSRCypher cypher, int i = -1) {
    string pixelDataName = pixelsToTxt(name, i);
    map<int, string> data;

    string encName = encrypt(pixelDataName, cypher);
    auto encDict = frequencyDict(encName);
    int sumEnc = 0;
    for_each(encDict.begin(), encDict.end(), [&sumEnc](auto x) { sumEnc += x[1]; });
    vector<pair<int, string>> res;
    auto enc = toHaffman(encDict, sumEnc);
    data = toMap(enc);
    string str = "";
    string otherStr = "";

    ifstream encFile(encName);
    string line = "";
    while (getline(encFile, line)) {
        int curNum = 0;
        for (int ind = 0; ind < line.size(); ++ind) {
            curNum = curNum << 1 | (line[ind] - '0');
            if (ind % 8 == 7) {
                str += data[curNum];
                curNum = 0;
            }
        }
    }

    auto decName = decrypt(encName, cypher);
    auto decDict = frequencyDict(decName);
    int sumDec = 0;
    for_each(decDict.begin(), decDict.end(), [&sumDec](auto x){ sumDec += x[1]; });
    auto dec = toHaffman(decDict, sumDec);
    data = toMap(dec);
    ifstream decFile(decName);
    line = "";
    while (getline(decFile, line)) {
        int curNum = 0;
        for (int ind = 0; ind < line.size(); ++ind) {
            curNum = curNum << 1 | (line[ind] - '0');
            if (ind % 8 == 7) {
                otherStr += data[curNum];
                curNum = 0;
            }
        }
    }

    res.push_back({str.size(), str});
    res.push_back({otherStr.size(), otherStr});

    return res;
}