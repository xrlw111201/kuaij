//
//  ConfScene.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef ConfScene_hpp
#define ConfScene_hpp

#include <cocos2d.h>
USING_NS_CC;

#include <cocos-ext.h>
USING_NS_CC_EXT;

#include <string>
using namespace std;

class ConfScene : public cocos2d::Layer, public EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ConfScene);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void editBoxEditingDidBegin(EditBox*);
    virtual void editBoxEditingDidEnd(EditBox*);
    virtual void editBoxTextChanged(EditBox*, const std::string&);
    virtual void editBoxReturn(EditBox*);
    
private:
    string m_strSnPath;
    bool initBtn();
    void setGlobalSn(string);
    void setBtnOrd();
};


#endif /* ConfScene_hpp */
