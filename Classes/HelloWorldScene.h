#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <ui/CocosGUI.h>
#include <string>
#include <map>

class HelloWorld : public cocos2d::Layer, public ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
//    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    
    virtual void editBoxEditingDidBegin(ui::EditBox*);
    virtual void editBoxEditingDidEnd(ui::EditBox*);
    virtual void editBoxTextChanged(ui::EditBox*, const std::string&);
    virtual void editBoxReturn(ui::EditBox*);
    void setBtnUp(int);

private:
//    static bool s_bWait;
    std::map<Sprite*, Label*> m_mpLstCtrl;
    //int m_nCurBtnTag;
    //int m_socket;
//    std::string m_strSnPath;
    pthread_t m_pidQuery;
    pthread_t m_pidRsp;
    bool m_bInetEnabel;
    
    
    static void* sendQuery(void*);
    static void* recvRsp(void*);
    
    bool initBtn();
//    void setGlobalSn(std::string);
    void setBtnOrd();
    void innerNet();
    std::string getMobileIpString();
    void querySn(std::string);
    void initSnLstFromVct();
    void getSnLstFromFile();
    void cleanLstCtrl();
    
    void searchCompleted(float);
};

#endif // __HELLOWORLD_SCENE_H__
