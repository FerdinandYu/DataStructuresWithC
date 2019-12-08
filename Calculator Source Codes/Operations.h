//
//  Operations.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/16.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Operations_h
#define Operations_h

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define STACK_INIT_SIZE 100      //栈的初始大小
#define STACKINCREMENT  10       //栈的增量大小
#define EXPRESSIONSIZE  300       //表达式的大小

typedef int Status;

typedef float OpndType;     //操作数元素类型
typedef struct {
    OpndType    * base;   //在栈构造之前和销毁之后，base的值为NULL
    OpndType    *  top;   //栈顶指针
    int      stacksize;   //当前已分配的存储空间，以元素为单位
} OpndStack;              //操作数栈

typedef char OptrType;    //操作符元素类型
typedef struct {
    OptrType    * base;   //在栈构造之前和销毁之后，base的值为NULL
    OptrType    *  top;   //栈顶指针
    int      stacksize;   //当前已分配的存储空间，以元素为单位
} OptrStack;              //操作符栈

//===================================基本操作=================================//
Status InitStack_OPND(OpndStack *S);
Status InitStack_OPTR(OptrStack *S);
    //构造空栈
Status DestroyStack_OPND(OpndStack *S);
Status DestroyStack_OPTR(OptrStack *S);
    //销毁栈
Status ClearStack_OPND(OpndStack *S);
Status ClearStack_OPTR(OptrStack *S);
    //清空栈中元素
Status StackEmpty_OPND(OpndStack S, OpndType *e);
Status StackEmpty_OPTR(OptrStack S, OptrType *e);
    //判定栈是否为空，返回布尔值
int StackLength_OPND(OpndStack S);
int StackLength_OPTR(OptrStack S);
    //返回栈中元素个数
OpndType GetTop_OPND(OpndStack S, OpndType *e1);
OptrType GetTop_OPTR(OptrStack S, OptrType *e2);
    //若栈不为空，则用e返回S的栈顶元素，并返回OK；否则返回FALSE
Status Push_OPND(OpndStack *S, OpndType e);
Status Push_OPTR(OptrStack *S, OptrType e);
    //插入元素e为新的栈顶元素
Status Pop_OPND(OpndStack *S, OpndType *e);
Status Pop_OPTR(OptrStack *S, OptrType *e);
    //若栈S不为空，则删除S的栈顶元素，用e返回其值，并返回OK,否则返回ERROR
Status Traverse_OPND(OpndStack *S);
Status Traverse_OPTR(OptrStack *S);
    //遍历栈，对从栈顶到栈底的元素逐一调用visit()函数

//-------------------------------------------------------------------------//
Status InitStack_OPND(OpndStack *S)
{   //构建空栈，先分配内存，把首元素地址赋给栈底指针变量，并将栈底指针变量的值赋给栈顶指针变量
    S->base = (OpndType *)malloc(STACK_INIT_SIZE*sizeof(OpndType));
    if(! S->base) exit(-2);
    S->top=S->base;
    S->stacksize=STACK_INIT_SIZE;
    return OK;
} //InitStack

OpndType GetTop_OPND(OpndStack S, OpndType *e1)
{   //获取栈顶元素
    if(S.top == S.base) return ERROR;
    *e1=*(S.top-1);
    return *e1;
} //GetPop

Status Push_OPND(OpndStack *S,OpndType e)
{   //压栈
    if((S->top-S->base)>=S->stacksize)   //如果栈满，追加存储空间
    {
        S->base=(OpndType *)realloc(S->base,(S->stacksize + STACKINCREMENT)*sizeof(OpndType));
        if(!S->base) exit(-2);
        S->top=S->base+S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top) = e;     //将栈顶元素设为e
    S->top++;          //将栈顶指针向上挪动
    return OK;
} //Push

Status Pop_OPND(OpndStack *S,OpndType *e)
{   //弹栈
    if(S->top==S->base) return ERROR;
    S->top--;          //将栈顶指针向下挪动
    *e = *(S->top);
    return OK;
} //Pop

Status StackEmpty_OPND(OpndStack S, OpndType *e)
{   //判别栈是否为空
    if(S.top==S.base)
        return  TRUE;
    else
        return FALSE;
} //StackEmpty

void PrintStack_OPND(OpndStack *S)
{   //打印栈元素
    if(S->top==S->base) printf("栈为空\n");
    else
    {
        OpndType * pBase = S->base;
        while(pBase!=S->top)
        {
            printf("%6.2f ",*pBase);
            pBase++;
        }
    }
} //PrintStack

int StackLength_OPND(OpndStack S)
{   //获取栈元素个数
    int len=S.top-S.base;
    printf("\n%d\n",len);
    return len;
} //StackLength

Status ClearStack_OPND(OpndStack *S)
{   //清空栈元素
    OpndType e;
    while(S->top!=S->base)
    {
        Pop_OPND(S, &e);
    }
    return OK;
} //ClearStack

Status Traverse_OPND(OpndStack *S)
{
    int len=StackLength_OPND(*S);
    OpndType *opnd=S->base;
    for(int i=0;i<len;i++)
    {
        printf("%f ",*opnd);
        ++opnd;
    }
    printf("\n");
    return OK;
} //Traverse
//-------------------------------------------------------------------------//
Status InitStack_OPTR(OptrStack *S)
{
    S->base = (OptrType *)malloc(STACK_INIT_SIZE*sizeof(OptrType));
    if(! S->base) exit(-2);
    S->top=S->base;
    S->stacksize=STACK_INIT_SIZE;
    //printf("\nOK\n");
    return OK;
} //InitStack

OptrType GetTop_OPTR(OptrStack S, OptrType *e2)
{
    if(S.top == S.base) return ERROR;
    *e2=*(S.top-1);
    return *e2;
} //GetPop

Status Push_OPTR(OptrStack *S,OptrType e)
{
    if((S->top-S->base)>=S->stacksize)   //如果栈满，追加存储空间
    {
        S->base=(OptrType *)realloc(S->base,(S->stacksize + STACKINCREMENT)*sizeof(OptrType));
        if(!S->base) exit(-2);
        S->top=S->base+S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top) = e;
    S->top++;
    return OK;
} //Push_OPTR

Status Pop_OPTR(OptrStack *S,OptrType *e)
{
    if(S->top==S->base) return ERROR;
    S->top--;
    *e = *(S->top);
    return OK;
} //Pop_OPTR

Status StackEmpty_OPTR(OptrStack S, OptrType *e)
{
    if(S.top==S.base)
        return  TRUE;
    else
        return FALSE;
} //StackEmpty_OPTR

void PrintStack_OPTR(OptrStack *S)
{
    if(S->top==S->base) printf("栈为空\n");
    else
    {
        OptrType * pBase = S->base;
        while(pBase!=S->top)
        {
            printf("%c ",*pBase);
            pBase++;
        }
    }
} //PrintStack_OPTR

int StackLength_OPTR(OptrStack S)
{
    int len=S.top-S.base;
    //printf("\n%d\n",len);
    return len;
} //StackLength_OPTR

Status ClearStack_OPTR(OptrStack *S)
{
    Status Pop_OPTR(OptrStack *S,OptrType *e);
    OptrType e;
    while(S->top!=S->base)
    {
        Pop_OPTR(S, &e);
    }
    return OK;
} //ClearStack_OPTR

Status Traverse_OPTR(OptrStack *S)
{
    int len=StackLength_OPTR(*S);
    OptrType *optr=S->base;
    for(int i=0;i<len;i++)
    {
        printf("%c ",*optr);
        ++optr;
    }
    printf("\n");
    return OK;
} //Traverse

#endif /* Operations_h */
