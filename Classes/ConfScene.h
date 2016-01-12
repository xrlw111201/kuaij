//
//  ConfScene.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef ConfScene_hpp
#define ConfScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#include "string"

class ConfScene : public cocos2d::Layer, public ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ConfScene);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void editBoxEditingDidBegin(ui::EditBox*);
    virtual void editBoxEditingDidEnd(ui::EditBox*);
    virtual void editBoxTextChanged(ui::EditBox*, const std::string&);
    virtual void editBoxReturn(ui::EditBox*);
    
private:
    std::string m_strSnPath;
    bool initBtn();
    void setGlobalSn(std::string);
    void setBtnOrd();
};


#endif /* ConfScene_hpp */
