//
//  contrlBtn.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef contrlBtn_hpp
#define contrlBtn_hpp

#include <cocos2d.h>
USING_NS_CC;

#include <string>
using namespace std;

typedef struct btn_info{
    int tag;
    string strOrd;
}BTNINFO;

class ContrlBtn: public Sprite{
public:
    virtual bool init();
    virtual bool onTouchBegan(Touch*, Event*);
    CREATE_FUNC(ContrlBtn);
    
private:
    int m_tag;
    string m_strOrd;
    float m_scrWidth;
    float m_scrHeight;
    static int s_nCtrlSelBtnTag;
    BTNINFO m_bi;
    
    void setBtnPos();
    void back2home();
    void setBi();
    
public:
    static Node* s_pCtrlScene;
    
    void setProperties(int);
};

#endif /* contrlBtn_hpp */
