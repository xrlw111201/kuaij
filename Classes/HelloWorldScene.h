#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <cocos2d.h>
USING_NS_CC;

#include <cocos-ext.h>
USING_NS_CC_EXT;

#include <string>
using namespace std;

class HelloWorld : public cocos2d::Layer, public EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
    
    void menuCloseCallback(CCObject* pSender);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void editBoxEditingDidBegin(EditBox*);
    virtual void editBoxEditingDidEnd(EditBox*);
    virtual void editBoxTextChanged(EditBox*, const std::string&);
    virtual void editBoxReturn(EditBox*);
private:
    int m_nCurBtnTag;
    int m_socket;
    string m_strSnPath;
    
    bool initBtn();
    void setGlobalSn(string);
    void setBtnOrd();
};

#endif // __HELLOWORLD_SCENE_H__
