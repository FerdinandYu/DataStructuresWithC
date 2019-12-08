//
//  Matrix.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/16.
//  Copyright © 2018 余宏昊. All rights reserved.
//
//支持矩阵的运算。比如矩阵的加、减、乘、转制、特征值、行列式的值等等。

#ifndef Matrix_h
#define Matrix_h

#include <math.h>
#include <string.h>

#define OK       1
#define ERROR    0
#define ERROR1  -1
#define ERROR2  -2

#define MAXSIZE 1000
#define MAXRC   1000

typedef int Status;
typedef float ElemType;
typedef struct {
    int       i,j;
    ElemType    e;
} Triple;      //三元组稀疏矩阵
typedef struct {
    Triple     *data[MAXSIZE+1];
    int                   taken;  //已插入的三元组个数
    int           rpos[MAXRC+1];  //每行首个非零元在改行的相对位置
    int            num[MAXRC+1];  //每行非零元个数
    int              mu,nu,tu;
} TSMatrix;

//---------------------------------------------------------//
Status InitSMatrix(TSMatrix *M);
    //初始化稀疏矩阵
Triple * CreateTriple(int i, int j, ElemType e);
    //创建三元组
Status InsertOrderSMatrix(TSMatrix *M, Triple * triple);
    //按顺序向稀疏矩阵插入三元组
Status CreateSMatrix(TSMatrix *M);
    //创建稀疏矩阵
Status DestroySMatrix(TSMatrix *M);
    //销毁稀疏矩阵
Status ResetSMatrix(TSMatrix *M);
    //将稀疏矩阵置空
float GetMatrixElem(TSMatrix M, int i, int j);
    //获取稀疏矩阵第i行第j列的元素
Status PrintSMatrix(TSMatrix M);
    //打印稀疏矩阵
Status CopySMatrix(TSMatrix M, TSMatrix *T);
    //复制稀疏矩阵
Status AddSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);
    //将稀疏矩阵M和N相加，并将结果存入矩阵Q中
Status SubtSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);
    //将稀疏矩阵M和N相减，并将结果存入矩阵Q中
Status MultSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q);
    //将稀疏矩阵M和N相乘，并将结果存入矩阵Q中
Status TransposeSMatrix(TSMatrix M, TSMatrix *T);
    //将稀疏矩阵M进行转置，并将结果存入矩阵T中
float DetVal(TSMatrix M);
    //求解n阶矩阵M对应的行列式
//---------------------------------------------------------//
Status InitSMatrix(TSMatrix *M)
{   //对稀疏矩阵M进行初始化操作
    M->mu=0;M->nu=0;M->tu=0;M->taken=0;
    memset(M->data, 0, MAXSIZE+1);
    memset(M->rpos, 0, MAXRC+1);
    memset(M->num, 0, MAXRC+1);
    return OK;
} //InitSMatrix

Triple * CreateTriple(int i, int j, ElemType e)
{   //创建三元组
    Triple * pTriple=(Triple *)malloc(sizeof(Triple));
    if(!pTriple) exit(ERROR);
    pTriple->i=i;pTriple->j=j;pTriple->e=e;
    return pTriple;
} //CreateTriple

Status InsertOrderSMatrix(TSMatrix *M, Triple * triple)
{   //按行序将三元组插入到稀疏矩阵M中
    int pre=0,post=0;int pos;
    if(triple->i>M->mu||triple->j>M->nu)
        return ERROR1;
    //如果还未插入任何元素，则插在第一个位置上
    if(M->taken==0)
    {M->data[1]=triple;return OK;}
    for(pos=1;pos<M->taken+1;++pos)
    {
        //printf("Pos:%d; i:%d\n",pos,M->data[pos]->i);
        pre=pos-1;post=pos+1;
        if(triple->i<M->data[pos]->i)        //在no之前插入
        {
            for(int p=M->taken;p>=pre;--p)
                M->data[p+1]=M->data[p];
            M->data[pre+1]=triple;
            return OK;
        }
        else if(triple->i==M->data[pos]->i)
        {
            if(triple->j<M->data[pos]->j)   //在no之前插入
            {
                for(int p=M->taken;p>pre;--p)
                    M->data[p+1]=M->data[p];
                M->data[pre+1]=triple;
                return OK;
            }
            else if(triple->j==M->data[pos]->j)
            {
                //printf("此位置已有元素，请在其他位置插入：\n");
                return ERROR2;
            }
            else if(triple->j>M->data[pos]->j)
                continue;
        }
        else if(triple->i>M->data[pos]->i)
            continue;
    }
    if(pos==M->taken+1)
    {M->data[M->taken+1]=triple;return OK;}
    return OK;
} //InsertOrderSMatrix

Status CreateSMatrix(TSMatrix *M)
{   //根据用户输入创建矩阵
    int i,j;ElemType e;
    printf("请输入要创建的矩阵的行列数以及非零元个数:\n");
    scanf("%d %d %d",&M->mu,&M->nu,&M->tu);
    for(int no=1;no<M->tu+1;++no)
    {
        printf("No:%d; Taken:%d\n",no,M->taken);
        printf("请依次输入非零元元素的位置和值(如:3(行坐标) 2(列坐标) 4(值))：\n");
        printf("正在输入第%d个元素\n",no);
        scanf("%d %d %f",&i,&j,&e);
        Triple * pData=CreateTriple(i, j, e);
        int success=InsertOrderSMatrix(M, pData);
        if(success==1)
        {
            M->taken+=1;
            if(pData->j<M->rpos[pData->i])      //更新改行首个非零元元素的位置
                M->rpos[pData->i]=pData->j;
            M->num[pData->i]+=1;                //改行非零元个数加1
            printf("插入((%d %d),%6.4f)成功\n",pData->i,pData->j,pData->e);
        }
        else if(success==-2)
        {
            printf("此位置已有元素，请在其他位置插入：\n");
            --no;
        }
        else if(success==-1)
        {
            printf("输入的位置超出矩阵大小，请重新输入：\n");
            --no;
        }
    }
    //printf("tu:%d\n",M->tu);
    return OK;
} //CreateSMatrix

Status PrintSMatrix(TSMatrix M)
{
    int row=M.mu;int col=M.nu;
    float elem=0;
    for(int i=1;i<row+1;++i)
    {
        printf("  ");
        for(int j=1;j<col+1;++j)
        {
            elem=GetMatrixElem(M,i,j);
            printf("%6.4f ",elem);
        }
        printf("\n");
    }
    return OK;
} //PrintSMatrix

float GetMatrixElem(TSMatrix M, int i, int j)
{
    ElemType elem=0;
    for(int pos=1;pos<M.taken+1;++pos)
    {
        if(i==M.data[pos]->i&&j==M.data[pos]->j)
            elem=M.data[pos]->e;
    }
    return elem;
} //GetElem

Status DestroySMatrix(TSMatrix *M)
{
    for(int pos=1;pos<M->taken+1;++pos)
        free(M->data[pos]);
    M->mu=0;M->nu=0;M->tu=0;M->taken=0;
    memset(M->data, 0, MAXSIZE+1);
    memset(M->rpos, 0, MAXRC+1);
    memset(M->num, 0, MAXRC+1);
    return OK;
} //DestroySMatrix

Status ResetSMatrix(TSMatrix *M)
{   //重置矩阵
    for(int pos=1;pos<M->taken+1;++pos)
    {
        M->data[pos]->i=0;M->data[pos]->j=0;M->data[pos]->e=0;
    }
    return OK;
} //ResetSMatrix

Status CopySMatrix(TSMatrix M, TSMatrix *T)
{
    T->mu=M.mu;T->nu=M.nu;T->tu=M.tu;
    for(int index=1;index<MAXRC+1;++index)
    {
        T->num[index]=M.num[index];
        T->rpos[index]=M.rpos[index];
    }
    for(int pos=1;pos<M.taken+1;++pos)
        T->data[pos]=M.data[pos];
    T->taken=M.taken;T->tu=M.tu;
    return OK;
} //CopySMatrix

Status AddSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    int sum=0,SumNum=0;ElemType elemM=0,elemN=0;
    if(M.mu!=N.mu||M.nu!=N.nu)
    {
        printf("两个矩阵大小不一致，不能进行相加\n");
        return ERROR;
    }
    else
    {
        Q->mu=M.mu;Q->nu=M.nu;Q->taken=0;
        int row=M.mu; int col=M.nu;
        for(int i=1;i<row+1;++i)
            for(int j=1;j<col+1;++j)
            {
                elemM=GetMatrixElem(M, i, j);
                elemN=GetMatrixElem(N, i, j);
                sum=elemM+elemN;
                if(sum!=0)
                {
                    SumNum+=1;
                    Triple * pData=CreateTriple(i, j, sum);
                    Q->data[SumNum]=pData;
                    Q->taken+=1;Q->tu+=1;
                    if(pData->j<Q->rpos[pData->i])      //更新改行首个非零元元素的位置
                        Q->rpos[pData->i]=pData->j;
                    Q->num[pData->i]+=1;
                }
            }
        return OK;
    }
    return OK;
} //AddSMatrix

Status SubtSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    int diff=0,DiffNum=0;ElemType elemM=0,elemN=0;
    if(M.mu!=N.mu||M.nu!=N.nu)
    {
        printf("两个矩阵大小不一致，不能进行相减\n");
        return ERROR;
    }
    else
    {
        Q->mu=M.mu;Q->nu=M.nu;Q->taken=0;
        int row=M.mu; int col=M.nu;
        for(int i=1;i<row+1;++i)
            for(int j=1;j<col+1;++j)
            {
                elemM=GetMatrixElem(M, i, j);
                elemN=GetMatrixElem(N, i, j);
                diff=elemM-elemN;
                if(diff!=0)
                {
                    DiffNum+=1;
                    Triple * pData=CreateTriple(i, j, diff);
                    Q->data[DiffNum]=pData;
                    Q->taken+=1;Q->tu+=1;
                    if(pData->j<Q->rpos[pData->i])      //更新改行首个非零元元素的位置
                        Q->rpos[pData->i]=pData->j;
                    Q->num[pData->i]+=1;
                }
            }
        return OK;
    }
    return OK;
} //AddSMatrix

Status MultSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{   //改进方法
    ElemType product=0;int ProductNum=0;
    if(M.nu!=N.mu)
    {
        printf("左矩阵的列数与右矩阵的行数不相等，不能相乘\n");
        return ERROR;
    }
    else if(M.nu*N.mu==0) return ERROR;
    else
    {
        Q->mu=M.mu;Q->nu=N.nu;Q->tu=Q->taken=0; //Q初始化
        int row=Q->mu;int col=Q->nu;
        for(int i=1;i<row+1;++i)
            for(int j=1;j<col+1;++j)
            {
                product=0;
                for(int n=1;n<M.nu+1;++n)
                    product+=GetMatrixElem(M, i, n)*GetMatrixElem(N, n, j);
                if(product!=0)
                {
                    ProductNum+=1;
                    Triple * pData=CreateTriple(i, j, product);
                    Q->data[ProductNum]=pData;
                    Q->taken+=1;Q->tu+=1;
                    if(pData->j<Q->rpos[pData->i])      //更新改行首个非零元元素的位置
                        Q->rpos[pData->i]=pData->j;
                    Q->num[pData->i]+=1;
                }
            }
        return OK;
    }
    return OK;
} //MultSMatrix

Status TransposeSMatrix(TSMatrix M, TSMatrix *T)
{
    ElemType elem=0;int i=0;int j=0;
    //printf("Taken:%d;tu:%d\n",M.taken,M.tu);
    T->mu=M.nu;T->nu=M.mu;T->tu=T->taken=0;
    for(int pos=1;pos<M.tu+1;++pos)
    {
        i=M.data[pos]->j;j=M.data[pos]->i;
        elem=M.data[pos]->e;
        //printf("No:%d;Elem:%d\n",pos,elem);
        Triple * pData=CreateTriple(i, j, elem);
        if(InsertOrderSMatrix(T, pData)==1)
        {
            T->taken+=1;T->tu+=1;
            if(pData->i<T->rpos[pData->j])      //更新改行首个非零元元素的位置
                T->rpos[pData->j]=pData->i;
            T->num[pData->j]+=1;
        }
    }
    return OK;
} //TransposeSMatrix

float DetVal(TSMatrix M)
{
    if(M.mu!=M.nu)
    {
        printf("此矩阵不是方阵，不能进行行列式求值\n");
        exit(ERROR);
    }
    int n=M.mu;   //行列式的阶数
    int z;
    float a[n][n],result=1.0,temp;
    memset(a, 0, sizeof(a));
    int i,j;   //记录行列坐标及对应的数值
    ElemType e;
    for(int no=0;no<M.tu;no++)    //还原矩阵
    {
        i=M.data[no+1]->i-1;
        j=M.data[no+1]->j-1;
        e=M.data[no+1]->e;
        a[i][j]=e;
    }
    i=0;j=0;
    
    for(z=0;z<n-1;z++)
        for(i=z;i<n-1;i++)
        {
            if(a[z][z]==0)
                for(i=z;a[z][z]==0;i++)
                {
                    {
                        for(j=0;j<n;j++)
                            a[z][j]=a[z][j]+a[i+1][j];
                    }
                    if(a[z][z]!=0)  break;
                }
            {
                temp=-a[i+1][z]/a[z][z];
                for(j=z;j<n;j++)
                    a[i+1][j]=temp*(a[z][j])+a[i+1][j];
            }
        }
    for(z=0;z<n;z++)
        result=result*(a[z][z]);
    
    return result;
} //DetVal

float Eigenval(TSMatrix M)
{
    float result=0.0;
    
    return result;
} //EigenVal

#endif /* Matrix_h */
