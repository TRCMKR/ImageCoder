#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <initializer_list>
#include <iostream>
#include <string>
#include <array>
#include <ostream>
#include <random>
//#include "suppFuncs.h"
#include <filesystem>

#include "../CImg-3.3.5/CImg.h"
#include "NTL/ALL_FEATURES.h"
#include "NTL/mat_GF2.h"
#include "NTL/matrix.h"

struct BSC {
    double q;
    BSC(): q(0) { };
    BSC(double newQ): q(newQ) { };
    string operator()(const string & data) {
        string res;
        for (const auto & bit: data)
            res += (bit - '0') ^ ((double)rand() / RAND_MAX < q) + '0';
        return res;
    }
};

struct my_mat_GF2 : public NTL::mat_GF2
{
    my_mat_GF2(): NTL::mat_GF2() { }
    my_mat_GF2(std::initializer_list<std::initializer_list<long>> ilst) {
        int n = ilst.size();
        int m = ilst.begin()->size();
        SetDims(n, m);
        int i = 0;
        int j = 0;
        for (auto x: ilst) {
            for (auto elem: x){
                put(i, j, NTL::GF2(elem));
                j++;
            }
            i++;
            j = 0;
        }
    };

    my_mat_GF2(const NTL::mat_GF2 & other) {
        int n = other.NumRows();
        int m = other.NumCols();
        SetDims(n, m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                put(i, j, other.get(i, j));
            }
        }
    };

    template<typename it>
    my_mat_GF2(it b, it e) {
        int n = e - b;
        int m = b->length();
        SetDims(n, m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                put(i, j, b->get(j));
            }
            ++b;
        }
    };

    my_mat_GF2& operator=(const NTL::mat_GF2 & other) {
        kill();
        int n = other.NumRows();
        int m = other.NumCols();
        SetDims(n, m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                put(i, j, other.get(i, j));
            }
        }
        return *this;
    };

    using vec = NTL::Vec<NTL::GF2>;
    vec& operator()(long i) { return NTL::mat_GF2::operator()(i + 1); };
    const vec& operator()(long i) const { return NTL::mat_GF2::operator()(i + 1); };
    reference operator()(long i, long j) { return NTL::mat_GF2::operator()(i + 1, j + 1); };
    const_reference operator()(long i, long j) const { return NTL::mat_GF2::operator()(i + 1, j + 1); };
};

//struct my_mat_GF2
//{
//    NTL::mat_GF2 mat;
//    my_mat_GF2(): mat() { };
//    my_mat_GF2(initializer_list<initializer_list<long>> ilst): mat() {
//        int n = ilst.size();
//        int m = ilst.begin()->size();
//        mat.SetDims(n, m);
//        int i = 0;
//        int j = 0;
//        for (auto x: ilst) {
//            for (auto elem: x){
//                mat(i, j) = elem;
//                j++;
//            }
//            i++;
//            j = 0;
//        }
//    };
//    my_mat_GF2(NTL::mat_GF2 other) {
//        mat = other;
//    };
//
//    my_mat_GF2& operator=(const NTL::mat_GF2 & other) {
//        mat = other;
//        return *this;
//    };
//};