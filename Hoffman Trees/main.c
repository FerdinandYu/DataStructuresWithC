//
//  main.c
//  Trees
//
//  Created by 余宏昊 on 2018/11/30.
//  Copyright © 2018 余宏昊. All rights reserved.
//

#include <stdio.h>
#include "HuffmanCode.h"

int main()
{
    char msg[]="At the post G20 Summit in Buenos Aires, US President Donald Trump held what the White House called a 'very successful meeting' with Chinese leader Xi Jinping and agreed that current US tariffs on $200 billion worth of Chinese imports of 10% will remain unchanged for 90 days. Tariffs were due to be raised to 25% on January 1.";
    MsgInfo msgInfo;
    GetSortedMsg(msg, &msgInfo);
    printf("\nThe frequency for each character is as follows:\n");
    PrintMsgInfo(msgInfo);
    HuffmanTree HT;Code code;
    HuffmanTree * pTree=HuffmanCoding(&HT, &msgInfo, &code);
    printf("\nThe Huffman code for each character is as follows:\n");
    for(int i=0;i<msgInfo.numChar;++i)
        printf("%c:%s\n",code._char[i],code._code[i]);
    printf("\nThe Huffman code for the message is:\n");
    char _code[20];
    char msgcode[10000];
    char blank=' ';
    char blankcode[20];
    memset(_code,0,sizeof(_code));
    memset(msgcode,0,sizeof(msgcode));
    memset(blankcode,0,sizeof(blankcode));
    printf("\n");
    for(int i=1;i<=strlen(msg);i++)
    {
        GetCode(&HT, msgInfo, msg[i-1], _code);
        printf("%s",_code);
        strcpy(&msgcode[strlen(msgcode)],_code);
    }
    printf("\n");
    GetCode(&HT, msgInfo, blank, blankcode);
    strcpy(&msgcode[strlen(msgcode)],blankcode);
    //char encoded[]="010001101001101000111110010010111100011011111011111001011";
    char decoded[10000];
    memset(decoded, 0, 10000);
    printf("Encoded:%s\n",msgcode);
    HuffmanDecoding(pTree, msgInfo, code, msgcode, decoded);
    return 0;
}
