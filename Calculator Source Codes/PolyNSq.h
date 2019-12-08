//
//  PolyNSq.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/25.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef PolyNSq_h
#define PolyNSq_h

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define LIST_INIT_SIZE 100  //线性表存储空间的初始分配量
#define LISTINCREMENT  10   //线性表存储空间的分配增量

typedef int StatusSq;
typedef float ElemTypeSq;

typedef struct {
    ElemTypeSq  coefSq;
    ElemTypeSq  expoSq;
} TermSq;
typedef struct {
    TermSq           * termSq;
    int              lengthSq;  //一元多项式的项数
    int            listsizeSq;  //当前分配的存储容量
} PolynomialSq;

//--------------------------------------------------------------------------//
StatusSq InitPolynomialSq(PolynomialSq * pPolySq);
    //构建一个空的用于存放一元多项式的顺序表
StatusSq ExtendPolynomialSq(PolynomialSq * pPolySq, int n);
    //扩充存放一元多项式的顺序表的容量
StatusSq InsertPolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq coefSq, ElemTypeSq expoSq);
    //根据用户输入向顺序表中插入多项式的项
StatusSq InsertPolynomialOrderSq(PolynomialSq * pPolySq, ElemTypeSq coefSq, ElemTypeSq expoSq);
    //按指数升序插入新的项，如果指数相同，则合并同类项
StatusSq CreatePolynomialSq(PolynomialSq * pPolySq);
    //根据用户输入创建一元多项式
StatusSq DeletePolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq * coefSq, ElemTypeSq * expoSq);
    //根据用户输入删除一元多项式中特定的项
StatusSq ReplacePolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq coefSq, ElemTypeSq expoSq);
    //根据用户输入替换一元多项式中特定的项
StatusSq PrintPolynomialSq(PolynomialSq * pPolySq);
    //输出一元多项式
StatusSq DestroyPolynomialSq(PolynomialSq * pPolySq);
    //销毁一个存放一元多项式的顺序表
StatusSq ClearPolynomialSq(PolynomialSq * pPolySq);
    //清空一个存放一元多项式的顺序表
StatusSq EmptyPolynomialSq(PolynomialSq PolySq);
    //判别一元多项式是否含有项
StatusSq LengthPolynomialSq(PolynomialSq PolySq);
    //返回一元多项式的项数
StatusSq GetTermPolynomialSq(PolynomialSq PolySq, int i, ElemTypeSq * coefSq, ElemTypeSq * expoSq);
    //用基本元素coef和expo返回一元多项式中第i项的值
StatusSq AddPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq);
    //将多项式A和多项式B相加，把结果存放在另一个多项式中
StatusSq SubtractPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq);
    //将多项式A和多项式B相减，把结果存放在另一个多项式中
StatusSq MultiplyPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq);
    //将多项式A和多项式B相乘，把结果存放在一个新的多项式中
StatusSq DerivativePolynomialSq(PolynomialSq * pPolySq, PolynomialSq * pPolySq1);
    //多项式求导，结果存放在顺序表PolySq1中
//--------------------------------------------------------------------------//

/*-----------------------------------------基本函数----------------------------------------*/
StatusSq InitPolynomialSq(PolynomialSq * pPolySq)
{   //构建一个空的用于存放一元多项式的顺序表
    assert(pPolySq);         //当参数为0时报错并终止程序
    pPolySq->termSq=(TermSq *)malloc(sizeof(TermSq)*LIST_INIT_SIZE);
    if(pPolySq != NULL)
    {
        pPolySq->lengthSq=0;                 //空表长度（元素个数）为零
        pPolySq->listsizeSq=LIST_INIT_SIZE;  //将表的初始容量设定为LIST_INIT_LIST
        //printf("一元多项式初始化成功，当前表长为%d，当前容量为%d\n",pPoly->length,pPoly->listsize);
        return OK;
    }
    else
        exit(OVERFLOW);
} //InitPolynomialSq

StatusSq ExtendPolynomialSq(PolynomialSq * pPolySq, int n)
{   //扩充存放一元多项式的顺序表的容量
    assert(pPolySq);
    pPolySq->listsizeSq+=(n*LISTINCREMENT);    //根据用户输入进行扩充
    TermSq *new_space=(TermSq *)malloc(pPolySq->listsizeSq*sizeof(TermSq));
    if(new_space == NULL) exit(OVERFLOW);
    else
    {
        memcpy(new_space,pPolySq->termSq,pPolySq->lengthSq*sizeof(TermSq));  //从原来的内存地址的起始位置开始拷贝n个字节到新的的内存地址
        free(pPolySq->termSq);
        pPolySq->termSq=new_space;
        //printf("扩容成功，当前容量为%d\n",pPoly->listsize);
    }
    return OK;
} //ExtendPolynomialSq

StatusSq InsertPolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq coefSq, ElemTypeSq expoSq)
{   //根据用户输入向顺序表中插入多项式的项
    int j;
    assert(pPolySq);
    if(pPolySq->lengthSq==pPolySq->listsizeSq)  //如果表已满则进行扩容
    {ExtendPolynomialSq(pPolySq,1);}
    if(i<1||i>pPolySq->lengthSq+1)     //判断插入位置是否正确
    {
        //printf("插入位置错误，请重新输入\n");
        return ERROR;
    }
    else
    {
        for(j=pPolySq->lengthSq-1;j>=i-1;j--)        //向右挪动元素
            pPolySq->termSq[j+1]=pPolySq->termSq[j];
        pPolySq->termSq[i-1].coefSq=coefSq;          //插入系数
        pPolySq->termSq[i-1].expoSq=expoSq;          //插入指数
        pPolySq->lengthSq++;
        //printf("插入成功，插入%dx^%d为第%d项\n",coef,expo,i);
        return OK;
    }
} //InsertPolynomialSq

StatusSq InsertPolynomialOrderSq(PolynomialSq * pPolySq, ElemTypeSq coefSq, ElemTypeSq expoSq)
{   //按指数升序插入新的项，如果指数相同，则合并同类项
    if(pPolySq->lengthSq==0)   //如果还没有项，则把新项插在第一个元素位置
        InsertPolynomialSq(pPolySq, 1, coefSq, expoSq);
    else
    {
        for(int j=1;j<pPolySq->lengthSq+1;j++)  //遍历现有的一元多项式
            if(expoSq<pPolySq->termSq[j-1].expoSq)  //如果指数小于正在比较的项的指数，则把新项插在正在比较的项的位置上
            {
                InsertPolynomialSq(pPolySq, j, coefSq, expoSq);
                break;
            }
            else if(expoSq==pPolySq->termSq[j-1].expoSq) //如果指数相同，则合并同类项
            {
                pPolySq->termSq[j-1].coefSq+=coefSq;
                //printf("合并同类项成功\n");
                break;
            }
            else if(expoSq>pPolySq->termSq[j-1].expoSq) //如果指数大于正在比较的项的指数
            {
                if(j==pPolySq->lengthSq)   //如果已经到了最后一项，则把新项插在表尾
                {InsertPolynomialSq(pPolySq, j+1, coefSq, expoSq);break;}
                else   //如果不是最后一项，则与下一项进行比较
                    continue;
            }
    }
    return OK;
} //InsertPolynomialOrderSq

StatusSq CreatePolynomialSq(PolynomialSq * pPolySq)
{   //根据用户输入创建一元多项式，存储在顺序表中
    int numTerm;
    printf("创建一元多项式：请输入项数\n");
    scanf("%d",&numTerm);
    float coef,expo;
    for(int i=0;i<numTerm;i++)
    {
        printf("请输入系数和指数，现在是第%d项\n",i+1);
        scanf("%f %f",&coef,&expo);
        InsertPolynomialSq(pPolySq, i+1, coef, expo);
    }
    return OK;
} //CreatePolynomialSq

StatusSq DeletePolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq * coefSq, ElemTypeSq * expoSq)
{   //根据用户输入删除一元多项式中特定的项
    int j;
    if(i<1||i>pPolySq->lengthSq)     //判断删除位置是否正确，如果不正确，则删除尾项
    {
        i=pPolySq->lengthSq;
    }
    *coefSq = pPolySq->termSq[i-1].coefSq;
    *expoSq = pPolySq->termSq[i-1].expoSq;
    for(j=i-1;j<pPolySq->lengthSq;j++)  //向左挪动元素
    {
        pPolySq->termSq[j]=pPolySq->termSq[j+1];
    }
    pPolySq->lengthSq--;
    //printf("第%d项被删除，该项为%dx^%d\n",i,*coef,*expo);
    return OK;
} //DeletePolynomialSq

StatusSq ReplacePolynomialSq(PolynomialSq * pPolySq, int i, ElemTypeSq coefSq, ElemTypeSq expoSq)
{   //根据用户输入替换一元多项式中特定的项
    assert(pPolySq);
    if(i<1||i>pPolySq->lengthSq+1)     //判断位置是否正确
    {
        //printf("选择位置错误，请重新输入\n");
        return ERROR;
    }
    else
    {
        pPolySq->termSq[i-1].coefSq=coefSq;
        pPolySq->termSq[i-1].expoSq=expoSq;
        //printf("替换成功，替换第%d项为%dx^%d\n",i,coef,expo);
        return OK;
    }
} //ReplacePolynomialSq

StatusSq PrintPolynomialSq(PolynomialSq * pPolySq)
{   //输出一元多项式
    if(pPolySq->lengthSq==0)
    {
        printf("\n一元多项式为空\n");
        return OK;
    }
    else
    {
        for(int i=0;i<pPolySq->lengthSq-1;i++)
            printf("%fX^%f + ",pPolySq->termSq[i].coefSq,pPolySq->termSq[i].expoSq);
        printf("%fX^%f\n",pPolySq->termSq[pPolySq->lengthSq-1].coefSq,pPolySq->termSq[pPolySq->lengthSq-1].expoSq);
        return OK;
    }
} //PrintPolynomialSq

StatusSq DestroyPolynomialSq(PolynomialSq * pPolySq)
{   //销毁一个存放一元多项式的顺序表
    free(pPolySq->termSq);   //释放存储空间
    pPolySq->lengthSq=0;
    pPolySq->listsizeSq=0;
    //printf("一元多项式销毁成功\n");
    return OK;
} //DestroyPolynomialSq

StatusSq ClearPolynomialSq(PolynomialSq * pPolySq)
{   //清空一个存放一元多项式的顺序表
    int len=pPolySq->lengthSq;
    ElemTypeSq coefSq,expoSq;
    for(int i=1;i<len+1;i++)
        DeletePolynomialSq(pPolySq, i, &coefSq, &expoSq);
    //printf("一元多项式清空成功\n");
    return OK;
} //ClearPolynomialSq

StatusSq EmptyPolynomialSq(PolynomialSq PolySq)
{   //判别一元多项式是否含有项
    int i;
    TermSq * p_polyelemSq = PolySq.termSq;
    for(i=0;i<PolySq.lengthSq;i++)
    {
        if( PolySq.lengthSq != 0)
        {
            PolySq.termSq=p_polyelemSq;
            printf("\n该一元多项式含有项\n");
            return FALSE;
        }
        PolySq.termSq++;
    }
    printf("\n该一元多项式不含任何项\n");
    return TRUE;
} //EmptyPolynomialSq

StatusSq LengthPolynomialSq(PolynomialSq PolySq)
{   //返回一元多项式的项数
    int num = PolySq.lengthSq;
    printf("\n该一元多项式项数为%d\n",num);
    return OK;
} //LengthPolynomialSq

StatusSq GetTermPolynomialSq(PolynomialSq PolySq, int i, ElemTypeSq * coefSq, ElemTypeSq * expoSq)
{   //用基本元素coef和expo返回一元多项式中第i项的值
    int j;
    TermSq * p_polyelemSq = PolySq.termSq;
    if(i<1||i>PolySq.lengthSq)     //判断位置是否正确
    {
        printf("\n位置错误，请重新输入\n");
        return ERROR;
    }
    for(j=1;j<=i-1;j++)
        p_polyelemSq++;
    *coefSq = p_polyelemSq->coefSq;
    *expoSq = p_polyelemSq->expoSq;
    printf("\n第%d项为%fx^%f\n",i,*coefSq,*expoSq);
    return OK;
} //GetTermPolynomialSq

StatusSq AddPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq)
{   //将多项式A和多项式B相加，把结果存放在多项式A的顺序表中，并输出新的多项式A
    int lenA=pPolySqA->lengthSq;
    int lenB=pPolySqB->lengthSq;
    ElemTypeSq coefSq,expoSq;
    for(int i=0;i<lenA;i++)
    {
        coefSq=pPolySqA->termSq[i].coefSq;
        expoSq=pPolySqA->termSq[i].expoSq;
        InsertPolynomialOrderSq(pPolySq, coefSq, expoSq);
    }
    for(int j=0;j<lenB;j++)
    {
        coefSq=pPolySqB->termSq[j].coefSq;
        expoSq=pPolySqB->termSq[j].expoSq;
        InsertPolynomialOrderSq(pPolySq, coefSq, expoSq);
    }
    return OK;
} //AddPolynomialsSq

StatusSq SubtractPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq)
{   //将多项式A和多项式B相减，把结果存放在多项式A的顺序表中，并输出新的多项式A
    int lenA=pPolySqA->lengthSq;
    int lenB=pPolySqB->lengthSq;
    ElemTypeSq coefSq,expoSq;
    for(int i=0;i<lenA;i++)
    {
        coefSq=pPolySqA->termSq[i].coefSq;
        expoSq=pPolySqA->termSq[i].expoSq;
        InsertPolynomialOrderSq(pPolySq, coefSq, expoSq);
    }
    for(int j=0;j<lenB;j++)
    {
        coefSq=pPolySqB->termSq[j].coefSq;
        coefSq=-coefSq;
        expoSq=pPolySqB->termSq[j].expoSq;
        InsertPolynomialOrderSq(pPolySq, coefSq, expoSq);
    }
    return OK;
} //SubtractPolynomialsSq

StatusSq MultiplyPolynomialsSq(PolynomialSq * pPolySqA, PolynomialSq * pPolySqB, PolynomialSq * pPolySq)
{   //将多项式A和多项式B相乘，把结果存放在一个新的多项式中
    int lenA=pPolySqA->lengthSq;
    int lenB=pPolySqB->lengthSq;
    ElemTypeSq coefSqA,expoSqA,coefSqB,expoSqB,coefSqC,expoSqC;
    for(int i=0;i<lenA;i++) //将多项式A的每一项与多项式B的每一项分别相乘，并按升序插入多项式C中，时间复杂度为o(n^2)
    {
        coefSqA = pPolySqA->termSq[i].coefSq;
        expoSqA = pPolySqA->termSq[i].expoSq;
        for(int j=0;j<lenB;j++)
        {
            coefSqB = pPolySqB->termSq[j].coefSq;
            expoSqB = pPolySqB->termSq[j].expoSq;
            coefSqC = coefSqA*coefSqB;
            expoSqC = expoSqA+expoSqB;
            InsertPolynomialOrderSq(pPolySq, coefSqC, expoSqC);
        }
    }
    return OK;
} //MultiplyPolynomials

StatusSq DerivativePolynomialSq(PolynomialSq * pPolySq, PolynomialSq * pPolySq1)
{   //多项式求导，结果存放在顺序表PolySq1中
    int numTerm = pPolySq->lengthSq;
    ElemTypeSq coefSq,expoSq;
    for(int i=0;i<numTerm;i++)
    {
        //printf("%fX^%f\n",pPolySq->termSq[i].coefSq,pPolySq->termSq[i].expoSq);
        if(pPolySq->termSq[i].expoSq==0)
            i++;
        else
        {
            coefSq=pPolySq->termSq[i].coefSq*pPolySq->termSq[i].expoSq;
            expoSq=pPolySq->termSq[i].expoSq-1;
            InsertPolynomialOrderSq(pPolySq1, coefSq, expoSq);
        }
    }
    return OK;
} //DerivativePolynomialSq

#endif /* PolyNSq_h */
