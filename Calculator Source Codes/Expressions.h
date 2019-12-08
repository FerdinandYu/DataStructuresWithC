//
//  Expressions.h
//  NewTest
//
//  Created by 余宏昊 on 2018/11/16.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#ifndef Expressions_h
#define Expressions_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "Variables.h"
#include "Operations.h"


#define TERMNUM 100

typedef int Status;
typedef struct {
    char    term[30];
    int     termType;    //
} Term;
typedef struct {
    Term   *termList[TERMNUM];
    int               termNum;
} Terms;

char digit[]=".0123456789";
char letter[]="_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char delimiter[]="+-*^/()#";

//===================================运算操作=================================//
void ConvertStandard(char *expression);
    //将用户输入的表达式转化为标准的算术式
Status In(char c,char OP[]);
    //判断输入的c是否属于某一字符数组
Status InitTerm(Term * Term);
    //对项进行初始化操作
Status InitTerms(Terms * Terms);
    //对项库进行初始化操作
int TermType(char * Term);
    //返回项的类别，1-运算数，2-未知数，3-运算符
Status Fragmentation(char *expression, Terms * Terms);
    //对表达式进行分割处理，将各项存入项库
OptrType Precede(OptrType a, OptrType b);
    //比较a和b的优先级
OpndType Operate(OpndType a, OptrType theta, OpndType b);
    //基本的加减乘除幂操作
OpndType GetVal(char *c, Vars * Vars);
    //获取用户输入的数字
OpndType EvaluateExpression(char * Func, char *expression, int IsFunc, float AssignedVal);
    //解析表达式，进行运算
//===================================辅助操作=================================//
void PrintMenuA(void);
void PrintMenuB(void);
void PrintMenuC(void);

//----------------------------------------------------------------------------------//
void ConvertStandard(char *expression)
{
    //将字符串表达式标准化为算术表达式
    char *p=expression,*q;
    while(*p!='\0')
    {   //遍历字符串
        if(*p==' ')
        {   //如果是空格，删除
            q=p;
            for(;*q!='\0';q++)
                *q=*(q+1);
        }
        p++;
    }
    *p++='#';  //在表达式末尾加上'#'以与最先压入算符栈的'#'匹配
    *p='\0';
} //ConvertStandard

Status In(char c,char op[])
{   //判定字符c是否在字符数组OP中，返回TRUE或者FALSE
    char *p;
    p=op;
    while(*p != '\0')  //遍历字符数组OP
    {
        if(c == *p)
            return TRUE;
        p++;
    }
    return FALSE;
} //In

Status InitTerm(Term * Term)
{   //对项进行初始化操作
    Term->termType=0;
    memset(Term->term,0,sizeof(Term->term));
    return OK;
} //InitTerm

Status InitTerms(Terms * Terms)
{   //对项库进行初始化操作
    Terms->termNum=0;
    memset(Terms->termList,0,sizeof(Terms->termList));
    for(int i=0;i<TERMNUM;++i)
    {
        Term * newTerm=(Term *)malloc(sizeof(Term));
        if(!newTerm) return ERROR;
        InitTerm(newTerm);        //初始化字符串
        Terms->termList[i]=newTerm;
    }
    return OK;
} //InitTerms

Status Fragmentation(char *expression, Terms *Terms)
{   //对表达式进行分割处理，将各项存入项库
    char *c=expression;
    char *c_pre=c;
    char *c_post=c;
    
    //处理首元素，构建一个项
    Terms->termList[0]->term[0]=*c;
    //printf("%c\n",*c);
    Terms->termNum+=1;
    ++c;
    
    //处理后面的元素
    for(;*c!='\0';++c)
    {
        c_pre=c-1;c_post=c+1;
        //---------------------------------------------------------//
        if(*c=='-')  //如果当前元素为'-'
        {
            if(*c_pre!='('&&*c_pre!='e'&&*c_pre!='E')   //如果前一个元素不为'('且不为e或E，则分割为减号项
            {
                //printf("RUNNING FRAGMENTATION A1\n");
                //printf("%c\n",*c);
                Terms->termList[Terms->termNum]->term[strlen(Terms->termList[Terms->termNum]->term)]='-';
                Terms->termNum+=1;
            }
            else if(*c_pre=='(')   //如果前一个元素为'('，则分割为负数项，并且将括号删除
            {
                //printf("RUNNING FRAGMENTATION A2\n");
                //printf("%c\n",*c);
                Terms->termList[Terms->termNum-1]->term[0]='-';
                ++c;
                for(;*c!=')';++c)
                {
                    Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                }
                //printf("%c\n",*c);
            }
            else if(*c_pre=='e'||*c_pre=='E')     //如果前一个元素为e或E
            {
                char * c_prepre=c_pre-1;
                if(In(*c_prepre, digit))  //如果前前元素为数字，则分割为用科学计数法表示的常数
                {
                    //printf("RUNNING FRAGMENTATION A3-1\n");
                    //printf("%c\n",*c);
                    Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                    ++c;
                    for(;!In(*c, delimiter);++c)
                    {
                        //printf("%c\n",*c);
                        Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                    }
                    --c; //回溯指针
                }
                else                      //如果前前元素不是数字，则分割为减号项
                {
                    //printf("RUNNING FRAGMENTATION A3-2\n");
                    //printf("%c\n",*c);
                    Terms->termList[Terms->termNum]->term[strlen(Terms->termList[Terms->termNum]->term)]=*c;
                    Terms->termNum+=1;
                }
            }
        }
        //---------------------------------------------------------//
        else if((In(*c, delimiter)&&*c!='-'))   //如果为界限符
        {
            if(*c=='('&&(In(*c_pre,digit)||In(*c_pre,letter)))  //函数体
            {
                //printf("RUNNING FRAGMENTATION B-1\n");
                //printf("%c\n",*c);
                //Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                for(;*c!=')';++c)
                {
                    //printf("%c\n",*c);
                    Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                }
                Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
                //--c;      //指针回溯一个元素
            }
            else
            {
                //printf("RUNNING FRAGMENTATION B-2\n");
                //printf("%c\n",*c);
                Terms->termList[Terms->termNum]->term[strlen(Terms->termList[Terms->termNum]->term)]=*c;
                Terms->termNum+=1;
            }
        }
        //---------------------------------------------------------//
        else if(In(*c,digit)||In(*c,letter))              //如果当前字符为标识符
        {
            if(In(*c_pre, delimiter))       //如果前一个字符为界限符
            {
                //printf("RUNNING FRAGMENTATION C-1\n");
                //printf("%c\n",*c);
                Terms->termList[Terms->termNum]->term[0]=*c;
                Terms->termNum+=1;
            }
            else if(In(*c_pre, digit)||In(*c_pre,letter))    //如果前一个字符为标识符或数字或小数点
            {
                //printf("RUNNING FRAGMENTATION C-2\n");
                //printf("%c\n",*c);
                Terms->termList[Terms->termNum-1]->term[strlen(Terms->termList[Terms->termNum-1]->term)]=*c;
            }
        }
    }
    return OK;
} //Fragmentation

int TermType(char * Term)
{   //1-运算数，2-未知数，3-运算符，4-函数体
    char * c=Term;
    int type=0;
    unsigned long len=strlen(Term);
    if(In(*c,delimiter)&&strlen(c)==1)
        {type=3;}
    else
    {
        int IsLetter=0;
        int IsE=0;
        int IsMinus=0;
        int IsDigit=0;
        int LeftBracket=0,RightBracket=0;
        for(int i=0;i<len;++i)
        {
            if(In(*c,digit))
                {IsDigit+=1;++c;}
            else if(In(*c,letter))
            {
                if(*c=='e'||*c=='E') {IsE+=1;IsLetter+=1;}
                else IsLetter+=1;
                ++c;
            }
            else if(*c=='-')
                {IsMinus+=1;++c;}
            else if(*c=='(')
                {LeftBracket+=1;++c;}
            else if(*c==')')
                {RightBracket+=1;++c;}
            else ++c;
        }
        //printf("Letters:%d;Es:%d;Minus:%d;Digits:%d\n",IsLetter,IsE,IsMinus,IsDigit);
        c=Term;  //指针回溯
        if(IsDigit==strlen(c))   //纯数字
            type=1;
        else if((IsMinus==1)&&(IsDigit==strlen(c)-1)) //负数
            type=1;
        else if(IsLetter==1&&IsE==1&&IsMinus<=2)  //科学计数法
            type=1;
        else if(LeftBracket>=1&&LeftBracket==RightBracket)
            type=4;     //函数体
        else type=2;    //未知变量
    }
    return type;
} //TermType

OptrType Precede(OptrType a, OptrType b)
{   //判断运算符之间的优先级关系
    int i = 0,j = 0;
    char pre[8][8]={
        /*运算符之间的优先级制作成一张表格*/
             /*  +   -   *   /   ^   (   )   #  */
        /* + */{'>','>','<','<','<','<','>','>'},
        /* - */{'>','>','<','<','<','<','>','>'},
        /* * */{'>','>','>','>','<','<','>','>'},
        /* / */{'>','>','>','>','<','<','>','>'},
        /* ^ */{'>','>','>','>','>','<','>','>'},
        /* ( */{'<','<','<','<','<','<','=','0'},
        /* ) */{'>','>','>','>','>','0','>','>'},
        /* # */{'<','<','<','<','<','<','0','='}};
    switch(a){
        case '+': i=0; break;
        case '-': i=1; break;
        case '*': i=2; break;
        case '/': i=3; break;
        case '^': i=4; break;
        case '(': i=5; break;
        case ')': i=6; break;
        case '#': i=7; break;
    }
    switch(b){
        case '+': j=0; break;
        case '-': j=1; break;
        case '*': j=2; break;
        case '/': j=3; break;
        case '^': j=4; break;
        case '(': j=5; break;
        case ')': j=6; break;
        case '#': j=7; break;
    }
    return pre[i][j];      //运用在矩阵中的位置带回优先级关系'<','>'或'='
} //Precede

OpndType Operate(OpndType a, OptrType theta, OpndType b)
{   //基本运算
    OpndType result=0;
    
    switch(theta)   {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if(b==0)
            {
                printf("ERROR:除数不能为0\n");
                exit(0);
            }
            result = a / b;
            break;
        case '^': result = powf(a,b); break;
        default:
            printf("Is not an operator.\n");
            break;
    }
    return result;
} //Operate

Status GetSubTerm(char * Func, char * SubFuncName, char * SubFunc)
{
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
} //GetSubTerm

OpndType EvaluateExpression(char * Func, char *expression, int IsFunc, float AssignedVal)
{   //操作数为1，操作符为2，未知变量为3，函数体为4
    //IsFunc为0表示算术表达式，为1表示简单函数，为2表示复合函数
    //Func为复合函数中被调用的函数
    OpndStack OPND; OptrStack OPTR; //运算数、运算符栈
    Terms Terms;
    Vars Vars; //变量库
    OptrType theta, x, e1;
    OpndType a,b,num=0.0,result=0;
    
    InitVars(&Vars);InitTerms(&Terms);
    InitStack_OPND(&OPND);
    InitStack_OPTR(&OPTR); Push_OPTR(&OPTR, '#'); //向运算符栈推入'#'
    
    Fragmentation(expression, &Terms);
    for(int i=0;i<Terms.termNum;i++)
    {
        Terms.termList[i]->termType=TermType(Terms.termList[i]->term);
        //printf("No:%d; %s; Type:%d\n",i+1,Terms.termList[i]->term,Terms.termList[i]->termType);
    }

    //---------------------------------表达式求值------------------------------//
    //处理第一项
    //printf("第1项：%c;类型：%d\n",*Terms.termList[0]->term,Terms.termList[0]->termType);
    if(Terms.termList[0]->termType==3)  //运算符，压入运算符栈
    {
        //printf("RUNNING EVALUATION A1\n");
        Push_OPTR(&OPTR, *Terms.termList[0]->term);
    }
    else if(Terms.termList[0]->termType==1)  //常数，压入运算数栈
    {
        //printf("RUNNING EVALUATION A2\n");
        num=ConvertNum(Terms.termList[0]->term);Push_OPND(&OPND, num);
    }
    else if(Terms.termList[0]->termType==2)  //未知数，赋值并压入运算数栈
    {
        if(IsFunc==0)          //算术表达式
        {
            //printf("RUNNING EVALUATION A3-1\n");
            num=AssignVar(&Vars, Terms.termList[0]->term);
        }
        else if(IsFunc==1||IsFunc==2)     //函数解析式
        {
            //printf("RUNNING EVALUATION A3-2\n");
            num=AssignedVal;
        }
        Push_OPND(&OPND, num);
    }
    else if(Terms.termList[0]->termType==4)   //如果是函数体
    {
        //printf("RUNNING EVALUATION A4\n");
        char subfuncname[10];char subfuncbody[50];char chosenfuncbody[50];
        memset(subfuncname,0,sizeof(subfuncname));
        memset(subfuncbody,0,sizeof(subfuncbody));
        memset(chosenfuncbody,0,sizeof(chosenfuncbody));
        GetSubTerm(Terms.termList[0]->term, subfuncname, subfuncbody);
        ConvertStandard(subfuncbody);
        num=EvaluateExpression(Func, subfuncbody, IsFunc, AssignedVal);
        //printf("Inner Value: %f\n",num);
        strcpy(chosenfuncbody, Func);
        ConvertStandard(chosenfuncbody);
        num=EvaluateExpression(Func, chosenfuncbody, IsFunc, num);
        //printf("Final Value: %f\n",num);
        Push_OPND(&OPND, num);
        //printf("Operating the No.1 function term:\n");
        //printf("OPND Stack: ");PrintStack_OPND(&OPND);printf("\n");
        //printf("OPTR Stack: ");PrintStack_OPTR(&OPTR);printf("\n");
    }
    
    //处理后面的项
    for(int i=1;i<Terms.termNum;i++)
    {
        if(Terms.termList[i]->termType==1)         //常数
        {
            //printf("RUNNING EVALUATION B1\n");
            num=ConvertNum(Terms.termList[i]->term);Push_OPND(&OPND, num);
        }
        else if(Terms.termList[i]->termType==2)    //未知数
        {
            if(IsFunc==0)
            {
                //printf("RUNNING EVALUATION B2-1\n");
                num=AssignVar(&Vars, Terms.termList[i]->term);
            }
            else if(IsFunc==1||IsFunc==2)
            {
                //printf("RUNNING EVALUATION B2-2\n");
                num=AssignedVal;
            }
            Push_OPND(&OPND, num);
        }
        else if(Terms.termList[i]->termType==4)   //如果是函数体
        {
            //printf("RUNNING EVALUATION B3\n");
            char subfuncname[10];char subfuncbody[50];char chosenfuncbody[50];
            memset(subfuncname,0,sizeof(subfuncname));
            memset(subfuncbody,0,sizeof(subfuncbody));
            memset(chosenfuncbody,0,sizeof(chosenfuncbody));
            GetSubTerm(Terms.termList[i]->term, subfuncname, subfuncbody);
            ConvertStandard(subfuncbody);
            num=EvaluateExpression(Func, subfuncbody, IsFunc, AssignedVal);
            //printf("Inner Value: %f\n",num);
            strcpy(chosenfuncbody, Func);
            ConvertStandard(chosenfuncbody);
            num=EvaluateExpression(Func, chosenfuncbody, IsFunc, num);
            //printf("Final Value: %f\n",num);
            Push_OPND(&OPND, num);
            //printf("Operating the No.%d function term:\n",i);
            //printf("OPND Stack: ");PrintStack_OPND(&OPND);printf("\n");
            //printf("OPTR Stack: ");PrintStack_OPTR(&OPTR);printf("\n");
        }
        else                                       //运算符
        {
            switch(Precede(GetTop_OPTR(OPTR,&e1),*Terms.termList[i]->term))  //根据c与运算符栈栈顶元素的优先级关系进行操作
            {
                case '<':  //如果c优先于栈顶元素，则将c压入运算符栈
                    //printf("RUNNING EVALUATION B4-1\n");
                    //if((*Terms.termList[i]->term=='-')&&(*Terms.termList[i-1]->term=='('))
                    //{Push_OPND(&OPND, 0);}
                    Push_OPTR(&OPTR,*Terms.termList[i]->term);
                    break;
                case '=':  //如果c与栈顶元素平级，则用x带回运算符栈栈顶元素
                    //printf("RUNNING EVALUATION B4-2\n");
                    if(*Terms.termList[i]->term=='#')
                        Push_OPTR(&OPTR, *Terms.termList[i]->term);
                    Pop_OPTR(&OPTR,&x);
                    break;
                case '>':  //如果栈顶元素优先于c
                    //printf("RUNNING EVALUATION B4-3\n");
                    Pop_OPTR(&OPTR,&theta);  //用theta带回运算符栈顶元素
                    Pop_OPND(&OPND,&b);      //用a,b带回运算数栈栈顶的两个元素
                    Pop_OPND(&OPND,&a);
                    //printf("%f %c %f\n",a,theta,b);
                    result=Operate(a,theta,b); //进行运算
                    //printf("Temp result is: %f\n",result);
                    Push_OPND(&OPND,result);   //将运算结果压入运算数栈
                    i--; //回溯一个项
                    break;
                default:
                    break;
            }//switch
        }//else
    }//for loop
    //PrintStack_OPND(&OPND);
    //PrintStack_OPTR(&OPTR);
    GetTop_OPND(OPND,&result);    //用result带回运算数栈栈顶元素，即运算结果
    return result;
}

void PrintMenuA()
{
    printf("Please enter the number to choose an operation from the following:\n");
    printf("1. Enter an expression and do the calculations\n");
    printf("2. Define a function\n");
    printf("3. Use a defined function to do calculations\n");
    printf("4. Manipulate the function store\n");
}

#endif /* Expressions_h */
