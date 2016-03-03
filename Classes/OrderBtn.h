//
//  OrderBtn.hpp
//  kj
//
//  Created by liuhui on 15/12/10.
//
//

#ifndef OrderBtn_hpp
#define OrderBtn_hpp
#include <cocos2d.h>

#include <string>
using namespace std;
USING_NS_CC;

typedef struct btn_info{
    int tag;
    string strOrd;
}BTNINFO;

class OrderBtn: public Sprite{
public:
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
//    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    CREATE_FUNC(OrderBtn);
    
private:
//    static int s_nOrderSelBtnTag;
    
    int m_tag;
    string m_strOrder;
    
    float m_scrWidth;
    float m_scrHeight;
    pthread_t m_pid;
    BTNINFO m_bi;
    
    void setBtnPos();
    static void* sendOrder(void*);
    void _sendOrder();
    void setBi();
    
public:
    static Node* s_pHomeScene;
    void setProperties(int);
    void setOrd();
};

#endif /* OrderBtn_hpp */
