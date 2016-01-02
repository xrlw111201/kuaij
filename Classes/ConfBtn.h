//
//  ConfBtn.hpp
//  kj
//
//  Created by liuhui on 15/12/12.
//
//

#ifndef ConfBtn_hpp
#define ConfBtn_hpp

#include <cocos2d.h>

#include <string>
using namespace std;
USING_NS_CC;

typedef struct btn_info{
    int tag;
    string strOrd;
}BTNINFO;

class ConfBtn: public Sprite{
public:
    virtual bool init();
    virtual bool onTouchBegan(Touch*, Event*);
    CREATE_FUNC(ConfBtn);
    
private:
    int m_tag;
    string m_strOrder;
    float m_scrWidth;
    float m_scrHeight;
    pthread_t m_pid;
    BTNINFO m_bi;
    
    void setBtnPos();
    void back2home();
    static void* sendOrder(void*);
    void _sendOrder();
    void setBi();
    
public:
    static Node* s_pConfScene;
    static int s_nConfSelBtnTag;
    
    void setProperties(int);
    void setOrd();
};


#endif /* ConfBtn_hpp */
