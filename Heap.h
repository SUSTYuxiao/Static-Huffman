//
// Created by 张鹏霄 on 2018/3/16.
//

//堆的模拟实现
#include <iostream>
#include <vector>

using namespace std;

template <class T, class comPare>
class Heap
{
    public:
    Heap(){};
    Heap(T* a, int n)
    {
        _a.reserve(n);//开空间，改进性能

        //push数据
        for (int i = 0; i < n; ++i)
        {
            _a.push_back(a[i]);
        }

        //向下调整
        for (int j = (_a.size() - 2)/2; j > -1; j--)//从后向前调整
        {
            Down(j);
        }
    }

    void push(const T &a)
    {
        _a.push_back(a);
        Up(_a.size() - 1);
    }

    void pop(const T &a)
    {
        swap(_a[0], _a[_a.size() - 1]);
        _a.pop_back();
        Down(0);
    }

    void Down(int root)//向下调整思想（建堆）
    {
        comPare c;
        int parent = root;
        int chirld = parent * 2 + 1;

        while (chirld < _a.size())
        {
            if (chirld+1 <_a.size()
                &&c(_a[chirld+1] , _a[chirld]))//找到较大的孩子
            {
                chirld++;
            }

            if (c(_a[chirld] , _a[parent]))//如果不符合 大/小堆 则进行交换
            {
                swap(_a[parent], _a[chirld]);

                //子树有可能不再符合 大/小堆，继续调整
                parent = chirld;
                chirld = parent * 2 + 1;
            }
            else
            {
                break;//不需要调整
            }

        }
    }

    void Up(int chirld)
    {
        comPare c;
        int parent = (chirld - 1) / 2;

        while (chirld > 0)
        {
            if (c(_a[chirld] , _a[parent]))
            {
                swap(_a[chirld], _a[parent]);

                chirld = parent;
                parent = (chirld - 1) / 2;
            }
            else
            {
                break;//不需要调整
            }
        }
    }

    T& Top()
    {
//        if(_a.size() == 0)
//        {
//            return NULL;
//        }

        return _a[0];
    }

    int size()
    {
        return _a.size();
    }
    private:
    vector<T> _a;
};

template <class T>//仿函数 大堆
class BigHeap
{
    public:
    bool operator()(const T &left, const T &right)
    {
        return left > right;
    }
};


template <class T>//仿函数 小堆
class SmallHeap
{
    public:
    bool operator()(const T &left, const T &right)
    {
        return left < right;
    }
};


//main
//int main(void)
//{
//    int data[10]={10,16,18,12,11,13,15,17,14,19};
//    Heap<int, BigHeap<int>> h(data,10);
//
//    h.push(200);
//
//}
//return 0;
//}