//
//  main.c
//  NewTest
//
//  Created by 余宏昊 on 2018/11/16.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Vectors.h"
#include "PolyNSq.h"
#include "PolyNLk.h"
#include "Expressions.h"
#include "Functions.h"
#include "Matrix.h"

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define INFEASIBLE -1

int main()
{
    FuncWH FuncWH;
    InitFuncWH(&FuncWH);
    
    float result=0.0;
    FuncUnit * newFunc1=DefSimple();
    PushHeadWH(&FuncWH, newFunc1);
    result=RunSimple(newFunc1);
    printf("结果为%4.2f\n",result);
    
    FuncUnit * newFunc2=DefCompo(FuncWH);
    PushHeadWH(&FuncWH, newFunc2);
    result=RunCompo(newFunc2, FuncWH);
    printf("结果为%4.2f\n",result);
    
    FuncUnit * newFunc3=DefSimple();
    PushHeadWH(&FuncWH, newFunc3);
    
    FuncUnit * newFunc4=DefCompo(FuncWH);
    PushHeadWH(&FuncWH, newFunc4);
    result=RunCompo(newFunc4, FuncWH);
    printf("结果为%4.2f\n",result);
    
    TSMatrix A, E, Q1, Q2, Q3;
    InitSMatrix(&A);InitSMatrix(&E);
    InitSMatrix(&Q1);InitSMatrix(&Q2);InitSMatrix(&Q3);
    CreateSMatrix(&A);
    printf("矩阵A为：\n");
    PrintSMatrix(A);
    printf("\n\n");
    CreateSMatrix(&E);
    printf("矩阵E为：\n");
    PrintSMatrix(E);
    printf("\n\n");
    
    AddSMatrix(A, E, &Q1);
    printf("A+E= \n");
    PrintSMatrix(Q1);
    printf("\n\n");
    
    MultSMatrix(A, Q1, &Q2);
    printf("A*(A+E)= \n");
    PrintSMatrix(Q2);
    printf("\n\n");
    
    SubtSMatrix(Q2, A, &Q3);
    printf("A*(A+E)-A= \n");
    PrintSMatrix(Q3);
    printf("\n\n");
    
    result=DetVal(A);
    printf("矩阵A对应的行列式的值为:%4.2f\n",result);
    
    DestroySMatrix(&Q3);
    TransposeSMatrix(A, &Q3);
    printf("对矩阵A进行转置得:\n");
    PrintSMatrix(Q3);
    
    
    /*
    //float result=0.0;
    TSMatrix M,N,P,T,Q;
    InitSMatrix(&M);InitSMatrix(&N);InitSMatrix(&P);InitSMatrix(&T);InitSMatrix(&Q);
    CreateSMatrix(&M);
    PrintSMatrix(M);
    result=DetVal(M);
    //SolveDet(M, &result);
    printf("The result of the determinant is: %f\n",result);
    */
    
    /*
    printf("\n\n");
    //CopySMatrix(M, &T);
    //PrintSMatrix(T);
    //printf("\n\n");
    //AddSMatrix(M, T, &Q);
    //PrintSMatrix(Q);
    //printf("\n\n");
    //ResetSMatrix(&Q);
    //PrintSMatrix(Q);
    //printf("\n\n");
    //DestroySMatrix(&Q);
    CreateSMatrix(&N);
    PrintSMatrix(N);
    printf("\n\n");
    AddSMatrix(M, N, &Q);
    PrintSMatrix(Q);
    printf("\n\n");
    ResetSMatrix(&Q);
    SubtSMatrix(M, N, &Q);
    PrintSMatrix(Q);
    printf("\n\n");
    DestroySMatrix(&Q);
    CreateSMatrix(&P);
    PrintSMatrix(P);
    printf("\n\n");
    MultSMatrix(M, P, &Q);
    PrintSMatrix(Q);
    printf("\n\n");
    TransposeSMatrix(Q, &T);
    PrintSMatrix(T);
    printf("\n\n");
    //MultSMatrix(M, N, &Q);
    //PrintSMatrix(Q);
    //printf("\n\n");
    //TransposeSMatrix(Q, &T);
    //PrintSMatrix(T);
    //printf("\n\n");
    */
    
    /*
    FuncWH FuncWH;
    InitFuncWH(&FuncWH);
    FuncUnit * newFunc1=DefSimple();
    PushHeadWH(&FuncWH, newFunc1);
    FuncUnit * newFunc2=DefSimple();
    PushHeadWH(&FuncWH, newFunc2);
    PrintFuncWH(&FuncWH);
    printf("请选择一个函数(序号)进行运算:\n");
    int choice=0;
    scanf("%d",&choice);
    FuncUnit * chosenFunc = GetFuncUnit(&FuncWH, choice);
    //float result=0.0;
    float result1=RunSimple(chosenFunc);
    printf("%f\n",result1);
    FuncUnit * newFunc3=DefCompo(FuncWH);
    PushHeadWH(&FuncWH, newFunc3);
    PrintFuncWH(&FuncWH);
    printf("\n");
    float result2=RunCompo(newFunc3, FuncWH);
    printf("The Final Result is: %f\n",result2);
     */
    
    /*
    char Func[]="6*int+7/float-1.6E-3+1";
    printf("%s\n",Func);
    char StndFunc[50];
    //GetStndFunc(Func, StndFunc);
    printf("%s\n",StndFunc);
     */
    
    return 0;
}
