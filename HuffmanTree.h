//
// Created by 张鹏霄 on 2018/3/18.
//

#ifndef MYCODWAREHOUSE_HUFFMANTREE_H
#define MYCODWAREHOUSE_HUFFMANTREE_H

#include "Heap.h"

template <class W>
struct HuffmanTreeNode
{
    W _w;
    HuffmanTreeNode<W> *_left;
    HuffmanTreeNode<W> *_right;

    HuffmanTreeNode(const W &w)
            :_left(NULL)
            ,_right(NULL)
            ,_w(w)
    {}
};

template <class T>
class HuffmanTree
{
    public:
    typedef HuffmanTreeNode<T> Node;
    struct compare
    {
        bool operator()(Node *left, Node *right)
        {
            return left->_w < right->_w;
        }
    };

    HuffmanTree(T* arr, int n, T& invaild)
    {
        if(!arr || n==1)
        {
            return ;
        }

        Heap<Node*, compare> h;

        for (int i = 0; i < n; ++i)
        {
            if (arr[i] != invaild)
                h.push(new Node(arr[i]));
        }

        while(h.size() > 1)
        {
            Node *left = h.Top();
            h.pop(0);
            Node *right = h.Top();
            h.pop(0);

            Node *parent = new Node(left->_w + right->_w);
            parent->_left = left;
            parent->_right = right;
            h.push(parent);
        }
        _root = h.Top();
    }

    Node *getRoot()
    {
        return _root;
    }

    private:
    Node* _root;
};
#endif //MYCODWAREHOUSE_HUFFMANTREE_H
