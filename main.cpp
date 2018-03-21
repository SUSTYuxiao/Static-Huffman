#include <cstdio>
#include <iostream>
using namespace std;

#include "fileCompress.h"
#include "sys/stat.h"

int main(void)
{
    compress com1;

    //压缩
    string str("the.mp4");
    com1.doCompress(str);

    compress com2;

//    //解压缩
//    str += ".Huffman";
//    com2.unCompress(str);


    return 0;
}