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
#define LBL         90004
#define LST         90005
#define INNERNET_G  90007
#define INNERNET    90008
#define EBNAME      90009
#define EBPW        90010
#define SEND_B      90011
#define SEND        90012
#define EXITS_B     90013
#define EXITS       90014

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
