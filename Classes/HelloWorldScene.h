#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"
#include "string"

class HelloWorld : public cocos2d::Layer, public ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    
    virtual void editBoxEditingDidBegin(ui::EditBox*);
    virtual void editBoxEditingDidEnd(ui::EditBox*);
    virtual void editBoxTextChanged(ui::EditBox*, const std::string&);
    virtual void editBoxReturn(ui::EditBox*);
private:
    //int m_nCurBtnTag;
    int m_socket;
    std::string m_strSnPath;
    
    bool initBtn();
    void setGlobalSn(std::string);
    void setBtnOrd();
};

#endif // __HELLOWORLD_SCENE_H__
