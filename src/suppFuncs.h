#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <tuple>

#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include <ostream>
#include <queue>

#include "../CImg-3.3.5/CImg.h"
//#include "Coders.h"
//#include "Cyphers.h"
#include "NTL/vec_GF2.h"
#include "suppClasses.h"

using namespace cimg_library;
//using namespace std;

using std::cin;
using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::string;
using std::pair;
using std::map;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::bitset;
using std::ostream;
using std::to_string;

void inc(NTL::vec_GF2 & vec, int i = 1) {
    if (i == 1) {
        int ind = vec.MaxLength() - 1;
        while (vec[ind] == 1) {
            vec[ind] = 0;
            --ind;
        }
        vec[ind] = 1;
    }
    else for (int j = 0; j < i; ++j) inc(vec);
}

void dec(NTL::vec_GF2 & vec, int i = 1) {
    if (i == 1) {
        int ind = vec.MaxLength() - 1;
        while (vec[ind] == 0) {
            vec[ind] = 1;
            --ind;
        }
        vec[ind] = 1;
    }
    else for (int j = 0; j < i; ++j) inc(vec);
}

void toIdentQ(my_mat_GF2 & matr) {
    int count = matr.NumRows();
    for (int i = count - 1; i >= 1; --i) {
        for (int j = i - 1; j >= 0; --j) {
            if (NTL::IsOne(matr(j, i))) {
                matr(j) += matr(i);
            }
        }
    }
}

NTL::vec_GF2 linearComb(const NTL::vec_vec_GF2 & vec, const NTL::vec_GF2 & coefs) {
    NTL::vec_GF2 res;
    res.SetLength(coefs.length());
    for (int i = 0; i < vec.length(); ++i) {
        res += vec[i] * coefs[coefs.length() - 1 - i];
    }
    return res;
}

NTL::vec_vec_GF2 linearSpan(const NTL::vec_vec_GF2 & vec) {
    NTL::vec_GF2 coefs;
    NTL::vec_vec_GF2 res;
    coefs.SetLength(vec[0].length());
    for (int i = 0; i < pow(2, vec.length()); ++i) {
        res.append(linearComb(vec, coefs));
        inc(coefs);
    }
    return res;
}

using matrix = my_mat_GF2;

NTL::vec_vec_GF2 toVect(const matrix & mat) {
    NTL::vec_vec_GF2 res;
    res.SetLength(mat.NumRows());
    for (int i = 0; i < res.length(); ++i) res[i] = mat[i];
    return res;
}

template<typename it>
NTL::vec_vec_GF2 linearSpan(it b, it e) {
    NTL::vec_vec_GF2 vec;
    vec.SetLength(e - b);
    std::copy(b, e, vec.begin());
    return linearSpan(vec);
}

int minDistance(const matrix & mat) {
    int res = INT32_MAX;
    if (mat.NumRows() == 1) res = weight(mat(0));
    else
        for (int i = 0; i < mat.NumRows(); ++i) {
            for (int j = i + 1; j < mat.NumRows(); ++j) {
                if (weight((mat(i) + mat(j))) < res)
                    res = weight((mat(i) + mat(j)));
            }
        }

    return res;
}

vector<matrix> toVector(NTL::vec_vec_GF2 basis, int count) {
    vector<matrix> vec;
    for (int i = 0; i < basis.length() / count; ++i) {
        matrix m(basis.begin() + i * count, basis.begin() + (i + 1) * count);
        vec.push_back(m);
    }

    return vec;
}

vector<int> toVector(const NTL::vec_GF2 & vec) {
    vector<int> res(vec.length());
    for (int i = 0; i < vec.length(); ++i) res[i] = NTL::IsOne(vec[i]);
    return res;
}

matrix projectMatr(const matrix & mat, const vector<int> & t) {
    NTL::vec_vec_GF2 vec;
    matrix res;
    int i = -1;
    for (const auto & coef: t) {
        ++i;
        if (!coef) continue;
        NTL::vec_GF2 tmp;
        for (int j = 0; j < mat.NumRows(); ++j) tmp.append(mat(j, i));
        vec.append(tmp);
    }
    res = transpose(to_mat_GF2(vec));
    return res;
}

matrix concatMatr(const matrix & matr1, const matrix & matr2) {
    NTL::vec_vec_GF2 vec;
    matrix res;
    for (int i = 0; i < matr1.NumRows(); ++i) {
        NTL::vec_GF2 tmp;
        tmp.SetLength(matr1.NumCols() + matr2.NumCols());
        std::copy(matr1(i).begin(), matr1(i).end(), tmp.begin());
        std::copy(matr2(i).begin(), matr2(i).end(), tmp.begin() + matr1.NumCols());
        vec.append(tmp);
    }
    res = NTL::to_mat_GF2(vec);
    return res;
}

vector<int> nOnes(int offset, int count, int n) {
    vector<int> res(n);
    for (int i = 0; i < count; ++i)
        res[i + offset] = 1;

    return res;
}

int numDigits(int x) {
    x = abs(x);
    return (x < 10 ? 1 :
            (x < 100 ? 2 :
             (x < 1000 ? 3 :
              (x < 10000 ? 4 :
               (x < 100000 ? 5 :
                (x < 1000000 ? 6 :
                 (x < 10000000 ? 7 :
                  (x < 100000000 ? 8 :
                   (x < 1000000000 ? 9 :
                    10)))))))));
}

vector<vector<int>> getImagePixels(const string & name) {
    CImg<unsigned char> img(name.c_str());
    vector<vector<int>> pixels;
    int width = img.width();
    int height = img.height();
    cout << width << "x" << height << endl;
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++) {
            pixels.push_back(vector{(int)img(c,r,0,0),
                                    (int)img(c,r,0,1),
                                    (int)img(c,r,0,2)});
        }

    return pixels;
}

vector<int> getBits(int b, int count) {
    vector<int> res;
//    int num = to_integer<int>(b);
    int num = b;
    int step = 0;
    while (num > 0 || step > count) {
        res.insert(res.begin(), num % 2);
        num /= 2;
        ++step;
    }

    while (res.size() < count) {
        res.insert(res.begin(), 0);
    }

    return res;
}

template<typename T>
string toString(T b, T e) {
    string res = "";
    while (b != e) {
        res += to_string(*b);
        ++b;
    }
    return res;
}

map<int, string> toMap(const vector<pair<int, string>> & vec) {
    map<int, string> map;
    for (auto elem: vec)
        map[elem.first] = elem.second;

    return map;
}

map<string, int> toInvMap(const vector<pair<int, string>> & vec) {
    map<string, int> map;
    for (auto elem: vec)
        map[elem.second] = elem.first;

    return map;
}

template<typename T>
class cmpVec {
public:
    bool operator()(T x, T y) { return x[1] > y[1]; };
};

template<typename T>
class cmpPair {
public:
    bool operator()(T x, T y) { return get<1>(x) > get<1>(y); };
};

void printLnData(vector<pair<int, string>> data, string delim = "", ostream& stream = cout, int i = -1, int j = 0) {
    if (j == 1) stream << "Fano:\n";
    else if (j == 2) stream << "Huffman:\n";
    for (auto pair: data) {
        if (i == -1) stream << "(" << pair.first << ":" << pair.second.substr(0) << ") " << delim;
        else stream << "(" << pair.first << ":" << pair.second.substr(0, i) << ") " << delim;
    }
    if (&stream != &cout) stream << (data[0].first / (double)data[1].first) << "\n\n";
    else stream << '\n';
//    cout << '\n';
}

int seqLength(vector<pair<int, string>> data, vector<vector<int>> freqDict) {
    int res = 0;
    int i = 0;

    for_each(data.begin(), data.end(), [&res, &i, freqDict](auto x){
        res += x.second.size() * freqDict[i][1];
        ++i;
    });

    return res;
}

typedef vector<pair<int, string>> (*encodingFunc)(const vector<vector<int>> &, int);

template<typename T>
void printLn(T b, T e) {
    auto beg = b;
    while (beg != e)
        cout << *beg++ << ' ';

    cout << '\n';
}

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string hexStr(unsigned char data)
{
    std::string s(2, ' ');
    s[0] = hexmap[(data & 0xF0) >> 4];
    s[1] = hexmap[data & 0x0F];
    return s;
}

std::string hexStr(unsigned char *data, int len)
{
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

string copyBMPHeader(const string & name) {
    ifstream inputFile(name, std::ios_base::binary);
    inputFile.seekg(10, ios::beg);
    vector<unsigned char> lenData(1);
    inputFile.read(reinterpret_cast<char *>(lenData.data()), 1);
    inputFile.seekg(0, ios::beg);
    int length = +lenData[0];
//    cout << +lenData[0] << endl;

    vector<unsigned char> buffer(length);
    inputFile.read(reinterpret_cast<char*>(buffer.data()), length);

    string res;
    int count = 0;
    for (auto x: buffer) {
        res += bitset<8>(x).to_string();
//        res += hexStr(x) + ' ';
//        if (count % 2) res += ' ';
//        if (count % 16 == 15) res += '\n';
//        ++count;
    }
    return res;
//    ofstream newFile(newName);
//    for (auto x: buffer)
//        newFile << x;
}

string toHexView(const string & data) {
    string res = "";
    int count = 0;
    for (auto x: data) {
        res += hexStr(x);
        if (count % 2) res += ' ';
        if (count % 16 == 15) res += '\n';
        ++count;
    }

    return res;
}

void printLnMaxElem(const vector<pair<int, string>> & data) {
    cout << std::max_element(data.begin(), data.end(), [](auto x, auto y){ return (x.second).size() < (y.second).size(); }) -> second << '\n';
}

string pixelsToString(string name, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);

    int width = img.width();
    int height = img.height();
    string str = "";
    for (auto pixel: pixels) {
        if (i != -1) {
            str += bitset<8>(pixel[i]).to_string();
        }
        else {
            for (int j = 0; j < 3; ++j) {
                str += bitset<8>(pixel[j]).to_string();
            }
        }
    }

    return str;
}

string pixelsToTxt(string name, int i = -1) {
    CImg<unsigned char> img(name.c_str());
    auto pixels = getImagePixels(name);

    int width = img.width();
    int height = img.height();
    string str = "";
    for (auto pixel: pixels) {
        if (i != -1) {
            str += bitset<8>(pixel[i]).to_string();
        }
        else {
            for (int j = 0; j < 3; ++j) {
                str += bitset<8>(pixel[j]).to_string();
            }
        }
    }
    std::filesystem::path p(name);
    string newName = p.stem().string() + "_pixels.txt";
    newName = p.remove_filename().string() + newName;
    ofstream file(newName);
    file << str;

    return newName;
}

vector<vector<int>> frequencyDict(const vector<vector<int>> & pixels, const int & color = -1) {
    map<int, int> dict;

    if (color != -1)
        for (const auto & pix: pixels) {
            dict[pix[color]]++;
        }
    else
        for (const auto & pix: pixels) {
            for (int i = 0; i < 3; i++) {
                dict[pix[i]]++;
            }
        }

    vector<vector<int>> result;
    for(auto it = dict.begin(); it != dict.end(); ++it ) { result.push_back( vector{it -> first, it -> second}); }
    sort(result.begin(), result.end(), [](auto x, auto y){ return x[1] > y[1]; });

    return result;
}

vector<vector<int>> frequencyDict(const string & name) {
    map<int, int> dict;
    ifstream file(name);
    string line = "";
    int curNum = 0;
    while (getline(file, line)) {
        for (int i = 0; i < line.size(); ++i) {
            curNum = curNum << 1 | (line[i] - '0');
            if (i % 8 == 7) {
                dict[curNum]++;
                curNum = 0;
            }
        }
    }

    vector<vector<int>> result;
    for(auto it = dict.begin(); it != dict.end(); ++it ) { result.push_back( vector{it -> first, it -> second}); }
    sort(result.begin(), result.end(), [](auto x, auto y){ return x[1] > y[1]; });

    return result;
}

unsigned char toByte(const string & str) {
    unsigned char res = 0;
    for (auto x: str) res = (res << 1) | (x - '0');
    return res;
}

void saveBMPFile(const string & name, const string & data, int w, int h) {
    ofstream file(name);
    CImg<unsigned char> img(h, w, 1, 3);

    int ind = 0;
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++) {
            for (int i = 0; i < 3; ++i) {
                img(c, r, 0, i) = toByte(data.substr(ind, 8));
                ind += 8;
            }
        }
    img.save(name.c_str());
//    int size = toByte(data.substr(10 * 8, 8));
//    int count = size % 5;
//    cout << data.size() - size + count << endl;
//    for (int i = 0; i < data.size(); i += 8) {
//        if (i <= size)
//            file << toByte(data.substr(i, 8));
//        if (i == size)
//            for (int j = 0; j < count; ++j) file << toByte(string('0', 8));
//        else if (i > size)
//        {
//            file << toByte(data.substr(data.size() + size - count - i, 8));
//        }
//        if ((i / 8) % w == w - 1) file << 0 << 0;
//    }
}

///Old comments
//template<typename T>
//void printVec(vector<T> vect) {
//    cout << '(';
//    for (auto vec: vect)
//        for (int i = 0; i < vec.size(); ++i) {
//            cout << (vec[i]) << ((i != (vec.size() - 1)) ? " " : "");
//        }
//    cout << ')';
//}
//
//template<typename it>
//void printLn(it b, it e) {
//    while (b != e) {
//        cout << "(" << (b -> first) << ' ' << (b -> second) << ") ";
//        ++b;
//    }
//    cout << '\n';
//}

//void writeDownText(string name, map<int, unsigned char> encodedData) {
//    std::ofstream output("binary.txt");
//    CImg<unsigned char> img(name.c_str());
//
//    auto b = encodedData.begin();
//    output << name << ',' << img.height() << ',' << img.width() << ':' << '\x00' <<'\n' ;
//    while (b != encodedData.end()) {
//        output << (b -> first) << ':' << (b -> second) << ' ';
//        ++b;
//    }
//    output << '\n';
//    int width = img.width();
//    int height = img.height();
//    for (int r = 0; r < height; r++)
//        for (int c = 0; c < width; c++) {
//            output << (encodedData[img(c, r, 0, 0)]);
//        }
//    output << '\0';
//}


//template<typename T>
//unsigned char toUnsignedChar(T b, T e) {
//    unsigned char res = 0;
//    while (b != e) {
//        res = *b | res << 1;
//        ++b;
//    }
//    return res;
//}


//vector<pair<int, string>> compareFano(string name, int i = -1) {
//    CImg<unsigned char> img(name.c_str());
//    auto pixels = getImagePixels(name);
//    vector<vector<int>> dict;
//    dict = frequencyDict(pixels, i);
//
//    auto encodedData = toFano(dict, (i == -1 ? pixels.size() * 3 : pixels.size()));
//    auto data = toMap(encodedData);
//
//    string str = "";
//    string otherStr = "";
//    int width = img.width();
//    int height = img.height();
//    for (int r = 0; r < height; r++)
//        for (int c = 0; c < width; c++) {
//            if (i != -1) {
//                str += data[img(c, r, 0, i)];
//                otherStr += bitset<8>((img(c, r, 0, i))).to_string();
//            }
//            else
//                for (int j = 0; j < 3; ++j) {
//                    str += data[img(c, r, 0, j)];
//                    otherStr += bitset<8>((img(c, r, 0, j))).to_string();
//                }
//        }
//
//    vector<pair<int, string>> res;
//    res = { {str.size(), str}, {otherStr.size(), otherStr} };
//    return res;
//}
//
//vector<pair<int, string>> compareHuffman(string name, int i = -1) {
//    CImg<unsigned char> img(name.c_str());
//    auto pixels = getImagePixels(name);
//    vector<vector<int>> dict;
//    dict = frequencyDict(pixels, i);
//
//    auto encodedData = toHuffman(dict, (i == -1 ? pixels.size() * 3 : pixels.size()));
//    auto data = toMap(encodedData);
//
//    string str = "";
//    string otherStr = "";
//    int width = img.width();
//    int height = img.height();
//    for (int r = 0; r < height; r++)
//        for (int c = 0; c < width; c++) {
//            if (i != -1) {
//                str += data[img(c, r, 0, i)];
//                otherStr += bitset<8>((img(c, r, 0, i))).to_string();
//            }
//            else
//                for (int j = 0; j < 3; ++j) {
//                    str += data[img(c, r, 0, j)];
//                    otherStr += bitset<8>((img(c, r, 0, j))).to_string();
//                }
//        }
//
//    vector<pair<int, string>> res;
//    res = { {str.size(), str}, {otherStr.size(), otherStr} };
//    return res;
//}