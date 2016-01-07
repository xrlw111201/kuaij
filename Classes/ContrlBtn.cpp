//
//  contrlBtn.cpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//
#include "HelloWorldScene.h"
#include "ContrlBtn.h"
#include "ContrlScene.h"
#include "Global.h"

Node* ContrlBtn::s_pCtrlScene = NULL;
int ContrlBtn::s_nCtrlSelBtnTag = 0;

void ContrlBtn::setProperties(int index){
    m_tag = index;
    m_strOrd = "";
    //memset(&m_bi, 0, sizeof(BTNINFO));
    
    Size size = Director::getInstance()->getVisibleSize();
    m_scrWidth = size.width;
    m_scrHeight = size.height;
    
    char szName[20] = {0};
    sprintf(szName, "btn%d.png", m_tag);
    initWithFile(szName);
    
    setBtnPos();
}

bool ContrlBtn::init(){

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(ContrlBtn::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ContrlBtn::setBtnPos(){
    switch(m_tag){
        case 300:{setPosition(m_scrWidth*0.1828, m_scrHeight*0.7991); break;}
        case 400:{setPosition(m_scrWidth*0.1828, m_scrHeight*0.7991); setVisible(false); break;}
        case 301:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7991); break;}
        case 401:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7991); setVisible(false); break;}
        case 302:{setPosition(m_scrWidth*0.8156, m_scrHeight*0.7991); break;}
        case 402:{setPosition(m_scrWidth*0.8156, m_scrHeight*0.7991); setVisible(false); break;}
        case 303:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6380); break;}
        case 403:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6380); setVisible(false); break;}
        case 304:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.5104); break;}
        case 404:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.5104); setVisible(false); break;}
        case 305:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3827); break;}
        case 405:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3827); setVisible(false); break;}
        case 306:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2569); break;}
        case 406:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2569); setVisible(false); break;}
        default:break;
    }
    if(m_tag == s_nCtrlSelBtnTag){
        setVisible(true);
    }
}

void ContrlBtn::setBi(){
    m_bi.tag = m_tag;
    m_bi.strOrd = m_strOrd;
}

void ContrlBtn::back2home(){
    auto trans = TransitionMoveInL::create(0.5, HelloWorld::createScene());
    Director::getInstance()->replaceScene(trans);
}

bool ContrlBtn::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    if(m_tag < 400 || m_tag > 500) return false;
    Vec2 localTouch = touch->getLocation();
    if(getBoundingBox().containsPoint(localTouch)){
        if(406 == m_tag){
            s_nCtrlSelBtnTag = 0;
            //点击返回按钮，返回主场景
            back2home();
            return false;
        }
        if(0 == s_nCtrlSelBtnTag){
            setVisible(true);
            s_nCtrlSelBtnTag = m_tag;
        }
        else if(s_nCtrlSelBtnTag == m_tag){
            setVisible(false);
            s_nCtrlSelBtnTag = 0;
        }else{
            getParent()->getChildByTag(s_nCtrlSelBtnTag)->setVisible(false);
            setVisible(true);
            s_nCtrlSelBtnTag = m_tag;
        }
    }
    return false;
}