//
//  Global.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef Global_hpp
#define Global_hpp

#define HOMEBG      90
//#define CFGBG       91
//#define CTRLBG      92
//#define EDITBOXPNG  90000
#define EDITBOX     90001
#define CFG         90002
#define EXIT        90003
#define BTN_LBL     90005
#define LBL         90006
#define LST         90007
#define INNERNET_G  90008
#define INNERNET    90009
#define EBNAME      90010
#define EBPW        90011
#define SEND_B      90012
#define SEND        90013
#define EXITS_B     90014
#define EXITS       90015

#define FIRSTPNG    140000
#define FIRSTLBL    150000

#include "string"
#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
//#include <sys/types.h>
//#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

class Global{
private:
    static std::string g_strSn;
    static std::string g_strIp;
    static int g_socket;
    static bool g_bNetErr;
    static sockaddr_in g_sa;
    static pthread_t s_pid;
    
    static void* parseDomain(void*);
    
public:
    static bool g_bInnerNet;
    static std::vector<std::string> g_vSnIp;
    static std::string g_fileName;
    
    static std::string getIp();
    static int getSocket();
    static sockaddr_in getAddr();
    static bool getNetErr();
    static void closeSock();
    static void initSock();
    static void setSn(std::string);
    static std::string getOrd(char, char);
    static bool isRefrain(std::string);
};

#endif /* Global_hpp */
