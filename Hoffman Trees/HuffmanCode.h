//
//  HuffmanCode.h
//  Trees
//
//  Created by 余宏昊 on 2018/11/30.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef HuffmanCode_h
#define HuffmanCode_h

#include <stdlib.h>
#include <stdio.h>
#include "LkList.h"

typedef struct {
    unsigned int      weight;
    unsigned int      parent,lchild,rchild;
} HTNode, HuffmanTree;      //动态分配数组存储赫夫曼树
typedef struct {
    int  taken1;
    int  taken2;
} Taken;
typedef struct {
    char   _char[10000];  //字符数组
    char * _code[10000];  //存放赫夫曼编码指针的数组
} Code;

status Select(HuffmanTree * HT, Taken * taken, int n, int no, int * s1, int * s2)
{   //在HT[1...no]选择parent为0且weight最小的两个结点，其序号分别为s1和s2
    
    //由于读入文本时已经根据频数进行了排序处理，所以只需要按顺序选择未被选择过的前两个结点，但是由于合并子树后会产生新的结点，所以还要与新产生的未被选择的结点进行比较，选出最小的两个结点
    //taken用来记录执行当前操作时最后被选择过的结点的位置
    int A_pos=taken->taken1+1; int B_pos=taken->taken1+2;
    int A_weight=HT[A_pos].weight; int B_weight=HT[B_pos].weight;
    int C_pos=taken->taken2+1; int D_pos=taken->taken2+2;
    int C_weight=HT[C_pos].weight; int D_weight=HT[D_pos].weight;
    
    //--------------一共有六种情况---------------//
    //1.A<=B<=C<=D;         *s1=A, *s2=B;
    //2.C<=D<=A<=B;         *s1=C, *s2=D;
    //3.A<=C<=D<=B;         *s1=A, *s2=C;
    //4.C<=A<=B<=D;         *s1=C, *s2=A;
    //5.A<=C<=B<=D;         *s1=A, *s2=C;
    //6.C<=A<=D<=B;         *s1=C, *s2=A;
    //----------------------------------------//
    
    if(taken->taken1+2<=n)
    {
        if(C_weight==0) {*s1=A_pos;*s2=B_pos;}
        else if(C_weight!=0&&D_weight==0)
        {
            if(B_weight<=C_weight) {*s1=A_pos;*s2=B_pos;}
            else if(A_weight<=C_weight) {*s1=A_pos;*s2=C_weight;}
            else if(A_weight>C_weight) {*s1=C_pos;*s2=A_pos;}
        }
        else if(C_weight!=0&&D_weight!=0)
        {
            if(B_weight<=C_weight) {*s1=A_pos;*s2=B_pos;}
            else if(D_weight<=A_weight) {*s1=C_pos;*s2=D_pos;}
            else if(A_weight<=C_weight) {*s1=A_pos;*s2=C_pos;}
            else if(C_weight<A_weight) {*s1=C_pos;*s2=A_pos;}
        }
    }
    else if(taken->taken1+1==n)
    {
        if(A_weight<=C_weight) {*s1=A_pos;*s2=C_pos;}
        else if(A_weight<=D_weight) {*s1=C_pos;*s2=A_pos;}
        else if(A_weight>D_weight) {*s1=C_pos;*s2=D_pos;}
    }
    else if(taken->taken1==n)
    {
        *s1=C_pos;*s2=D_pos;
    }
    
    if(*s1<*s2)  //更新taken所代表的位置
    {
        if(*s2<=n) taken->taken1=*s2;
        else if(*s1>n) taken->taken2=*s2;
        else {taken->taken1=*s1;taken->taken2=*s2;}
    }
    else
    {
        if(*s1<=n) taken->taken1=*s1;
        else if(*s2>n) taken->taken2=*s1;
        else {taken->taken1=*s2;taken->taken2=*s1;}
    }
    
    return OK;
}

HuffmanTree * HuffmanCoding(HuffmanTree * HT, MsgInfo * Info, Code * code)
{   //对文本进行赫夫曼编码
    int n=Info->numChar;    //文本中不同字符的个数
    if(n<=1) return ERROR;
    int m=2*n-1;    //结点数量
    int w[n];
    memset(w, 0, n);
    memset(code->_char,0,10000);
    memset(code->_code,0,10000);
    LNode * pTemp=Info->head->next;
    for(int i=0;pTemp!=NULL;pTemp=pTemp->next,i++)
    {
        w[i]=pTemp->freq;  //用w存储每个字符的权值，权值由小到大升序排列
        code->_char[i]=pTemp->_char;
        //printf("%c\n",code->_char[i]);
    }
    
    HT=(HuffmanTree *)malloc((m+1)*sizeof(HTNode));   //0号单元未用
    HTNode * p; int i;
    for(p=HT+1,i=1;i<=n;++i,++p)  //存入每个字符的权值
    {
        p->weight=w[i-1];p->parent=p->lchild=p->rchild=0;
        //printf("Weight:%d\n",p->weight);
    }
    for(;i<=m;++i,++p)   //将空结点的权值、双亲、左孩子、右孩子设为零
    {
        p->weight=p->parent=p->lchild=p->rchild=0;
    }
    
    Taken taken;
    taken.taken1=0;taken.taken2=n;
    int s1=0,s2=0;
    //printf("\n\nCreating Huffman Tree\n\n");
    for(i=n+1;i<=m;++i)
    {   //建赫夫曼树
        Select(HT, &taken, n, i-1, &s1, &s2);   //选择HT[1...i-1]中最小的两个结点
        //printf("Choosing: %d %d\n",s1,s2);
        //printf("taken1:%d; taken2:%d\n",taken.taken1,taken.taken2);
        HT[s1].parent=i;HT[s2].parent=i;
        HT[i].lchild=s1;HT[i].rchild=s2;
        HT[i].weight=HT[s1].weight+HT[s2].weight;
        //printf("Parent:%d; Weight:%d\n",i,HT[i].weight);
    }
    //----------从叶子到根逆向求每个字符的赫夫曼编码---------------//
    //printf("\n\nEncoding\n\n");
    char * cd =(char *)malloc(n*sizeof(char));       //分配求编码的工作空间
    cd[n-1]='\0';                                    //编码结束符
    int wpl=0;                                       //带权路径
    LNode * pNode=Info->head->next;
    for(int i=1;i<=n;++i)                            //逐个字符求赫夫曼编码
    {
        //printf("Encoding %c\n",code->_char[i-1]);
        int start = n-1;            //编码结束符位置
        for(int c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent)   //从叶子到根逆向求编码
        {
            if(HT[f].lchild==c)  cd[--start]='0';
            else cd[--start]='1';
            //printf("Inserted %c\n",cd[start]);
        }
        code->_code[i-1]=(char *)malloc((n-start+1)*sizeof(char)); //为第i个字符编码分配空间
        char * pStr = cd;
        for(int j=1;j<=start;++j)
            ++pStr;
        //printf("Code is:%s\n",pStr);
        strcpy(code->_code[i-1],pStr);
        strcpy(pNode->_code, pStr);
        pNode->length=strlen(pNode->_code);
        wpl+=(pNode->freq*pNode->length);
        pNode=pNode->next;
    }
    free(cd);          //释放工作空间
    //for(int i=1;i<=m;++i)
    //{printf("pos:%d;weight:%d;lchild:%d;rchild:%d\n",i,HT[i].weight,HT[i].lchild,HT[i].rchild);}
    printf("\nWPL:%d\n",wpl);    //输出带权路径
    HuffmanTree * pHT = HT;
    return pHT;
} //HuffmanCoding

status GetCode(HuffmanTree * HT, MsgInfo Info, char _char, char * code)
{   //根据字符获取相应的赫夫曼编码
    char _code[20];
    memset(_code,0,20);
    int numChar=Info.numChar;
    
    LNode * pNode=Info.head->next;
    for(int i=1;i<=numChar;i++)
    {
        if(_char==pNode->_char)  //如果字符相匹配，则存入相应的赫夫曼编码
            strcpy(_code,pNode->_code);
        pNode=pNode->next;
    }
    
    //printf("%s",_code);
    strcpy(code, _code);
    return OK;
} //GetCode

status HuffmanDecoding(HuffmanTree * HT, MsgInfo Info, Code code, char * Msg, char * Trans)
{   //对赫夫曼编码进行解译
    int pos=0;
    int n=Info.numChar;
    int root=2*n-1; //root为根结点的位置
    pos=root;
    //printf("Pos:%d\n",pos);
    //for(int i=1;i<=root;++i)
        //printf("pos:%d;weight:%d;lchild:%d;rchild:%d\n",i,HT[i].weight,HT[i].lchild,HT[i].rchild);
    for(int i=0;i<strlen(Msg);++i)  //根据编码逐一解码
    {
        //printf("%c\n",Msg[i]);
        if(Msg[i]=='0')
        {
            if(HT[pos].lchild!=0)
                {//printf("A\n");
                    pos=HT[pos].lchild;}
            else
                {//printf("Reset\n");
                    Trans[strlen(Trans)]=code._char[pos-1];
                    //printf("Inserted:%c\n",code._char[pos-1]);
                    pos=root;--i;}
        }
        else
        {
            if(HT[pos].rchild!=0)
                {//printf("B\n");
                    pos=HT[pos].rchild;}
            else
                {//printf("Reset\n");
                    Trans[strlen(Trans)]=code._char[pos-1];
                    //printf("Inserted:%c\n",code._char[pos-1]);
                    pos=root;--i;}
        }
    }
    printf("Decoded:%s\n",Trans);
    return OK;
} //HuffmanDecoding

#endif /* HuffmanCode_h */
