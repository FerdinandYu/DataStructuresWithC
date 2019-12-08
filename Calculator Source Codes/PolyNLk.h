//
//  PolyNLk.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/25.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef PolyNLk_h
#define PolyNLk_h

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

typedef int StatusLk;
typedef float ElemTypeLk;

typedef struct TermLk {
    ElemTypeLk        coefLk;   //系数
    ElemTypeLk        expoLk;   //指数
    struct TermLk    *next;   //指针域
} TermLk;    //多项式的项
typedef struct {
    int            lengthLk;   //项数
    TermLk           * head;   //头结点
} PolynomialLk;

//----------------------------------------------------------------------------//
StatusLk InitPolynomialLk(PolynomialLk * pPolyLk);
    //初始化用于存放一元多项式的链表
TermLk * CreateTermLk(ElemTypeLk coef,ElemTypeLk expo);
    //创建新的项，并返回项的存储地址
StatusLk PushHeadLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk);
    //在单链表头部插入项
StatusLk PushTailLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk);
    //在单链表尾部插入项
StatusLk DeleteHeadLk(PolynomialLk * pPolyLk);
    //删除单链表的头结点
StatusLk DeleteTailLk(PolynomialLk * pPolyLk);
    //删除单链表的尾结点
StatusLk InsertPolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk coefLk, ElemTypeLk expoLk);
    //在任意位置插入项
StatusLk InsertPolynomialOrderLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk);
    //根据项的指数大小进行升序插入，如果指数相同则进行同类项合并
StatusLk CreatePolynomicalLK(PolynomialLk * pPolyLk);
    //根据用户输入创建一元多项式
StatusLk DeletePolynomialLk(PolynomialLk * pPolyLk, int i);
    //删除一元多项式的任意项
StatusLk PrintPolynomialLk(PolynomialLk * pPolyLk);
    //打印输出一元多项式
int LengthPolynomialLk(PolynomialLk * pPolyLk);
    //返回多项式项数
StatusLk ClearPolynomialLk(PolynomialLk * pPolyLk);
    //清空存放一元多项式的链表
StatusLk DestroyPolynomialLk(PolynomialLk * pPolyLk);
    //销毁存放一元多项式的链表
StatusLk ReplacePolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk coefLk, ElemTypeLk expoLk);
    //根据用户输入替换一元多项式中的某一项
StatusLk EmptyPolynomialLk(PolynomialLk PolyLk);
    //判别一个一元多项式是否为空，返回TRUE或者FALSE
StatusLk GetTermPolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk * coefLk, ElemTypeLk * expoLk);
    //根据用户输入获取一元多项式中的第i项，并将其返回到(coef,expo)
StatusLk AddPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk);
    //将多项式A与多项式B相加，将结果存放在另一个多项式中
StatusLk SubtractPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk);
    //将多项式A与多项式B相减，将结果存放在另一个多项式中
StatusLk MultiplyPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk);
    //将多项式A和多项式B相乘，将结果存放在另一个多项式中
StatusLk DerivativePolynomialLk(PolynomialLk * pPolyLk, PolynomialLk * pPolyLk1);
    //多项式求导，将结果存放在另一个多项式中
//----------------------------------------------------------------------------//

StatusLk InitPolynomialLk(PolynomialLk * pPolyLk)
{   //初始化用于存放一元多项式的链表
    pPolyLk->head = (TermLk *)malloc(sizeof(TermLk));
    pPolyLk->head->next = NULL;
    pPolyLk->lengthLk = 0;
    //printf("一元多项式初始化成功\n");
    return OK;
} //InitPolynomialLk

TermLk * CreateTermLk(ElemTypeLk coef,ElemTypeLk expo)
{   //创建新的项，并返回项的存储地址
    TermLk *pNew=(TermLk *)malloc(sizeof(TermLk));   //为新结点开辟空间
    pNew->coefLk=coef;
    pNew->expoLk=expo;
    pNew->next=NULL;
    return pNew;
} //CreateTermLk

StatusLk PushHeadLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk)
{   //在单链表头部插入项
    TermLk *pn = CreateTermLk(coefLk,expoLk);  //创建项
    if(pPolyLk->lengthLk==0)
        pPolyLk->head->next=pn;
    else
    {
        pn->next=pPolyLk->head->next;
        pPolyLk->head->next=pn;
    }
    pPolyLk->lengthLk++;
    return OK;
} //PushHeadLk

StatusLk PushTailLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk)
{   //在单链表尾部插入项
    TermLk *pn = pPolyLk->head;       //找到最后一个结点
    while (pn->next != NULL)
    {
        pn = pn->next;
    }
    //插入项
    TermLk *pm = CreateTermLk(coefLk, expoLk);
    pn->next = pm;
    pPolyLk->lengthLk++;
    return OK;
} //PushTailLk

StatusLk DeleteHeadLk(PolynomialLk * pPolyLk)
{   //删除单链表的头结点
    TermLk *pn = pPolyLk->head->next;           //把要删除的结点保存起来
    pPolyLk->head->next = pPolyLk->head->next->next;    //连接它的前结点与后结点
    free(pn);//释放并置为空
    pn = NULL;
    pPolyLk->lengthLk--;
    return OK;
} //DeleteHeadLk

StatusLk DeleteTailLk(PolynomialLk * pPolyLk)
{   //删除单链表的尾结点
    TermLk *pn = pPolyLk->head;          //第一个结点
    while (pn->next->next != NULL)
    {
        pn = pn->next;
    }
    TermLk *pm = pn->next;
    pn->next = NULL;
    free(pm);
    pm = NULL;
    pPolyLk->lengthLk--;
    return OK;
} //DeleteTailLk

StatusLk InsertPolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk coefLk, ElemTypeLk expoLk)
{   //在任意位置插入项
    //判断插入位置是否合法
    if (i<1 || i>pPolyLk->lengthLk)  //若插入位置不合法，则插入到尾部
    {
        //printf("插入位置不合法，插入到尾部\n");
        PushTailLk(pPolyLk, coefLk, expoLk);
        return OK;
    }
    //头插
    else if (i == 1)
    {
        TermLk *pn = CreateTermLk(coefLk, expoLk);
        pn->next = pPolyLk->head->next;
        pPolyLk->head->next = pn;
        pPolyLk->lengthLk++;
        return OK;
    }
    //中间任意位置插入
    else
    {
        TermLk *pn = CreateTermLk(coefLk, expoLk);
        TermLk *pm = pPolyLk->head->next;
        for (int j=1; j<i-1; j++)
        {
            pm = pm->next;
        }
        pn->next = pm->next;
        pm->next = pn;
        pPolyLk->lengthLk++;
        return OK;
    }
} //InsertPolynomialLk

StatusLk DeletePolynomialLk(PolynomialLk * pPolyLk, int i)
{   //删除一元多项式的任意项
    //判断删除位置是否合法
    if (i<1 || i>pPolyLk->lengthLk)       //如果位置不合法，则删除最后一项
    {
        DeleteTailLk(pPolyLk);
        return OK;
    }
    TermLk *pn = pPolyLk->head->next;
    if (i == 1)     //头删
    {
        pPolyLk->head->next = pPolyLk->head->next->next;
        free(pn);
        pPolyLk->lengthLk--;
        return OK;
    }
    //任意位置删除
    for (int j = 1; j < i-1; j++)
    {
        pn = pn->next;
    }
    TermLk *pm = pn->next;
    pn->next = pn->next->next;
    free(pm);
    pm = NULL;
    pPolyLk->lengthLk--;
    //printf("删除成功，第%d个元素被删除\n",i);
    return OK;
} //DeletePolynomialLk

StatusLk PrintPolynomialLk(PolynomialLk * pPolyLk)
{   //打印输出一元多项式
    StatusLk GetTermPolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk * coefLk, ElemTypeLk * expoLk);
    TermLk *pTemp = pPolyLk->head->next;
    ElemTypeLk coefLk,expoLk;
    int len=pPolyLk->lengthLk;
    if (len == 0)
    {
        //printf("一元多项式为空\n");
        return ERROR;
    }
    else
    {
        //printf("一元多项式为：\n");
        while (pTemp->next != NULL)
        {
            printf("%fx^%f + ", pTemp->coefLk, pTemp->expoLk);
            pTemp = pTemp->next;
        }
        GetTermPolynomialLk(pPolyLk, len, &coefLk, &expoLk);
        printf("%fx^%f", coefLk, expoLk);
        printf("\n");
        return OK;
    }
} //PrintPolynomialLk

int LengthPolynomialLk(PolynomialLk * pPolyLk)
{   //返回多项式项数
    printf("多项式项数为%d\n",pPolyLk->lengthLk);
    return pPolyLk->lengthLk;
} //LengthPolynomialLk

StatusLk ClearPolynomialLk(PolynomialLk * pPolyLk)
{   //清空存放一元多项式的链表
    int len=pPolyLk->lengthLk;
    for(int i=1;i<len+1;i++)
        DeletePolynomialLk(pPolyLk, i);
    return OK;
} //ClearPolynomialLk

StatusLk DestroyPolynomialLk(PolynomialLk * pPolyLk)
{   //销毁存放一元多项式的链表
    while (pPolyLk->head != NULL)
    {
        TermLk *pn = pPolyLk->head;
        pPolyLk->head = pPolyLk->head->next;
        free(pn);
        pn = NULL;
        pPolyLk->lengthLk--;
    }
    //printf("销毁链表成功\n");
    return OK;
} //DestroyPolynomialLk

StatusLk ReplacePolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk coefLk, ElemTypeLk expoLk)
{   //根据用户输入替换一元多项式中的某一项
    if (i-1<0 || i-1>pPolyLk->lengthLk)
    {
        //printf("选择位置不合法，请重新输入\n");
        return ERROR;
    }
    //头部
    if (i-1 == 0)
    {
        pPolyLk->head->coefLk = coefLk;
        pPolyLk->head->expoLk = expoLk;
        //printf("第%d个元素被替换成功，新元素为(%d,%d)\n",i,coef,expo);
        return OK;
    }
    //中间任意位置替换
    else
    {
        TermLk *pm = pPolyLk->head;
        for (int j=1; j<i; j++)
        {
            pm = pm->next;
        }
        pm->coefLk = coefLk;
        pm->expoLk = expoLk;
        //printf("第%d个元素被替换成功，新元素为(%d,%d)\n",i,coef,expo);
        return OK;
    }
} //ReplacePolynomialLk

StatusLk EmptyPolynomialLk(PolynomialLk PolyLk)
{   //判别一个一元多项式是否为空
    if(PolyLk.lengthLk==0)
    {
        printf("该一元多项式为空\n");
        return TRUE;
    }
    else
    {
        printf("该一元多项式不为空");
        return FALSE;
    }
} //EmptyPolynomialLk

StatusLk GetTermPolynomialLk(PolynomialLk * pPolyLk, int i, ElemTypeLk * coefLk, ElemTypeLk * expoLk)
{   //根据用户输入获取一元多项式中的第i项，并将其返回到(coef,expo)
    if (i-1<0 || i-1>pPolyLk->lengthLk)
    {
        //printf("选择位置不合法，请重新输入\n");
        return ERROR;
    }
    //头部
    if (i-1 == 0)
    {
        *coefLk = pPolyLk->head->coefLk;
        *expoLk = pPolyLk->head->expoLk;
        //printf("获取第%d项成功，该项为%dx^%d\n",i,*coef,*expo);
        return OK;
    }
    //中间任意位置
    else
    {
        TermLk *pm = pPolyLk->head;
        for (int j=1; j<i; j++)
        {
            pm = pm->next;
        }
        *coefLk = pm->coefLk;
        *expoLk = pm->expoLk;
        //printf("获取第%d项成功，该项为%dx^%d\n",i,*coef,*expo);
        return OK;
    }
} //GetTermPolynomial

StatusLk InsertPolynomialOrderLk(PolynomialLk * pPolyLk, ElemTypeLk coefLk, ElemTypeLk expoLk)
{   //根据输入的指数大小进行元素插入，如果指数相同则进行同类项合并
    if(pPolyLk->lengthLk==0)
        PushHeadLk(pPolyLk, coefLk, expoLk);
    else
    {
        TermLk *pm = pPolyLk->head->next;
        for(int i=1;i<pPolyLk->lengthLk+1;i++)
        {
            if(expoLk<pm->expoLk)
            {
                InsertPolynomialLk(pPolyLk, i, coefLk, expoLk);
                break;
            }
            else if(expoLk==pm->expoLk)
            {
                pm->coefLk+=coefLk;
                break;
            }
            else if(expoLk>pm->expoLk)
            {
                if(pm->next==NULL)
                {
                    PushTailLk(pPolyLk, coefLk, expoLk);
                }
                else
                    pm=pm->next;
            }
        }
    }
    return OK;
} //InsertPolynomialOrderLk

StatusLk CreatePolynomicalLK(PolynomialLk * pPolyLk)
{   //根据用户输入创建一元多项式
    int numTerm;
    float coef,expo;
    printf("创建一元多项式：输入项数\n");
    scanf("%d",&numTerm);
    for(int i=0;i<numTerm;i++)
    {
        printf("请输入一对数组作为项的系数和指数，现在是第%d组：\n",i+1);
        scanf("%f %f",&coef,&expo);
        InsertPolynomialLk(pPolyLk, i+1, coef, expo);
    }
    return OK;
} //CreatePolynomialLk

StatusLk AddPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk)
{   //将多项式A与多项式B相加，将结果存放在另一个多项式中
    int lenA=pPolyLkA->lengthLk;
    int lenB=pPolyLkB->lengthLk;
    ElemTypeLk coefLk,expoLk;
    for(int i=1;i<lenA+1;i++)
    {
        GetTermPolynomialLk(pPolyLkA, i, &coefLk, &expoLk);
        InsertPolynomialOrderLk(pPolyLk, coefLk, expoLk);
    }
    for(int j=1;j<lenB+1;j++)
    {
        GetTermPolynomialLk(pPolyLkB, j, &coefLk, &expoLk);
        InsertPolynomialOrderLk(pPolyLk, coefLk, expoLk);
    }
    return OK;
} //AddPolynomialsLk

StatusLk SubtractPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk)
{   //将多项式A与多项式B相减，将结果存放在另一个多项式中
    int lenA=pPolyLkA->lengthLk;
    int lenB=pPolyLkB->lengthLk;
    ElemTypeLk coefLk,expoLk;
    for(int i=1;i<lenA+1;i++)
    {
        GetTermPolynomialLk(pPolyLkA, i, &coefLk, &expoLk);
        InsertPolynomialOrderLk(pPolyLk, coefLk, expoLk);
    }
    for(int j=1;j<lenB+1;j++)
    {
        GetTermPolynomialLk(pPolyLkB, j, &coefLk, &expoLk);
        InsertPolynomialOrderLk(pPolyLk, -coefLk, expoLk);
    }
    return OK;
} //SubtractPolynomialsLk

StatusLk MultiplyPolynomialsLk(PolynomialLk * pPolyLkA, PolynomialLk * pPolyLkB, PolynomialLk * pPolyLk)
{   //将多项式A和多项式B相乘，将结果存放在另一个多项式中
    int lenA=pPolyLkA->lengthLk;
    int lenB=pPolyLkB->lengthLk;
    ElemTypeLk coefLkA,expoLkA,coefLkB,expoLkB,coefLkC,expoLkC;
    
    for(int i=1;i<lenA+1;i++)
    {
        GetTermPolynomialLk(pPolyLkA, i, &coefLkA, &expoLkA);
        for(int j=1;j<lenB+1;j++)
        {
            GetTermPolynomialLk(pPolyLkB, j, &coefLkB, &expoLkB);
            coefLkC = coefLkA*coefLkB;
            expoLkC = expoLkA+expoLkB;
            InsertPolynomialOrderLk(pPolyLk, coefLkC, expoLkC);
        }
    }
    return OK;
} //MultiplyPolynomialsLk

StatusLk DerivativePolynomialLk(PolynomialLk * pPolyLk, PolynomialLk * pPolyLk1)
{   //多项式求导，将结果存放在另一个多项式中
    int len = pPolyLk->lengthLk;
    ElemTypeLk coefLk,expoLk;
    for(int i=1;i<len+1;i++)
    {
        GetTermPolynomialLk(pPolyLk, i, &coefLk, &expoLk);
        if(expoLk==0)
            i++;
        else
        {
            coefLk=coefLk*expoLk;
            expoLk=expoLk-1;
            InsertPolynomialOrderLk(pPolyLk1, coefLk, expoLk);
        }
    }
    return OK;
} //DerivativePolynomialLk

#endif /* PolyNLk_h */
