#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <tuple>
#include <filesystem>
#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <ostream>
#include <queue>
#include "BST.h"


//#define cimg_display 0
#include "../CImg-3.3.5/CImg.h"

#include "suppFuncs.h"


using std::cin;
using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::string;
using std::ifstream;
using std::ios;

using namespace std::filesystem;
using namespace cimg_library;

string encode(const string & name, const vector<pair<int, string>> & encodedData, int i);

template<typename InputIt>
void bisect(InputIt beg, InputIt end, int bytes, int allFreq, vector<vector<int>> & byteVec, int count) {
    if (next(beg) == end) {
        byteVec.push_back(vector{ (*beg)[0], bytes, count});
        return;
    }
    int curSum = 0;
    int lastDif = allFreq;
    for (InputIt it = beg; it != end; ++it) {
        curSum += (*it)[1];
        int curDif = abs(curSum - (allFreq - curSum));
        if (curDif >= lastDif) {
            bisect(beg, it, bytes << 1, curSum - (*it)[1], byteVec, count + 1);
            bisect(it, end, (bytes << 1) | 1, allFreq - curSum + (*it)[1], byteVec, count + 1);
            return;
        }
        lastDif = curDif;
    }
}

vector<pair<int, string>> toFano(const vector<vector<int>> & dict, int allFreq) {
    vector<vector<int>> byteVec;
    bisect(dict.begin(), dict.end(), 0, allFreq, byteVec, 0);


    vector<pair<int, string>> res;
    for (int i = 0; i < dict.size(); ++i) {
        auto bits = getBits(byteVec[i][1], byteVec[i][2]);
        res.push_back(make_pair(byteVec[i][0], toString(bits.begin(), bits.end())));
    }
    return res;
}

vector<pair<int, string>> toFano(const string & name, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);
    vector<vector<int>> dict = frequencyDict(pixels, i);
    int allFreq = 0;
    for_each(dict.begin(), dict.end(), [&allFreq](auto x){ allFreq += x[1]; });

    return toFano(dict, allFreq);
}

using HaffmanNode = HaffmanTreeNode<pair<string, int>>;

vector<pair<int, string>> toHaffman(const vector<vector<int>> & dict, int allFreq) {
    BinarySearchTree<tuple<string, int, HaffmanNode *>, cmpPair<tuple<string, int, HaffmanNode *>>> tree;
    vector<pair<int, string>> res;

    vector<HaffmanNode *> vecNodes;

    for (const auto & elem: dict) tree.insert( {to_string(elem[0]), elem[1], nullptr} );

    while (tree.size() != 1) {
        auto elem = prev(tree.end());
        auto prevElem = prev(elem);
        HaffmanNode * elemNode;
        HaffmanNode * prevElemNode;
        if (tree.size() == 7)
            1;
        if (get<2>(*elem) == nullptr) {
            elemNode = new HaffmanNode({get<0>(*elem), get<1>(*elem)});
            vecNodes.insert(vecNodes.begin(), elemNode);
        }
        else
            elemNode = get<2>(*elem);

        if (get<2>(*prevElem) == nullptr) {
            prevElemNode = new HaffmanNode({get<0>(*prevElem), get<1>(*prevElem)});
            vecNodes.insert(vecNodes.begin(), prevElemNode);
        }
        else
            prevElemNode = get<2>(*prevElem);

        HaffmanNode * node = new HaffmanNode({prevElemNode -> data.first + elemNode -> data.first, elemNode -> data.second + prevElemNode -> data.second}, elemNode, prevElemNode);
        elemNode -> p = node;
        elemNode -> data.second = 1;
        prevElemNode -> p = node;
        prevElemNode -> data.second = 0;
        tree.erase(elem);
        tree.erase(prevElem);
        tree.insert({node -> data.first, node -> data.second, node} );
    }

    for (const auto & elem: vecNodes) {
        string seq = "";
        auto curElem = elem;
        while (curElem -> p != nullptr) {
            seq = to_string((curElem -> data.second)) + seq;
            curElem = curElem -> p;
        }
        res.push_back({ stoi(elem -> data.first), seq });
    }

    return res;
}

vector<pair<int, string>> toHaffman(const string & name, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);
    vector<vector<int>> dict = frequencyDict(pixels, i);
    int allFreq = 0;
    for_each(dict.begin(), dict.end(), [&allFreq](auto x){ allFreq += x[1]; });

    return toHaffman(dict, allFreq);
}

typedef vector<pair<int, string>> (*encodingFunc)(const vector<vector<int>> &, int);

void saveToTxt(const string & name, const string & outputName, encodingFunc f, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);
    vector<vector<int>> dict = frequencyDict(pixels, i);
    int allFreq = 0;
    for_each(dict.begin(), dict.end(), [&allFreq](auto x){ allFreq += x[1]; });

    auto encodedData = f(dict, allFreq);
    auto str = encode(name, encodedData, i);

    ofstream file;
    file.open(outputName);
    for (auto kv: encodedData)
        file << kv.first << ':' << kv.second << ' ';
    file << '\n';
    file << str << '\n';

    file.close();
}

vector<pair<int, string>> compare(const string & name, encodingFunc f, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);
    vector<vector<int>> dict = frequencyDict(pixels, i);
    int allFreq = 0;
    for_each(dict.begin(), dict.end(), [&allFreq](auto x){ allFreq += x[1]; });

    auto encodedData = f(dict, allFreq);
    auto data = toMap(encodedData);

    string str = "";
    string otherStr = "";
    int width = img.width();
    int height = img.height();
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++) {
            if (i != -1) {
                str += data[img(c, r, 0, i)];
                otherStr += bitset<8>((img(c, r, 0, i))).to_string();
            }
            else
                for (int j = 0; j < 3; ++j) {
                    str += data[img(c, r, 0, j)];
                    otherStr += bitset<8>((img(c, r, 0, j))).to_string();
                }
        }

    vector<pair<int, string>> res;
    res = { {str.size(), str}, {otherStr.size(), otherStr} };

    return res;
}

void decode(const string & name) {
    ifstream file(name);
    string line;
    map<string, int> dict;
    int i = 0;
    string str = "";
    while (getline(file, line)) {
        if (i == 0) {
            auto beg = line.begin();
            int curNum = 0;
            string curSeq = "";
            bool flag = false;
            while (beg != line.end()) {
                if (*beg == ':') {
                    flag = true;
                    ++beg;
                    continue;
                }
                else if (*beg == ' ') {
                    dict[curSeq] = curNum;
                    curNum = 0;
                    curSeq = "";
                    flag = false;
                    ++beg;
                    continue;
                }

                if (!flag) curNum = curNum * 10 + (*beg - '0');
                else curSeq += *beg;

                ++beg;
            }
            ++i;
        }
        else {
            auto beg = line.begin();
            string curSeq = "";
            int j = 0;
            while (beg != line.end()) {
                curSeq += *beg++;
                if (dict.count(curSeq)) {
                    if (j % 3 == 0) str += "(";
                    str += to_string(dict[curSeq]);
                    if (j % 3 == 2) str += ") ";
                    else str += + ", ";

                    curSeq = "";
                    ++j;
                }
            }
        }
    }

    cout << str << endl;
}

string decode(const string & data, const vector<pair<int, string>> & vec) {
    map<string, int> dict = toInvMap(vec);
    string res = "";
    string cur = "";
    for (auto x: data) {
        cur += x;
        if (dict.count(cur)) {
            res += bitset<8>(dict[cur]).to_string();
            cur = "";
        }
    }

    return res;
}

string encode(const string & name, const vector<pair<int, string>> & encodedData, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto data = toMap(encodedData);

    string str = "";
    int width = img.width();
    int height = img.height();
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++) {
            if (i != -1) {
                str += data[img(c, r, 0, i)];
            }
            else {
                for (int j = 0; j < 3; ++j) {
                    str += data[img(c, r, 0, j)];
                }
            }
        }
    return str;
}

//void encodeImage(const string & name, encodingFunc f, int i = -1) {
//    path p(name); //filesystem::
//    string newName = "../Output Images/" + p.filename().string();
//    copyBMPHeader(name, newName);
//}