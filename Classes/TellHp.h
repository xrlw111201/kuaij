//
//  TellHp.h
//  kuaij
//
//  Created by liuhui on 16/2/5.
//
//

#ifndef __kuaij__TellHp__
#define __kuaij__TellHp__

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <string>
USING_NS_CC;

class TellHp : public cocos2d::Layer, public ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TellHp);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    
    virtual void editBoxEditingDidBegin(ui::EditBox*);
    virtual void editBoxEditingDidEnd(ui::EditBox*);
    virtual void editBoxTextChanged(ui::EditBox*, const std::string&);
    virtual void editBoxReturn(ui::EditBox*);
    
private:
    sockaddr_in m_sa;
    
    void initAddr();
    void sendWifi2device();
};

#endif /* defined(__kuaij__TellHp__) */
