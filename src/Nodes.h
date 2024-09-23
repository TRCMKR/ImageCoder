#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <array>

using std::cin;
using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::string;
using std::tuple;

template<typename T>
struct TreeNode {
    T data;
    TreeNode<T> * l;
    TreeNode<T> * r;
    TreeNode<T> * p;
    TreeNode() { std::cout << "Node created.\n"; };
    TreeNode(const T & d) { data = d; }
    TreeNode(const TreeNode<T> & n) { data = n.data; l = n.l; r = n.r; p = n.p; };
    TreeNode(const T & d, TreeNode * left, TreeNode * right, TreeNode * parent) { data = d; l = left; r = right; p = parent; };
    TreeNode(T&& d): data(std::move(d)) { };
    TreeNode(T&& d, TreeNode * left, TreeNode * right, TreeNode * parent): data(std::move(d)), l(left), r(right), p(parent) { };
    bool operator<(const TreeNode & other) const { return data < other.data; }
    TreeNode<T> & operator=(TreeNode<T> && other) { std::swap(data, other.data); std::swap(l, other.l); std::swap(r, other.r); std::swap(p, other.p); };
};

template<typename T>
struct Node {
    T data;
    Node<T> * next;
    Node() { std::cout << "Node created.\n"; };
    Node(const T & d) { data = d; }
    Node(const Node<T> & n) { data = n.data; next = n.next; };
    Node(const T & d, Node * n) { data = d; next = n; };
    Node(T&& d): data(std::move(d)), next(nullptr) { };
    Node(T&& d, Node * n): data(std::move(d)), next(n) { };
    Node(Node && n): data(std::move(n.data)) {std::swap(next, n.next); };
    bool operator<(const Node & other) { return data < other.data; }
};

template<typename T>
struct HaffmanTreeNode {
    T data;
    HaffmanTreeNode<T> * l;
    HaffmanTreeNode<T> * r;
    HaffmanTreeNode<T> * p;
//    HaffmanTreeNode(const T & d) { data = d; }
    HaffmanTreeNode(const HaffmanTreeNode<T> & n) { data = n.data; l = n.l; r = n.r; p = n.p; };
    HaffmanTreeNode(const T & d, HaffmanTreeNode * left = nullptr, HaffmanTreeNode * right = nullptr, HaffmanTreeNode * parent = nullptr) { data = d; l = left; r = right; p = parent; };
};