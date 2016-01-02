//
//  Global.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef Global_hpp
#define Global_hpp

#include "string"
using namespace std;

#include <arpa/inet.h>
#include <netdb.h>

class Global{
private:
    static string g_strSn;
    static int g_socket;
    static bool g_bNetErr;
    static sockaddr_in g_sa;
public:
    static int getSocket();
    static sockaddr_in getAddr();
    static bool getNetErr();
    static void closeSock();
    static bool initSock();
    static void setSn(string&);
    static string getOrd(char, char);
};

#endif /* Global_hpp */
