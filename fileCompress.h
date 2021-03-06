//
// Created by 张鹏霄 on 2018/3/18.
//

#ifndef MYCODWAREHOUSE_FILECOMPRESS_H
#define MYCODWAREHOUSE_FILECOMPRESS_H

#include "HuffmanTree.h"
#include <cassert>
#include "sys/stat.h"

using namespace std;
struct huffmanInfo//写入文本的配置信息，用于解压缩重建哈夫曼树
{
    unsigned char _c;
    int _weight;
    huffmanInfo(const unsigned char c, const int w):_c(c), _weight(w){};
    huffmanInfo():_c('a'), _weight(0){};
};

struct charInfo
{
    unsigned char _c;
    int _weight;
    string _code;

    charInfo operator+(const charInfo &right)
    {
        charInfo ch;
        ch._weight = _weight + right._weight;
        return ch;
    }

    bool operator<( const charInfo &right)
    {
        return _weight < right._weight;
    }

    bool operator != (const charInfo &right)
    {
        return _weight != right._weight;
    }
};

class compress
{
    public:
    compress()
    {
        for (int i = 0; i < 256; ++i)
        {
            _info[i]._c = i;
            _info[i]._weight = 0;
            _info[i]._code = "";
        }
    }

    //生成哈夫曼编码
    typedef HuffmanTreeNode<charInfo> Node;
    void creatHufmanCode(Node* root, string code)
    {
        if (!root)//判空
        {
            return;
        }

        if (!root->_right && !root->_left)//叶子结点
        {
            _info[root->_w._c]._code = code;
        }

        creatHufmanCode(root->_left, code + "0");
        creatHufmanCode(root->_right, code + "1");
    }

    //文件压缩
    void doCompress(const string &s)
    {
        //配置文件指针
        FILE *fIn = fopen("test.txt", "r");
        if (!fIn)
        {
            perror("fopen");
            return;
            //printf("%s", strerror(errno));
        }
        string fileOutName(s);
        fileOutName += ".Huffman";
        FILE *fOut = fopen(fileOutName.c_str(),"wb");


        //初始化词频信息
        unsigned char chIn;
        struct stat buff;
        stat(s.c_str(), &buff);
        long long fcount = 0;
        while (fcount != buff.st_size)
        {
            chIn = fgetc(fIn);
            _info[chIn]._weight++;
            fcount++;
        }

        //建立哈夫曼树以及哈夫曼编码
        charInfo invaild;
        invaild._weight = 0;
        HuffmanTree<charInfo> tree(_info, 256, invaild);
        creatHufmanCode(tree.getRoot(), "");

        //保存编码配置文件
        for (int j = 0; j < 256; ++j)
        {
            if (_info[j]._weight != 0)
            {
                huffmanInfo tmp(_info[j]._c, _info[j]._weight);
                fwrite(&tmp, sizeof(huffmanInfo), 1, fOut);
            }
        }
        huffmanInfo tmp;
        fwrite(&tmp, sizeof(huffmanInfo), 1, fOut);

        //输出文件
        unsigned char chOut = 0;
        int count = 0;
        fseek(fIn,0,SEEK_SET);
        fcount = 0;
        int rate_of_progress = 1;
        while (fcount != buff.st_size)
        {
            chIn = fgetc(fIn);
            string code = _info[chIn]._code;
            for (int i = 0; i < code.size(); ++i)
            {
                chOut <<= 1;
                if (code[i] == '1')
                {
                    chOut |= 1;
//                    cout << 1;
                }
                else if (code[i] == '0')
                {
                    chOut |= 0;
//                    cout << 0;
                }
                else
                {
                    assert(0);
                }
                count++;

                if (count == 8)
                {
                    fwrite(&chOut, 1, 1, fOut);
                    chOut = 0;
                    count = 0;
//                    cout << endl;
                }
            }
            fcount++;
            if (fcount == rate_of_progress * 1000000)
            {
                cout << ((double) fcount / (double) buff.st_size) * 100 << " %" << endl;
                rate_of_progress++;
            }

        }
        if (count != 0)
        {
            chOut <<= 8 - count;
            fwrite(&chOut, 1, 1, fOut);
        }
        fclose(fIn);
        fclose(fOut);
    }

    void unCompress(string &s)
    {
        //配置文件指针
        FILE *fIn = fopen(s.c_str(), "rb");
        string fileOutName(s);
        fileOutName += ".UnHuffman";
        FILE *fOut = fopen(fileOutName.c_str(), "w");
//        struct stat buff;
//        stat(s.c_str(), &buff);


        //读取配置信息
        while (1)
        {
            huffmanInfo tmp;
            fread(&tmp, sizeof(huffmanInfo), 1, fIn);
            if (tmp._weight != 0)
            {
                _info[tmp._c]._c = tmp._c;
                _info[tmp._c]._weight = tmp._weight;
            }
            else
            {
                break;
            }
        }

        //重建哈夫曼树
        charInfo invaild;
        invaild._weight = 0;
        HuffmanTree<charInfo> tree(_info, 256, invaild);

        //获取还原字符量
        long long fileSize = tree.getRoot()->_w._weight;
        long long charsCount = 0;

        //还原字符
        unsigned char chIn;
        fread(&chIn, 1, 1, fIn);
//        pnumin2(chIn);
        unsigned char chOut;
        Node *cur = tree.getRoot();
        while (1)
        {
            for (int i = 7; i >= 0 ; --i)
            {
                if (charsCount == fileSize)
                {
                    return;
                }
                if ( chIn & (1<<i))
                {
                    cur = cur->_right;
                }
                else
                {
                    cur = cur->_left;
                }

                if (!cur->_left && !cur->_right)
                {
                    fputc(cur->_w._c, fOut);
                    charsCount++;
                    cur = tree.getRoot();
//                    pnumin2(chIn);
                }
            }
            fread(&chIn, 1, 1, fIn);
        }
    }

//    void pnumin2(unsigned char num)
//    {
//
//        while (num)
//        {
//            for (int i = 7; i < 0; ++i)
//            {
//                if (num & (1 << i))
//                {
//                    printf("1");
//                }
//                else printf("0");
//            }
//        }
//        cout << endl;
//    }

    protected:
    charInfo _info[256];
};


#endif //MYCODWAREHOUSE_FILECOMPRESS_H
