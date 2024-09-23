#pragma once

#include <initializer_list>
#include <iterator>
#include <stack>
#include <queue>

#include <iostream>
#include <string>
#include "Nodes.h"
//#include "Set.h"

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::queue;
using std::string;



template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>> // , class Compare = std::less<T>, class Allocator = std::allocator<T>
class BinarySearchTree {
private:
    TreeNode<T> * n;
    size_t _size;
    Compare cmp = Compare();

    inline TreeNode<T> * createFict()
    {
        TreeNode<T> * newNode = alc.allocate(1);
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> p));
        newNode -> p = newNode;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> l));
        newNode -> l = newNode;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> r));
        newNode -> r = newNode;

        return newNode;
    }

    inline TreeNode<T> * createNode(T&& elem, TreeNode<T> * parent = nullptr, TreeNode<T> * left = nullptr, TreeNode<T>* right = nullptr)
    {
        TreeNode<T> * newNode = alc.allocate(1);
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> p));
        newNode -> p = parent;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> l));
        newNode -> l = left;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> r));
        newNode -> r = right;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> data), std::move(elem));

        return newNode;
    }

    inline TreeNode<T> * createNode(const T & elem, TreeNode<T> * parent = nullptr, TreeNode<T> * left = nullptr, TreeNode<T>* right = nullptr)
    {
        TreeNode<T> * newNode = alc.allocate(1);
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> p));
        newNode -> p = parent;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> l));
        newNode -> l = left;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> r));
        newNode -> r = right;
        std::allocator_traits<AllocType>::construct(alc, &(newNode -> data), elem);

        return newNode;
    }

    // Удаление фиктивной вершины
    inline void deleteFict(TreeNode<T> * node) {
        std::allocator_traits<AllocType>::destroy(alc, &(node -> p));
        std::allocator_traits<AllocType>::destroy(alc, &(node -> l));
        std::allocator_traits<AllocType>::destroy(alc, &(node -> r));
        std::allocator_traits<AllocType>::deallocate(alc, node, 1);
    }

    // Удаление вершины дерева
    inline void deleteNode(TreeNode<T> * node) {
        //  Тут удаляем поле данных (вызывается деструктор), а остальное удаляем так же, как и фиктивную
        std::allocator_traits<AllocType>::destroy(alc, &(node -> data));
        deleteFict(node);
    }

    void freeNodes(TreeNode<T> * node)
    {
        if (node == n) return;

        freeNodes(node -> l);
        freeNodes(node -> r);
        deleteNode(node);
    }

public:
    template<typename type, class Compar, class Allocato>
    friend class Set;
    using AllocType = typename std::allocator_traits<Allocator>::template rebind_alloc <TreeNode<T>>;
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    template<typename Pointer, typename Reference>
    class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t, Pointer, Reference>
    {
    private:
        TreeNode<T> * node;

    public:
        template<typename type, class Compar, class Allocato>
        friend class Set;
        friend class BinarySearchTree;
        BaseIterator(): node() {};
        BaseIterator(TreeNode<T> * n): node(n) {};
        bool operator==(const BaseIterator& other) { return node == other.node;}
        bool operator!=(const BaseIterator& other) { return !operator==(other); };
        pointer operator->() { return &(node -> data); };
        BaseIterator& operator--() {
            if (node -> l -> p != node && node -> r -> p != node && node -> p -> p == nullptr && node -> l -> l == node) node = node -> r; // && (node -> r -> p != node || node -> l -> p != node)
            else if (node -> l -> p -> p == nullptr && node -> l -> l == node) node = node -> l;
            else if (node == node -> l -> p) {
                node = node -> l;
                while (node == node -> r -> p) node = node -> r;
            }
            else {
                while (node -> p -> r != node) node = node -> p;
                node = node -> p;
            }

            return *this;
        };
        BaseIterator operator--(int) {
            auto it = *this;
            if (node -> l -> p != node && node -> r -> p != node && node -> p -> p == nullptr && node -> l -> l == node) node = node -> r;
            else if (node -> l -> p -> p == nullptr && node -> l -> l == node) node = node -> l;
            else if (node == node -> l -> p) {
                node = node -> l;
                while (node == node -> r -> p) node = node -> r;
            }
            else {
                while (node -> p -> r != node) node = node -> p;
                node = node -> p;
            }

            return it;
        };
        BaseIterator& operator++() {
            //if ((node -> p -> p == nullptr) && (node -> p -> l != node || node -> p -> r != node)) node = node -> l;
            if (node -> r -> p -> p == nullptr && node -> r -> r == node) node = node -> r;
            else if (node == node -> r -> p) {
                node = node -> r;
                while (node == node -> l -> p) node = node -> l;
            }
            else {
                while (node -> p -> l != node) node = node -> p;
                node = node -> p;
            }

            return *this;
        };
        BaseIterator operator++(int) {
            auto it = *this;
            //if (node -> p -> p == nullptr) node = node -> l;
            if (node -> r -> p -> p == nullptr && node -> r -> r == node) node = node -> r;
            else if (node == node -> r -> p) { //node != node -> r -> p && node != node -> l -> p
                node = node -> r;
                while (node == node -> l -> p) node = node -> l;
            }
            else {
                while (node -> p -> l != node) node = node -> p;
                node = node -> p;
            }

            return it;
        };
        Reference operator*() { return node -> data; };

//        int operator-(BaseIterator it) {
//            auto cur = *this;
//            int res = 0;
//            while (cur++ != it) res++;
//            return res;
//        }
//
//        BaseIterator operator+(int count) {
//            for (int i = 0; i < count; ++i) this->operator++();
//            return *this;
//        }
//        BaseIterator operator-(difference_type count) {
//            for (std::ptrdiff_t i = count; i > 0; --i) this->operator--();
//            return *this;
//        }
//        BaseIterator operator-(int count) {
//            for (int i = 0; i < count; ++i) this->operator--();
//            return *this;
//        }
    };
    using key_type = T;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = AllocType;
    using iterator               = BaseIterator<pointer, reference>;
    using const_iterator         = BaseIterator<const_pointer, const_reference>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    iterator insert(TreeNode<T> * node) {
        bool onlyLeft = true;
        bool onlyRight = true;
        auto nextNode = node;
        if (!_size) {
            n -> p = nextNode;
            //nextNode -> p = n;
        }
        else {
            auto curHead = n -> p;
            while (curHead) {
                if (cmp(node -> data, curHead -> data)) { //node -> data < curHead -> data
                    onlyRight = false;
                    if (curHead -> l != n) curHead = curHead -> l;
                    else {
                        curHead -> l = nextNode;
                        nextNode -> p = curHead;
                        break;
                    }
                }
                else {
                    onlyLeft = false;
                    if (curHead -> r != n) curHead = curHead -> r;
                    else {
                        curHead -> r = nextNode;
                        nextNode -> p = curHead;
                        break;
                    }
                }
            }
            nextNode -> p = curHead;
        }

        nextNode -> l = n;
        nextNode -> r = n;
        if (onlyLeft) n -> l = nextNode;
        if (onlyRight) n -> r = nextNode;
        _size++;
        return iterator(nextNode);
    };

//    template<typename InputIt>
//    iterator insert(InputIt pos, const T & elem) {
//        auto node = pos.node;
//        T data = node -> data;
//        node -> data = elem;
//
//        TreeNode<T> * nextNode = createNode(elem);
//        return insert(nextNode);
//    };

public:
    BinarySearchTree(Compare comp = Compare(), AllocType alloc = AllocType()): cmp(comp), alc(alloc) {
        n = createFict();
        //n = static_cast<TreeNode<T> *>(malloc(sizeof(TreeNode<T>)));
        _size = 0;
    };

    BinarySearchTree(std::initializer_list<T> init, Compare comp = Compare(), AllocType alloc = AllocType()): BinarySearchTree(comp, alloc) {
        for (const T & x: init) insert(x);
    };

    BinarySearchTree(BinarySearchTree && other): n(other.n) ,_size(other._size) {
        cmp = other.cmp;
        alc = other.alc;
        other.n = nullptr;
        other._size = 0;
    }
    template <class InputIterator>
    BinarySearchTree(InputIterator first, InputIterator last, Compare comparator = Compare(), AllocType alloc = AllocType()): BinarySearchTree(comparator, alloc)
    {
        auto b = first;
        while (b != last) {
            insert(*b++);
        }
//        std::for_each(first, last, [this](T x) {
//            insert(x);
//        });
    }


private:
    AllocType alc;

    //  Рекурсивное копирование дерева
    TreeNode<T> * recur_copy_tree(TreeNode<T> * source, const TreeNode<T> * source_dummy)
    {
        //  Сначала создаём дочерние поддеревья
        TreeNode<T> * left_sub_tree;
        if (source -> l != source_dummy)
            left_sub_tree = recur_copy_tree(source->l, source_dummy);
        else
            left_sub_tree = n;

        TreeNode<T> * right_sub_tree;
        if (source->r != source_dummy)
            right_sub_tree = recur_copy_tree(source->r, source_dummy);
        else
            right_sub_tree = n;

        //  Теперь создаём собственный узел
        TreeNode<T> * current = createNode(source->data, nullptr, left_sub_tree, right_sub_tree);
        //  Устанавливаем родителей
        if (source->r != source_dummy)
            current->r->p = current;
        if (source->l != source_dummy)
            current->l->p = current;
        //  Ну и всё, можно возвращать
        return current;
    };

public:
    BinarySearchTree(const BinarySearchTree<T> & bst, Compare comp = Compare(), AllocType alloc = AllocType()): BinarySearchTree(comp, alloc) {
        _size = bst._size;
        if (bst.empty()) return;

        n -> p = recur_copy_tree(bst.n -> p, bst.n);
        n -> p -> p = nullptr;

        //  Осталось установить min и max
        TreeNode<T> * curNode = n -> p;
        while (curNode -> l != n) curNode = curNode -> l;
        n -> l = curNode;
        curNode = n -> p;
        while (curNode -> r != n) curNode = curNode -> r;
        n -> r = curNode;
    };

    AllocType get_allocator() const noexcept { return alc; }
    key_compare key_comp() const noexcept { return cmp; }
    value_compare value_comp() const noexcept { return cmp; }

    iterator begin() noexcept { return iterator(n -> l); };
    const_iterator begin() const noexcept { return const_iterator(n -> l); };
    const_iterator cbegin() const noexcept { return const_iterator(n -> l); };
    iterator end() { return iterator(n); };
    const_iterator end() const noexcept { return const_iterator(n); };
    const_iterator cend() const noexcept { return const_iterator(n); };

    reverse_iterator rbegin() noexcept { return reverse_iterator(n); };
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(n); };
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(n); };
    reverse_iterator rend() noexcept { return reverse_iterator(n -> l); };
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(n -> l); };
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(n -> l); };

    bool operator==(const BinarySearchTree<T> & other) {
        if (size() != other.size()) return false;
        auto b = begin();
        auto otherB = other.begin();
        while (b != end()) {
            if (*b++ != *otherB++) return false;
        }
        return true;
    };

    bool isEqual(const BinarySearchTree<T> & other) {
        return isEqual(n -> p, other.n -> p);
    }

    bool isEqual(TreeNode<T> * one, TreeNode<T> * other) {
        if (one -> p == one && other -> p == other) return true;
        else if ((one -> l -> p != one && one -> r -> p != one && one -> p -> p == nullptr) &&
            (other -> l -> p != other && other -> r -> p != other && other -> p -> p == nullptr)) return true;
        else
            return one -> data == other -> data && isEqual(one -> l,other -> l) && isEqual(one -> r, other -> r);
    };

    iterator find(const T & data) const {
        TreeNode<T> * curNode = n -> p;
        while (curNode != n) {
            if (cmp(curNode -> data, data)) curNode = curNode -> r;
            else if (cmp(data, curNode -> data)) curNode = curNode -> l;
            else return iterator(curNode);
        }
        return iterator(curNode);
    };

    iterator lower_bound(const value_type & data) const {
        TreeNode<T> * curNode = n -> p;
        TreeNode<T> * result = n -> l;
        while (curNode != n) {
            if (cmp(data, curNode -> data)) {
                if (curNode -> l == n) break;
                curNode = curNode -> l;
            }
            else {
                if (curNode -> r == n) break;
                curNode = curNode -> r;
            }
            if (cmp(result -> data, curNode -> data) && cmp(curNode -> data, data)) result = curNode;
        }

        return iterator(result);
    };

    iterator lower_bound(iterator it, const value_type& data) {
        iterator b = it();
        while (b != end()) {
            if (*b > data) return --b;
            ++b;
        }

        return end();
    };

    iterator upper_bound(const value_type& data) const {
        TreeNode<T> * curNode = n -> p;
        TreeNode<T> * result = n -> r;
        while (curNode != n) {
            if (cmp(data, curNode -> data)) {
                if (curNode -> l == n) break;
                curNode = curNode -> l;
            }
            else {
                if (curNode -> r == n) break;
                curNode = curNode -> r;
            }
            if (cmp(curNode -> data, result -> data) && cmp(data, curNode -> data)) result = curNode;
        }

        return iterator(result);
    };

    iterator upper_bound(iterator it, const value_type& data) {
        iterator b = it;
        while (b != end()) {
            if (*b >= data) return b;
            b++;
        }

        return end();
    };

    bool empty() const { return _size == 0; };

    void erase(const T & elem) {
        auto nodeIt = find(elem);
        if (nodeIt == end()) return;
        auto node = nodeIt.node;
        auto nextNodeIt = std::next(nodeIt);
        auto nextNode = nextNodeIt.node;
        //if (nextNode == n) { nextNode = std::prev(nodeIt); }
        if (nextNodeIt != end()) {
            if (nextNode -> p -> l == nextNode) nextNode -> p -> l = (nextNode -> p == n ? n : nextNode -> r);
            nextNode -> p = node -> p;
            nextNode -> l = node -> l;
            node -> l -> p = nextNode;

            if (nextNode -> p == nullptr) n -> p = nextNode;
        }
        else {
            if (_size == 1) {
                n -> p = n;
                n -> l = n;
                n -> r = n;
            }
            else {
                auto prevNode = std::prev(nodeIt).node;
                node -> p -> r = node -> l;
                node -> l -> p = node -> p;
                n -> r = prevNode;
            }
        }
        --_size;
        deleteNode(node);
    };

    void erase(iterator elem) {
        auto nodeIt = elem;
        if (nodeIt == end()) return;
        auto node = nodeIt.node;
        auto nextNodeIt = std::next(nodeIt);
        auto nextNode = nextNodeIt.node;
        //if (nextNode == n) { nextNode = std::prev(nodeIt); }
        if (nextNodeIt != end()) {
            if (nextNode -> p -> l == nextNode) nextNode -> p -> l = (nextNode -> p == n ? n : nextNode -> r);
            nextNode -> p = node -> p;
            nextNode -> l = node -> l;
            node -> l -> p = nextNode;

            if (nextNode -> p == nullptr) n -> p = nextNode;
        }
        else {
            if (_size == 1) {
                n -> p = n;
                n -> l = n;
                n -> r = n;
            }
            else {
                auto prevNode = std::prev(nodeIt).node;
                if (node != n -> p) {
                    node -> p -> r = node -> l;
                    if (node -> l -> p -> p != nullptr) {
                        node -> l -> p = node -> p;
                    }
                }
                else {
                    node -> l -> p = nullptr;
                    n -> p = node -> l;
                }
                n -> r = prevNode;
            }
        }
        --_size;
        deleteNode(node);
    };

    iterator insert(const T & elem) {
        TreeNode<T> * nextNode = createNode(elem);
        return insert(nextNode);
    };

    template<typename inputIt>
    void insert(inputIt b, inputIt e) {
        while (b++ != e)
            insert(*b);
    };

    iterator insert(T && elem) {
        TreeNode<T> * nextNode = createNode(std::move(elem)); // new TreeNode(std::move(elem))
        return insert(nextNode);
    };

    void print(TreeNode<T> * node) {
        if (node == n) return;
        print(node -> l);
        cout << node -> data << ' ';
        print(node -> r);
    };

    void printLnInfix() {
        print(n -> p);
        cout << '\n';
    };

    void printLnBreadth() {
        int count = 0;
        queue<TreeNode<T> *> queue;
        std::queue<TreeNode<T> *> queue2;
        queue.push(n -> p);
        while (!queue.empty()) {
            TreeNode<T> * node = queue.front();
            TreeNode<T> * parent;
            queue.pop();
            if (node != n) {
                queue2.push(node);
                queue.push(node -> l);
                queue.push(node -> r);
            }
        }

        while (!queue2.empty()) {
            cout << queue2.front() -> data << ' ';
            queue2.pop();
        }
        cout << '\n';
    };

    void printLnRRL() {
        stack<TreeNode<T> *> stack;
        stack.push(n -> p);
        while (!stack.empty()) {
            TreeNode<T> * node = stack.top();
            stack.pop();
            if (stack.size() == 10)
                10;
            if (node != n) {
                stack.push(node -> l);
                stack.push(node);
                stack.push(node -> r);
            }
            else {
                if (stack.size() == 0) break;
                while (stack.top() == n) stack.pop();
                cout << stack.top() -> data << ' ';
                stack.pop();
            }
        }
        cout << '\n';
    };
    BinarySearchTree & operator=(const BinarySearchTree & tree)
    {
        if (this == &tree) return *this;
        BinarySearchTree tmp{tree};

        std::swap(n, tmp.n);
        std::swap(_size, tmp._size);

        return *this;
    }

    BinarySearchTree & operator=(BinarySearchTree && tree)
    {
        if (this == &tree) return *this;
        std::swap(n, tree.n);
        std::swap(_size, tree._size);

        return *this;
    }

    void clear() {
        freeNodes(n -> p);
        _size = 0;
        n -> p = n -> l = n -> r = n;
    }

    T min() { return n -> l -> data; };

    T max() { return n -> r -> data; };

    size_t size() const { return _size; };

    ~BinarySearchTree() {
        clear();
        deleteFict(n);
    }

protected:

};

template <class Key, class Compare, class Allocator>
bool operator==(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {
    typename BinarySearchTree<Key, Compare, Allocator>::const_iterator it1{ x.begin() }, it2{ y.begin() };
    while (it1 != x.end() && it2 != y.end() && *it1 == *it2) {
        ++it1; ++it2;
    }

    return it1 == x.end() && it2 == y.end();
}

template <class Key, class Compare, class Allocator>
bool operator<(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {

    typename BinarySearchTree<Key, Compare, Allocator>::const_iterator it1{ x.begin() }, it2{ y.begin() };
    while (it1 != x.end() && it2 != y.end() && *it1 == *it2) {
        ++it1; ++it2;
    }

    if (it1 == x.end())
        return it2 != y.end();

    return it2 != y.end() && *it1 < *it2;
}

template <class Key, class Compare, class Allocator>
bool operator!=(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {
    return !(x == y);
}

template <class Key, class Compare, class Allocator>
bool operator>(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {
    return y < x;
}

template <class Key, class Compare, class Allocator>
bool operator>=(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {
    return !(x<y);
}

template <class Key, class Compare, class Allocator>
bool operator<=(const BinarySearchTree<Key, Compare, Allocator>& x, const BinarySearchTree<Key, Compare, Allocator>& y) {
    return   !(y < x);
}