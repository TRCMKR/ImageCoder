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
#include <filesystem>
#include "../CImg-3.3.5/CImg.h"

#include "suppFuncs.h"

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

string embedLSB(const string & name, const string & data, const string & postfix = "_copy") {
    std::filesystem::path p(name);
    string newName = p.parent_path().string() + "/" + p.stem().string() + postfix + p.extension().string();
    cout << newName << endl;
    if (exists(newName)) std::remove(newName.c_str());
    std::filesystem::copy(name, newName);
    CImg<unsigned char> oldImg(name.c_str());
    CImg<unsigned char> img(newName.c_str());
    int width = img.width();
    int height = img.height();
    int ind = 0;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            for (int i = 0; i < 3; ++i) {
                img(c, r, 0, i) = oldImg(c, r, 0, i);
                if (ind < data.size()) img(c, r, 0, i) = ((255 - img(c, r, 0, i)) & 0xFE) | (data[ind++] - '0');
            }
        }
    }
    img.save(newName.c_str());

    return newName;
}

string embedLSBPM(const string & name, const string & data, const string & postfix = "_copy+-1") {
    std::filesystem::path p(name);
    string newName = p.parent_path().string() + "/" + p.stem().string() + postfix + p.extension().string();
    cout << newName << endl;
    if (exists(newName)) std::remove(newName.c_str());
    std::filesystem::copy(name, newName);

    CImg<unsigned char> oldImg(name.c_str());
    CImg<unsigned char> img(newName.c_str());
    int width = img.width();
    int height = img.height();
    int ind = 0;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            for (int i = 0; i < 3; ++i) {
                img(c, r, 0, i) = oldImg(c, r, 0, i);
                if (ind < data.size()) {
                    if (img(c, r, 0, i) & 1 && !(data[ind] - '0')) {
                        if (rand() > RAND_MAX / 2 && img(c, r, 0, i) != 255) img(c, r, 0, i)++;
                        else img(c, r, 0, i)--;
                    }
                    else if (!(img(c, r, 0, i) & 1) && (data[ind] - '0')) {
                        if (rand() <= RAND_MAX / 2 && img(c, r, 0, i) != 0) img(c, r, 0, i)--;
                        else img(c, r, 0, i)++;
                    }
                }
                ind++;
            }
        }
    }
    img.save(newName.c_str());

    return newName;
}

string extract(const string & name, int count) {
    string res = "";

    CImg<unsigned char> img(name.c_str());
    int width = img.width();
    int height = img.height();
    int ind = 1;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            for (int i = 0; i < 3; ++i) {
                res += (1 & img(c, r, 0, i)) + '0';
                ++ind;
                if (ind > count) return res;
            }
        }
    }

    return res;
}

string extractFromString(const string & str, int count) {
    string res = "";

    int ind = 1;
    for (int i = 0; i < str.size(); i += 8) {
        auto curStr = str.substr(i, 8);
        res += (1 & toByte(curStr)) + '0';
        ind++;
        if (ind == count) return res;
    }

    return res;
}