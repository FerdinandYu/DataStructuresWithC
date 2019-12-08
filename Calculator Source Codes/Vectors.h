//
//  Vectors.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/25.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Vectors_h
#define Vectors_h

#include <math.h>
#include <assert.h>

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define LIST_INIT_SIZE 100  //线性表存储空间的初始分配量
#define LISTINCREMENT  10   //线性表存储空间的分配增量

typedef int StatusVc;
typedef float ElemTypeVc;  //元素类型

typedef struct {
    ElemTypeVc * elem;   //存储空间基址
    int        length;   //线性表长度（元素个数）
    int      listsize;   //线性表当前分配的存储容量
} Vector;   //定义向量类型

//-----------------------------------------------------------------//
StatusVc InitVector(Vector * pVec);
    //构建一个空的向量
StatusVc ExtendVector(Vector * pVec, int n);
    //扩充存放向量的线性表的容量
StatusVc InsertVector(Vector * pVec, int i, ElemTypeVc e);
    //在向量的特定位置插入元素
StatusVc CreateVector(Vector * pVec);
    //根据用户输入创建向量
StatusVc DeleteVector(Vector * pVec, int i, ElemTypeVc * e);
    //根据用户输入删除指定位置的元素，同时维数减1
StatusVc ReplaceVector(Vector * pVec, int i, ElemTypeVc e);
    //根据用户输入替换向量中特定位置的元素
StatusVc PrintVector(Vector * pVec);
    //打印向量
StatusVc DestroyVector(Vector * pVec);
    //销毁向量
StatusVc ClearVector(Vector * pVec);
    //清空向量
StatusVc EmptyVector(Vector Vec);
    //判断向量是否为空，如果是返回TRUE，如果否返回FALSE
int DimensionVector(Vector Vec);
    //返回向量维数
StatusVc GetElemVector(Vector Vec, int i, ElemTypeVc * e);
    //用基本元素e返回向量中第i个元素的值
StatusVc AddVectors(Vector VecA, Vector VecB, Vector * Vec);
    //将向量VecA和向量VecB相加，并将结果存放在新的向量Vec中
StatusVc SubtractVectors(Vector VecA, Vector VecB, Vector * Vec);
    //将向量VecA和向量VecB相减，并将结果存放在新的向量Vec中
double MultiplyVectors(Vector VecA, Vector VecB);
    //求向量的乘积
double LengthVector(Vector Vec);
    //求向量的模长
double CosineVectors(Vector VecA, Vector VecB);
    //求向量的夹角余弦值
//-----------------------------------------------------------------//

StatusVc InitVector(Vector * pVec)
{   //构建一个空的向量
    assert(pVec);         //当参数为0时报错并终止程序
    pVec->elem=(ElemTypeVc *)malloc(sizeof(ElemTypeVc)*LIST_INIT_SIZE);
    if(pVec != NULL)
    {
        pVec->length=0;                 //空表长度（元素个数）为零
        pVec->listsize=LIST_INIT_SIZE;  //将表的初始容量设定为LIST_INIT_LIST
        //printf("向量初始化成功，当前表长为%d，当前容量为%d\n",pVec->length,pVec->listsize);
        return OK;
    }
    else
        return ERROR;
} //InitVector

StatusVc ExtendVector(Vector * pVec, int n)
{   //扩充向量的容量
    assert(pVec);
    pVec->listsize+=(n*LISTINCREMENT);    //根据用户输入进行扩充
    ElemTypeVc *new_space=(ElemTypeVc *)malloc(pVec->listsize*sizeof(ElemTypeVc));
    if(new_space == NULL) exit(-2);
    else
    {
        memcpy(new_space,pVec->elem,pVec->length*sizeof(ElemTypeVc));  //从原来的内存地址的起始位置开始拷贝n个字节到新的的内存地址
        free(pVec->elem);
        pVec->elem=new_space;
        //printf("扩容成功，当前容量为%d\n",pVec->listsize);
    }
    return OK;
} //ExtendVector

StatusVc InsertVector(Vector * pVec, int i, ElemTypeVc e)
{   //根据用户输入向向量中插入元素
    int j;
    assert(pVec);
    if(pVec->length==pVec->listsize)  //如果表已满则进行扩容
    {ExtendVector(pVec,1);}
    if(i<1||i>pVec->length+1)     //判断插入位置是否正确
    {
        //printf("插入位置错误，请重新输入\n");
        return ERROR;
    }
    else
    {
        for(j=pVec->length-1;j>=i-1;j--)        //向右挪动元素
            pVec->elem[j+1]=pVec->elem[j];
        pVec->elem[i-1]=e;
        pVec->length++;
        //printf("插入成功，插入%d为第%d个元素\n",e,i);
        return OK;
    }
} //InsertVector

StatusVc CreateVector(Vector * pVec)
{   //根据用户输入的维数创建向量
    int dim;      //向量维数
    printf("创建向量：请输入向量维数\n");
    scanf("%d",&dim);
    float input=0.0;
    for(int i=0;i<dim;i++)
    {
        printf("请输入数字，现在是第%d个\n",i+1);
        scanf("%f",&input);
        InsertVector(pVec, i+1, input);
    }
    return OK;
} //CreateVector

StatusVc DeleteVector(Vector * pVec, int i, ElemTypeVc * e)
{   //根据用户输入删除向量中特定位置的元素
    int j;
    if(i<1||i>pVec->length)     //判断删除位置是否正确，如果不正确，则删除最后一个
    {
        i=pVec->length;
    }
    e = &pVec->elem[i-1];
    for(j=i-1;j<pVec->length-1;j++)  //向左挪动元素
    {
        pVec->elem[j]=pVec->elem[j+1];
    }
    pVec->length--;
    //printf("删除成功，被删除的元素序号为%d\n",i);
    return OK;
} //DeleteVector

StatusVc ReplaceVector(Vector * pVec, int i, ElemTypeVc e)
{   //根据用户输入替换向量中特定位置的元素
    assert(pVec);
    if(i<1||i>pVec->length+1)     //判断插入位置是否正确
    {
        printf("选择位置错误，请重新输入\n");
        return ERROR;
    }
    else
    {
        pVec->elem[i-1]=e;
        printf("替换成功，替换%f为第%d个元素\n",e,i);
        return OK;
    }
} //ReplaceVector

StatusVc PrintVector(Vector * pVec)
{   //打印出向量
    int len=pVec->length;
    if(pVec==NULL) {printf("向量为空\n");return OK;}
    else
    {
        printf("(");
        for(int i=0;i<pVec->length-1;i++)
            printf("%f,",pVec->elem[i]);
        printf("%f)",pVec->elem[len-1]);
        return OK;
    }
} //PrintVector

StatusVc DestroyVector(Vector * pVec)
{   //销毁一个向量
    free(pVec->elem);   //释放存储空间
    pVec->length=0;
    pVec->listsize=0;
    printf("向量销毁成功\n");
    return OK;
} //DestroyVector

StatusVc ClearVector(Vector * pVec)
{   //清空一个向量
    int len=pVec->length;
    ElemTypeVc e;
    for(int i=1;i<len+1;i++)
        DeleteVector(pVec, i, &e);
    return OK;
} //ClearVector

StatusVc EmptyVector(Vector Vec)
{   //判别向量是否为空
    int len=Vec.length;
    if(len==0)
    {
        printf("该向量为空\n");
        return TRUE;
    }
    else
    {
        printf("该向量不为空\n");
        return FALSE;
    }
} //EmptyVector

int DimensionVector(Vector Vec)
{   //返回向量的维数
    int num = Vec.length;
    printf("该向量维数为%d\n",num);
    return num;
} //DimensionVector

StatusVc GetElemVector(Vector Vec, int i, ElemTypeVc * e)
{   //用基本元素e返回向量中第i个元素的值
    int j;
    ElemTypeVc * p_elem = Vec.elem;
    if(i<1||i>Vec.length)     //判断插入位置是否正确
    {printf("位置错误，请重新输入\n");return ERROR;}
    for(j=1;j<=i-1;j++)
        Vec.elem++;
    e = Vec.elem;
    Vec.elem = p_elem;
    printf("第%d个元素为%f\n",i,* e);
    return OK;
} //GetElemVector

StatusVc AddVectors(Vector VecA, Vector VecB, Vector * Vec)
{   //将向量VecA与向量VecB相加，并将结果存放在向量Vec中
    if(VecA.length!=VecB.length)
    {
        printf("两个向量维数不相等，不能相加\n");
        return ERROR;
    }
    int len=VecA.length;
    float sum=0.0;
    for(int i=0;i<len;i++)
    {
        sum=VecA.elem[i]+VecB.elem[i];
        InsertVector(Vec, i+1, sum);
    }
    return OK;
} //AddVectors

StatusVc SubtractVectors(Vector VecA, Vector VecB, Vector * Vec)
{   //将向量VecA与向量VecB相减，并将结果存放在向量Vec中
    if(VecA.length!=VecB.length)
    {
        printf("两个向量维数不相等，不能相减\n");
        return ERROR;
    }
    int len=VecA.length;
    float sum=0.0;
    for(int i=0;i<len;i++)
    {
        sum=VecA.elem[i]-VecB.elem[i];
        InsertVector(Vec, i+1, sum);
    }
    return OK;
} //SubtractVectors

double MultiplyVectors(Vector VecA, Vector VecB)
{   //求两个向量的乘积
    int lenA=VecA.length;
    int lenB=VecB.length;
    int sum=0;
    if(lenA!=lenB)
    {
        printf("两个向量维数不相等，不能相乘\n");
        return ERROR;
    }
    else
    {
        for(int i=0;i<lenA;i++)
        {
            sum+=VecA.elem[i]*VecB.elem[i];
        }
        printf("向量相乘成功\n");
        return sum;
    }
} //MultiplyVectors

double LengthVector(Vector Vec)
{   //求向量的模长
    int len=Vec.length;
    double x=0,SquareSum=0,length;
    for(int i=0;i<len;i++)
    {
        x=Vec.elem[i];
        SquareSum+=x*x;
    }
    length=sqrt(SquareSum);
    return length;
} //LengthVector

double CosineVectors(Vector VecA, Vector VecB)
{   //求两个向量的夹角余弦值
    int lenA=VecA.length;
    int lenB=VecB.length;
    if(lenA!=lenB)
    {
        printf("两个向量维数不相等，不能求夹角余弦值\n");
        return ERROR;
    }
    else
    {
        double lengthA=LengthVector(VecA);
        double lengthB=LengthVector(VecB);
        double product = MultiplyVectors(VecA, VecB);
        double cosine = product/(lengthA*lengthB);
        printf("两个向量的夹角余弦值为%8.4f\n",cosine);
        return cosine;
    }
}

#endif /* Vectors_h */
