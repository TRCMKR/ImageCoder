#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <filesystem>
#include <iostream>
#include <string>
#include <array>
#include <ostream>

#include "suppFuncs.h"
#include "suppClasses.h"

#include "../CImg-3.3.5/CImg.h"
#include "NTL/ALL_FEATURES.h"
#include "NTL/mat_GF2.h"

using namespace NTL;
using matrix = my_mat_GF2;

vector<matrix> toCheckVector(const vector<matrix> & vec) {
    vector<matrix> res;
    vector<matrix> vecCopy = vec;
    for (int i = 0; i < vecCopy.size(); ++i) {
        NTL::gauss(vecCopy[i]);
        bool flag = false;
        for (int j = 0; j < vec[i].NumRows(); ++j) {
            if (NTL::IsZero(vecCopy[i](j, j))) {
                flag = true;
                break;
            }
        }
        if (flag) continue;
        toIdentQ(vecCopy[i]);
        res.push_back(vecCopy[i]);
    }
    int rows = vecCopy[0].NumRows();
    int count = vecCopy[0].NumCols() - rows;
    auto t = nOnes(rows, count, count + rows);
    for (int i = 0; i < res.size(); ++i) {
        matrix tmp;
        tmp = transpose(projectMatr(res[i], t));
        tmp = concatMatr(tmp, ident_mat_GF2(count));
        res[i] = tmp;
    }

    return res;
}

matrix ISD(const matrix & z, const matrix & G, int t, int iter) {
    matrix res;
    vector<vector<int>> prevTau;
    for (int i = 0; i < iter; ++i) {
        vec_GF2 tau = random_vec_GF2(G.NumCols());
        while (weight(tau) != 5) tau = random_vec_GF2(G.NumCols());
        auto tmp = projectMatr(G, toVector(tau));
        while (std::find(prevTau.begin(), prevTau.end(), toVector(tau)) != prevTau.end()
               || gauss(tmp) != G.NumRows()) {
            tau = random_vec_GF2(G.NumCols());
            while (weight(tau) != 5) tau = random_vec_GF2(G.NumCols());
            tmp = projectMatr(G, toVector(tau));
//            cout << determinant(tmp) << endl;
        }
        auto mH = projectMatr(z, toVector(tau)) * inv(projectMatr(G, toVector(tau)));
        auto cH = mH * G;
        if (minDistance(z + cH) <= t) return mH;
        prevTau.push_back(toVector(tau));
    }
//    cout << "Ошибка декодирования ISD!" << endl;
    return res;
}

NTL::vec_vec_GF2 generateCode(int n, int k, int d) {
    NTL::vec_vec_GF2 res;
//    auto tmp = NTL::random_GF2(n - k);
//    while (NTL::weight(tmp) < d) tmp = NTL::random_vec_GF2(n - k);
    matrix tmp = NTL::random_mat_GF2(k, n - k);
    tmp = concatMatr(NTL::ident_mat_GF2(k), tmp);
    auto tmpLinearSpan = NTL::to_mat_GF2(linearSpan(toVect(tmp)));
    while (minDistance(tmpLinearSpan) < d) {
        tmp = NTL::random_mat_GF2(k, n - k);
        tmp = concatMatr(NTL::ident_mat_GF2(k), tmp);
        tmpLinearSpan = NTL::to_mat_GF2(linearSpan(toVect(tmp)));
    }

    return toVect(tmpLinearSpan);
}

NTL::vec_vec_GF2 findBasis(const matrix & mat, int n = 5, int count = 1) {
    NTL::vec_vec_GF2 res;
//    vec.SetLength(n * count);
    int ind = 0;
    while (weight(mat(ind)) == 0) ++ind;
    NTL::vec_vec_GF2 tmp;
    tmp.append(mat(ind));
    int curCount = 0;
    for (int i = ind + 1; i < mat.NumRows(); ++i) {
        auto curSpan = linearSpan(tmp.begin(), tmp.end());
        if (std::find(curSpan.begin(), curSpan.end(), mat(i)) == curSpan.end()) {
            tmp.append(mat(i));
            if (tmp.length() % n == 0) {
                for (const auto & x: tmp) res.append(x);
                tmp.kill();
                ++curCount;
                i = ind + curCount;
                while (weight(mat(i)) == 0) ++i;
                tmp.append(mat(i));
            }
            if (curCount >= count) break;
        }
    }
    return res;
}

vector<matrix> generateBasis(int count, int n, int m, int minD = 1) {
    vector<matrix> res(count);
    for (int i = 0; i < count; ++i) {
        matrix tmp = concatMatr(NTL::ident_mat_GF2(n), NTL::random_mat_GF2(n, m - n));
        while (minDistance(tmp) < minD &&
               std::find(res.begin(), res.end(), tmp) == res.end())
            tmp = concatMatr(NTL::ident_mat_GF2(n), NTL::random_mat_GF2(n, m - n));
        res[i] = tmp;
    }

    return res;
}

matrix stringToMatr(int n, int m, const string & data) {
    matrix res;
    res.SetDims(n, m);
    int ind = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            res(i, j) = data[ind];
            ++ind;
        }
    }

    return res;
}

string matrixToString(const matrix & mat) {
    string res;
    for (int i = 0; i < mat.NumRows(); ++i) {
        for (int j = 0; j < mat.NumCols(); ++j) {
            res += IsOne(mat(i, j)) + '0';
        }
    }
    return res;
}

matrix generateErr(int size, int count) {
    matrix res;
    res.SetDims(1, size);
    for (int i = 0; i < count; ++i) {
        res(0, rand() % size - 1) += rand() % 2;
    }

    return res;
}

string encodeData(const string & data, const matrix & G) {
    string res = "";
    int beg = 0;
    int size = G.NumRows();
    while (beg < data.size()) {
        matrix block = stringToMatr(1, size, data.substr(beg, size));
//        cout << block << endl;
        matrix err = generateErr(G.NumCols(), 2);
//        cout << block * G << endl;
//        cout << block * G + err << endl;
        res += matrixToString(block * G);
        beg += size;
    }
    return res;
}

string corruptData(const string & data, int blockSize) {
    string res = "";
    int beg = 0;
    while (beg < data.size()) {
        matrix block = stringToMatr(1, blockSize, data.substr(beg, blockSize));
//        cout << block << endl;
        matrix err = generateErr(blockSize, 2);
//        cout << block * G << endl;
//        cout << block * G + err << endl;
        res += matrixToString(block + err);
        beg += blockSize;
    }
    return res;
}

string decodeData(const string & data, const matrix & G, int iter) {
    string res = "";
    int beg = 0;
    int size = G.NumCols();
    while (beg < data.size()) {
        matrix block = stringToMatr(1, size, data.substr(beg, size));
//        cout << block << endl;
//        auto tmp = matrixToString(ISD(block, G, 2, iter));
        res += matrixToString(ISD(block, G, 2, iter));
        beg += size;
    }
    return res;
}

void printLnComparison(const string & data, const string & OGdata, const matrix & G, const vector<int> & iters) {
    for (int iter: iters) {
        string recoveredData = decodeData(data, G, iter);
        cout << "FOR " << iter << " ITERATIONS: " << abs(1 - (recoveredData.size() / (float)OGdata.size())) << '\n';
    }
    cout << '\n';
}

void printLnComparison(const string & data, const string & OGdata, int iter) {
    cout << "FOR " << iter << " ITERATIONS: " << abs(1 - (data.size() / (float)OGdata.size())) << '\n';
    cout << '\n';
}

void printComparison(const string & data, const string & OGdata, int iter) {
    cout << "FOR " << iter << " ITERATIONS: " << abs(1 - (data.size() / (float)OGdata.size()))  << '\n';
}