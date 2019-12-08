//
//  Functions.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/17.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Functions_h
#define Functions_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

#define VARNAMESIZE  15        //变量的大小限制
#define FUNCSIZE     100       //函数存储空间的分配量

typedef int StatusWH;

typedef struct FuncUnit {
    char      funcname[VARNAMESIZE];  //用于存放函数名
    char             func[FUNCSIZE];  //用于存放函数体
    char           var[VARNAMESIZE];  //存放变量名
    int                    FuncType;  //函数类型：1-简单函数，2-复合函数
    char         compo[VARNAMESIZE];  //若为复合函数，则用来存放被调用的函数名
    struct FuncUnit      *next;  //指针域
} FuncUnit;    //一个函数
typedef struct {
    int              length;   //储藏的函数个数
    FuncUnit         * head;   //头指针
} FuncWH;     //函数库

//===================================基本操作=================================//
StatusWH InitFuncWH(FuncWH * pFuncWH);
    //初始化用于存放函数的链表
FuncUnit * CreateFuncUnit(char * FuncName, char * Func, char * VarName);
    //
StatusWH InFuncWH(FuncWH * pFuncWH, FuncUnit * Func);
StatusWH PushHeadWH(FuncWH * pFuncWH, FuncUnit *Func);
StatusWH PushTailWH(FuncWH * pFuncWH, FuncUnit *Func);
StatusWH DeleteHeadWH(FuncWH * pFuncWH);
StatusWH DeleteTailWH(FuncWH * pFuncWH);
StatusWH InsertFuncWH(FuncWH * pFuncWH, int i, FuncUnit *Func);
StatusWH DeleteFuncWH(FuncWH * pFuncWH, int i);
StatusWH PrintFuncWH(FuncWH * pFuncWH);
int LengthFuncWH(FuncWH * pFuncWH);
StatusWH ClearFuncWH(FuncWH * pFuncWH);
StatusWH DestroyFuncWH(FuncWH * pFuncWH);
StatusWH EmptyFuncWH(FuncWH FuncWH);
FuncUnit * GetFuncUnit(FuncWH * pFuncWH, int i);
Status GetSubFunc(char * Func, char * SubFuncName, char * SubFunc);
FuncUnit * DefSimple(void);
    //定义简单函数
float RunSimple(FuncUnit * Func);
    //运行简单函数
FuncUnit * DefCompo(FuncWH FuncWH);
    //定义复合函数
float RunCompo(FuncUnit * Func, FuncWH FuncWH);
    //运行复合函数
//===================================衍生操作=================================//
StatusWH FuncWHMgmt(FuncWH * pFuncWH, FuncUnit * FuncUnit, int method);

StatusWH InitFuncWH(FuncWH * pFuncWH)
{   //初始化用于存放函数的链表
    pFuncWH->head = (FuncUnit *)malloc(sizeof(FuncUnit));
    pFuncWH->head->next = NULL;
    pFuncWH->length = 0;
    return OK;
} //InitFuncWH

FuncUnit * CreateFuncUnit(char * FuncName, char * Func, char * VarName)
{   //创建新的函数
    FuncUnit *pNew=(FuncUnit *)malloc(sizeof(FuncUnit));   //为新结点开辟空间
    memset(pNew->funcname,0,VARNAMESIZE);memset(pNew->func,0,FUNCSIZE);memset(pNew->var,0,VARNAMESIZE);
    char *pFuncName=FuncName;char *pfuncname=pNew->funcname;
    while((*pfuncname++=*pFuncName++));
    char *pFunc=Func;char *pfunc=pNew->func;
    while((*pfunc++=*pFunc++));  //将输入的函数复制到函数库的单元格中
    char *pVar=VarName;char *pvar=pNew->var;
    while((*pvar++=*pVar++));
    pNew->next=NULL;
    return pNew;
} //CreateFuncUnit

StatusWH InFuncWH(FuncWH * pFuncWH, FuncUnit * Func)
{   //代码有问题
    FuncUnit * pTemp = pFuncWH->head->next;  //pTemp指向函数仓库中的第一个函数
    int len=pFuncWH->length;
    if (len == 0)
    {
        printf("函数仓库中尚未储存任何函数，无法进行比较\n");
        return ERROR;
    }
    else
    {
        int match=0; //计算是否匹配
        while (pTemp != NULL)
        {
            char ch='#';int m,n;
            for(n=m=0;(pTemp->func[n]=pTemp->func[m]);m++)//防止因为发生运算使得函数表达式变化
                if(pTemp->func[n]-ch) n++;
            if(*Func->func==*(pTemp->func)) match+=1;
            pTemp = pTemp->next;
        }
        if(match==0) return FALSE;
        else return TRUE;
    }
} //InFuncWH

StatusWH PushHeadWH(FuncWH * pFuncWH, FuncUnit *Func)
{   //在函数库头部插入函数
    FuncUnit *pn = CreateFuncUnit(Func->funcname, Func->func, Func->var);  //创建项
    if(pFuncWH->length==0)
        pFuncWH->head->next=pn;
    else
    {
        pn->next=pFuncWH->head->next;
        pFuncWH->head->next=pn;
    }
    pFuncWH->length++;
    return OK;
} //PushHeadWH

StatusWH PushTailWH(FuncWH * pFuncWH, FuncUnit *Func)
{   //在函数仓库尾部插入函数
    FuncUnit *pn = pFuncWH->head;       //找到最后一个结点
    while (pn->next != NULL)
    {
        pn = pn->next;
    }
    //插入项
    FuncUnit *pm = CreateFuncUnit(Func->funcname, Func->func, Func->var);
    pn->next = pm;
    pFuncWH->length++;
    return OK;
} //PushTailWH

StatusWH DeleteHeadWH(FuncWH * pFuncWH)
{   //删除函数仓库头部的函数
    FuncUnit *pn = pFuncWH->head->next;           //把要删除的结点保存起来
    pFuncWH->head->next = pFuncWH->head->next->next;    //连接它的前结点与后结点
    free(pn);//释放并置为空
    pn = NULL;
    pFuncWH->length--;
    return OK;
} //DeleteHeadWH

StatusWH DeleteTailWH(FuncWH * pFuncWH)
{   //删除函数仓库尾部的函数
    FuncUnit *pn = pFuncWH->head;          //第一个结点
    while (pn->next->next != NULL)
    {
        pn = pn->next;
    }
    FuncUnit *pm = pn->next;
    pn->next = NULL;
    free(pm);
    pm = NULL;
    pFuncWH->length--;
    return OK;
} //DeleteTailWH

StatusWH InsertFuncWH(FuncWH * pFuncWH, int i, FuncUnit *Func)
{   //在任意位置插入函数
    //判断插入位置是否合法
    if (i<1 || i>pFuncWH->length)  //若插入位置不合法，则插入到尾部
    {
        //printf("插入位置不合法，插入到尾部\n");
        PushTailWH(pFuncWH, Func);
        return OK;
    }
    //头插
    else if (i == 1)
    {
        FuncUnit *pn = CreateFuncUnit(Func->funcname, Func->func, Func->var);
        pn->next = pFuncWH->head->next;
        pFuncWH->head->next = pn;
        pFuncWH->length++;
        return OK;
    }
    //中间任意位置插入
    else
    {
        FuncUnit *pn = CreateFuncUnit(Func->funcname, Func->func, Func->var);
        FuncUnit *pm = pFuncWH->head->next;
        for (int j=1; j<i-1; j++)
        {
            pm = pm->next;
        }
        pn->next = pm->next;
        pm->next = pn;
        pFuncWH->length++;
        return OK;
    }
} //InsertFuncWH

StatusWH DeleteFuncWH(FuncWH * pFuncWH, int i)
{   //删除函数仓库中的任意函数
    //判断删除位置是否合法
    if (i<1 || i>pFuncWH->length)       //如果位置不合法，则删除最后一项
    {
        DeleteTailWH(pFuncWH);
        return OK;
    }
    FuncUnit *pn = pFuncWH->head->next;
    if (i == 1)     //头删
    {
        pFuncWH->head->next = pFuncWH->head->next->next;
        free(pn);
        pFuncWH->length--;
        return OK;
    }
    //任意位置删除
    for (int j = 1; j < i-1; j++)
    {
        pn = pn->next;
    }
    FuncUnit *pm = pn->next;
    pn->next = pn->next->next;
    free(pm);
    pm = NULL;
    pFuncWH->length--;
    //printf("删除成功，第%d个元素被删除\n",i);
    return OK;
} //DeleteFuncWH

StatusWH PrintFunc(FuncUnit *Func)
{
    printf("%s(%s)=%s\n",Func->funcname,Func->var,Func->func);
    return OK;
} //PrintFunc

StatusWH PrintFuncWH(FuncWH * pFuncWH)
{
    FuncUnit *pTemp = pFuncWH->head->next;
    int i=1;
    int len=pFuncWH->length;
    if (len == 0)
    {
        printf("函数仓库中尚未储存任何函数\n");
        return ERROR;
    }
    else
    {
        while (pTemp != NULL)
        {
            char ch='#';int m,n;
            for(n=m=0;(pTemp->func[n]=pTemp->func[m]);m++)//防止因为发生运算使得函数表达式变化
                if(pTemp->func[n]-ch) n++;
            printf("%d. %s(%s)=%s\n", i, pTemp->funcname,pTemp->var,pTemp->func);
            pTemp = pTemp->next;++i;
        }
        printf("\n");
        return OK;
    }
} //PrintFuncWH

StatusWH ClearFuncWH(FuncWH * pFuncWH)
{   //清空函数仓库
    int len=pFuncWH->length;
    for(int i=1;i<len+1;i++)
        DeleteFuncWH(pFuncWH, i);
    return OK;
} //ClearFuncWH

StatusWH DestroyFuncWH(FuncWH * pFuncWH)
{   //销毁函数仓库
    while (pFuncWH->head != NULL)
    {
        FuncUnit *pn = pFuncWH->head;
        pFuncWH->head = pFuncWH->head->next;
        free(pn);
        pn = NULL;
        pFuncWH->length--;
    }
    //printf("函数仓库销毁成功\n");
    return OK;
} //DestroyFuncWH

StatusWH EmptyFuncWH(FuncWH FuncWH)
{   //判别函数仓库是否为空
    if(FuncWH.length==0)
    {
        //printf("函数仓库为空\n");
        return TRUE;
    }
    else
    {
        //printf("函数仓库不为空");
        return FALSE;
    }
} //EmptyFuncWH

FuncUnit * GetFuncUnit(FuncWH * pFuncWH, int i)
{   //根据用户输入获取函数仓库中的第i个函数
    FuncUnit * Func;
    if (i<1 || i>pFuncWH->length+1)
    {
        printf("选择位置不合法，请重新输入\n");
        return ERROR;
    }
    //头部
    if (i == 1)
    {
        Func = pFuncWH->head->next;
        printf("函数为%s(%s)=%s\n",Func->funcname,Func->var,Func->func);
        return Func;
    }
    //中间任意位置
    else
    {
        FuncUnit *pm = pFuncWH->head->next;
        for (int j=1; j<i; j++)
        {
            pm = pm->next;
        }
        Func = pm;
        printf("函数为%s(%s)=%s\n",Func->funcname,Func->var,Func->func);
        return Func;
    }
} //GetFuncUnit

Status GetSubFunc(char * Func, char * SubFuncName, char * SubFunc)
{   //获取被调用函数信息
    //printf("RUNNING GETSUBFUNC\n");
    char dst[3][50];
    int n=0;
    char spl[]="()";
    char * result=NULL;
    result = strtok(Func, spl);
    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    strcpy(SubFuncName, dst[0]);
    strcpy(SubFunc, dst[1]);
    return OK;
} //GetSubFunc

/*
Status GetStndFunc(char * Func, char * StndFunc)
{
    Terms Terms;
    InitTerms(&Terms);
    
    Fragmentation(Func, &Terms);
    for(int i=0;i<Terms.termNum;i++)
    {
        Terms.termList[i]->termType=TermType(Terms.termList[i]->term);
        printf("No:%d; %s; Type:%d\n",i+1,Terms.termList[i]->term,Terms.termList[i]->termType);
    }
    
    char *s=NULL;
    for(int j=0;j<Terms.termNum;j++)
    {
        if(Terms.termList[j]->termType==2)      //如果是未知变量
        {StndFunc[strlen(StndFunc)]='X';printf("%s\n",StndFunc);}
        else if(Terms.termList[j]->termType==1||Terms.termList[j]->termType==3) //如果是数字或界限符
        {
            s=Terms.termList[j]->term;
            printf("Term: %s\n",s);
            for(int p=0;p<strlen(Terms.termList[j]->term);++p)
            {
                printf("Character: %c\n",*s);
                if(*s=='-')
                    StndFunc[strlen(StndFunc)]='-';
                else
                    StndFunc[strlen(StndFunc)]=*s;
                printf("%s\n",StndFunc);++s;
            }
        }
    }
    return OK;
} //GetStndFunc
 */

FuncUnit * DefSimple()
{
    //printf("RUNNING DEFSIMPLE\n");
    char FuncName[VARNAMESIZE];
    char Func[FUNCSIZE];
    char VarName[VARNAMESIZE];
    printf("正在定义简单函数\n");
    printf("请输入函数名和变量名:\n");
    scanf("%s %s",FuncName,VarName);
    printf("请输入函数体:\n");
    printf("DEF %s(%s)= ",FuncName,VarName);
    scanf("%s",Func);
    FuncUnit * SimpleFunc=CreateFuncUnit(FuncName, Func, VarName);
    SimpleFunc->FuncType=1;  //简单函数
    printf("%s(%s)=%s\n",SimpleFunc->funcname,SimpleFunc->var,SimpleFunc->func);
    return SimpleFunc;
} //DefSimple

float RunSimple(FuncUnit * Func)
{   //运行简单函数
    //printf("RUNNING RUNSIMPLE\n");
    float result=0.0;float VarVal=0.0;
    char input[20];
    memset(input,0,sizeof(input));
    printf("RUN %s(%s)=%s\n",Func->funcname,Func->var,Func->func);
    ConvertStandard(Func->func);
    printf("Please enter a value to define %s\n",Func->var);
    scanf("%s",input);
    printf("RUN %s(%s)\n",Func->funcname,input);
    VarVal=ConvertNum(input);
    result=EvaluateExpression(input,Func->func,1,VarVal);
    return result;
} //RunSimple

FuncUnit * DefCompo(FuncWH FuncWH)
{   //
    //printf("RUNNING DEFCOMPO\n");
    char FuncName[VARNAMESIZE];
    char Func[FUNCSIZE];
    char VarName[VARNAMESIZE];
    int choice=0;
    printf("正在定义复合函数\n");
    printf("历史存储函数有:\n");
    PrintFuncWH(&FuncWH);
    printf("请选择要调用的函数序号：\n");
    scanf("%d",&choice);
    //加入一个报错机制
    FuncUnit * ChosenFunc=GetFuncUnit(&FuncWH, choice);
    //printf("DEF %s(%s)=%s\n",ChosenFunc->funcname,ChosenFunc->var,ChosenFunc->func);
    
    printf("请输入复合函数的函数名和变量名:\n");
    scanf("%s %s",FuncName,VarName);
    printf("请输入函数体:\n");
    printf("DEF COMPO %s(%s(%s))= ",FuncName, ChosenFunc->funcname, VarName);
    scanf("%s",Func);
    FuncUnit * CompoFunc=CreateFuncUnit(FuncName, Func, VarName);
    CompoFunc->FuncType=2;   //复合函数
    strcpy(CompoFunc->compo, ChosenFunc->funcname);  //调用的函数名
    printf("%s(%s(%s))=%s\n",CompoFunc->funcname,ChosenFunc->funcname,CompoFunc->var,CompoFunc->func);
    return CompoFunc;
} //DefCompo

float RunCompo(FuncUnit * Func, FuncWH FuncWH)
{   //运行复合函数，获得三个参数用于计算
    //printf("RUNNING RUNCOMPO\n");
    char MainFunc[50];char SubFunc[50];    //主函数以及被调用的函数
    memset(MainFunc,0,sizeof(MainFunc));
    memset(SubFunc,0,sizeof(SubFunc));
    strcpy(MainFunc, Func->func);
    FuncUnit *pTemp;
    for(int i=0;i<FuncWH.length;++i)
    {
        pTemp=GetFuncUnit(&FuncWH, i+1);
        //printf("%s\n",pTemp->func);
        if(strcmp(Func->compo,pTemp->funcname)==0)
        {
            strcpy(SubFunc, pTemp->func);
            //printf("MATCH!\n");
        }
    }

    float result=0.0;float VarVal=0.0;     //存放结果和转换后的变量值
    char input[20];                        //用来存放用户输入
    memset(input,0,sizeof(input));         //初始化
    
    printf("Please enter a value to define %s\n",Func->var);  //用户输入赋予函数值
    scanf("%s",input);
    VarVal=ConvertNum(input);                                 //转换成可运算的浮点数

    printf("RUN COMPO %s(%s(%s))=%s\n",Func->funcname,Func->compo,input,Func->func);
    ConvertStandard(MainFunc);
    result=EvaluateExpression(SubFunc, MainFunc, 2, VarVal);
    
    return result;
} //RunCompo

void PrintMenuB()
{
    printf("Please choose a method to manipulate the function store:\n");
    printf("1. Show the stored functions\n");
    printf("3. Define and store a function\n");
    printf("4. Delete a stored function\n");
    printf("5. Clear the function store\n");
}

#endif /* Functions_h */
