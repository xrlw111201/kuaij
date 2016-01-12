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
#define CFGBG       91
#define CTRLBG      92
#define EDITBOXPNG  90000
#define EDITBOX     90001
#define CTRL        90002
#define CFG         90003
#define EXIT        90004
#define LBL         90005


#include "string"
#include <arpa/inet.h>
#include <netdb.h>

class Global{
private:
    static std::string g_strSn;
    static int g_socket;
    static bool g_bNetErr;
    static sockaddr_in g_sa;
public:
    static int getSocket();
    static sockaddr_in getAddr();
    static bool getNetErr();
    static void closeSock();
    static bool initSock();
    static void setSn(std::string&);
    static std::string getOrd(char, char);
};

#endif /* Global_hpp */
