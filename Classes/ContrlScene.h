//
//  ContrlScene.hpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//

#ifndef ContrlScene_hpp
#define ContrlScene_hpp

#include <cocos2d.h>
USING_NS_CC;

#include <cocos-ext.h>
USING_NS_CC_EXT;

#include <string>
using namespace std;

class ContrlScene : public cocos2d::Layer, public EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ContrlScene);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void editBoxEditingDidBegin(EditBox*);
    virtual void editBoxEditingDidEnd(EditBox*);
    virtual void editBoxTextChanged(EditBox*, const std::string&);
    virtual void editBoxReturn(EditBox*);
    
private:
    string m_strSnPath;
    
    bool initBtn();
    void setGlobalSn(string);
};

#endif /* ContrlScene_hpp */
