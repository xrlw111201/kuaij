//
//  Global.cpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

/*  c#版校验码生成逻辑，生成32位整数crc，其中第0-7位为命令中的第18字节，第8-15为命令中的第17字节
 UInt32 crc = 0;
 Byte ucByte;
 int i, j;
 bool  iTopBitCRC, iTopBitByte;
 for (j = 0; j < 16; j++)
 {
    ucByte = WifiTxBuff[j];
    for (i = 0; i < 8; i++)
    {
        iTopBitCRC = (crc & 0x8000) != 0;
        iTopBitByte = (ucByte & 0x80) != 0;
        if(iTopBitCRC != iTopBitByte)
        {
            crc = (crc << 1) ^ 0x1201;
        }
        else
        {
            crc = (crc << 1);
        }
        ucByte <<= 1;
    }
 }
 crc = crc & 0xffff;
*/
#include <unistd.h>
#include "Global.h"
#include <stdlib.h>

std::string Global::g_strSn = "";
int Global::g_socket = 0;
bool Global::g_bNetErr = false;
sockaddr_in Global::g_sa = {0, 0, 0, {0}};

int Global::getSocket(){
    return g_socket;
}

sockaddr_in Global::getAddr(){
    return g_sa;
}

bool Global::getNetErr(){
    return g_bNetErr;
}

bool Global::initSock(){

    g_socket = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定本地端口
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = 6000;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    if(-1 == bind(g_socket, (sockaddr*)&localAddr, sizeof(sockaddr))){
        closeSock();
        g_bNetErr = true;
        return false;
    }
    
    //设置recvfrom超时值
    struct timeval tv_out;
    tv_out.tv_sec = 5;//等待5秒
    tv_out.tv_usec = 0;
    setsockopt(g_socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    memset(&g_sa, 0, sizeof(sockaddr_in));
    g_sa.sin_family = AF_INET;
    g_sa.sin_port = htons(6000);
    
    hostent* pHst = gethostbyname("gzkjhbkj.com");
    if(NULL == pHst){
        closeSock();
        g_bNetErr = true;
        return false;
    }
    char** alist = NULL;
    char str[32] = {0};
    for(alist = pHst->h_addr_list; *alist != NULL; alist++){
        inet_ntop(pHst->h_addrtype, *alist, str, 32);
    }
    
    //    int res = inet_pton(AF_INET, "101.200.89.64", &m_sa.sin_addr);
    inet_pton(AF_INET, str, &g_sa.sin_addr);
    
    return true;
}

void Global::closeSock(){
    if(g_socket >= 0){
        close(g_socket);
    }
}

void Global::setSn(std::string& dsn){
    //0 2 4 6 8 10
    //将12个设备字符转换成6个16进制ascii码保存到1-6个字
    std::string strArray[6];
    for(int i=0; i<6; i++){
        strArray[i].assign(dsn, i*2, 2);
    }
    char* pos;
    
    char sn[6] = {0};

    for(int j=0; j<6; j++){
        sn[j] = strtol(strArray[j].c_str(), &pos, 16);
    }

    g_strSn.assign(sn, 6);
}

std::string Global::getOrd(char mode, char ord){
    
    //接收2个参数，分别为模式和命令，处于第7、8字节
    char strOrd[18] = {0};
    
    //第0字节必须为0x86
    strOrd[0] = 0x86;
    
    //第1-6字节为设备序列号
    for(int i=0; i<6; i++){
        strOrd[i+1] = g_strSn.at(i);
    }

    //赋值模式和命令字节， 第9-14暂时未用，第15字节为0x01，代表手机端发送的命令
    strOrd[7] = mode;   strOrd[8] = ord;   strOrd[9] = 0x00;   strOrd[10] = 0x00;  strOrd[11] = 0x00;
    strOrd[12] = 0x00;  strOrd[13] = 0x00;  strOrd[14] = 0x00;  strOrd[15] = 0x01;


    //生成校验码crc，见上面c#版算法说明
    int crc = 0;
    char ucByte;
    bool  iTopBitCRC, iTopBitByte;
    for (int j=0; j<16; j++){
        ucByte = strOrd[j];
        
        for (int k=0; k<8; k++){
            iTopBitCRC = (crc & 0x8000) != 0;
            iTopBitByte = (ucByte & 0x80) != 0;
            if(iTopBitCRC != iTopBitByte){
                crc = (crc << 1) ^ 0x1201;
            }else{
                crc = (crc << 1);
            }
            ucByte <<= 1;
        }
    }
    crc = crc & 0xffff;
    
    //分别取高8位、低8位赋给16、17字节
    strOrd[16] = crc >> 8;
    strOrd[17] = crc & 0x00ff;

    std::string str;
    str.assign(strOrd, 18);

    return str;
}