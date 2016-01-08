//
//  ConfBtn.cpp
//  kj
//
//  Created by liuhui on 15/12/12.
//
//
#include "ConfBtn.h"
#include "Global.h"
#include "ConfScene.h"
#include "HelloWorldScene.h"
#include "ContrlScene.h"
#include <unistd.h>

Node* ConfBtn::s_pConfScene = NULL;
int ConfBtn::s_nConfSelBtnTag = 0;

void ConfBtn::setProperties(int index){
    m_tag = index;
    m_strOrder = "";
    //memset(&m_bi, 0, sizeof(BTNINFO));
    memset(&m_pid, 0, sizeof(pthread_t));
    
    Size size = Director::getInstance()->getVisibleSize();
    m_scrWidth = size.width;
    m_scrHeight = size.height;
    
    char szName[20] = {0};
    sprintf(szName, "btn%d.png", m_tag);
    initWithFile(szName);

    setBtnPos();
}

bool ConfBtn::init(){

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(ConfBtn::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void ConfBtn::setBi(){
    m_bi.tag = m_tag;
    m_bi.strOrd = m_strOrder;
}

void ConfBtn::setBtnPos(){
    switch(m_tag){
        case 500:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7367); break;}
        case 600:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7367); setVisible(false); break;}
        case 501:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6143); break;}
        case 601:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6143); setVisible(false); break;}
        case 502:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.4902); break;}
        case 602:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.4902); setVisible(false); break;}
        case 503:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3643); break;}
        case 603:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3643); setVisible(false); break;}
        case 504:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2419); break;}
        case 604:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2419); setVisible(false); break;}
        default:break;
    }
    if(m_tag == s_nConfSelBtnTag){
        setVisible(true);
    }
}

void ConfBtn::back2home(){
    auto trans = TransitionMoveInL::create(0.5, HelloWorld::createScene());
    Director::getInstance()->replaceScene(trans);
}

bool ConfBtn::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    if(m_tag < 600) return false;
    Vec2 localTouch = touch->getLocation();
    if(getBoundingBox().containsPoint(localTouch)){
        if(604 == m_tag){
            s_nConfSelBtnTag = 0;
            //点击返回按钮，返回主场景
            back2home();
            return false;
        }
        if(0 == ConfBtn::s_nConfSelBtnTag){
            setVisible(true);
            ConfBtn::s_nConfSelBtnTag = m_tag;
            //_sendOrder();
        }
        else if(ConfBtn::s_nConfSelBtnTag == m_tag){
            setVisible(false);
            ConfBtn::s_nConfSelBtnTag = 0;
        }else{
            getParent()->getChildByTag(ConfBtn::s_nConfSelBtnTag)->setVisible(false);
            setVisible(true);
            ConfBtn::s_nConfSelBtnTag = m_tag;
            //_sendOrder();
        }
    }
    return false;
}

void* ConfBtn::sendOrder(void* args){

    int sock = Global::getSocket();
    sockaddr_in sa = Global::getAddr();
    
    BTNINFO bi = *(BTNINFO*)args;

    sendto(sock, bi.strOrd.c_str(), 18, 0, (sockaddr*)&sa, sizeof(sockaddr));
    
    char buf[18] = {0};
    socklen_t fromlen;
    
    //前面设置了超时值，5秒后即使无响应也返回
    ssize_t recsize = recvfrom(sock, (void*)buf, 18, 0, (sockaddr*)&sa, &fromlen);
    
    Label* lbl = (Label*)s_pConfScene->getChildByTag(9002);
    
    if(-1 == recsize /*|| buf[7] != bi.strOrd.at(7)*/){
        s_nConfSelBtnTag = 0;
        if(s_pConfScene->getChildByTag(bi.tag)->isVisible()){
            s_pConfScene->getChildByTag(bi.tag)->setVisible(false);
        }
        lbl->setString("网络超时");
    }
    return NULL;
}

void ConfBtn::unSel(){
    setVisible(false);
    s_nConfSelBtnTag = 0;
}

void ConfBtn::_sendOrder(){
    
    Label* lbl = (Label*)s_pConfScene->getChildByTag(9002);
    
    if(Global::getNetErr()){
        lbl->setString("连接失败，请检查网络");
        unSel();
    }
    else if("" == m_strOrder){
        lbl->setString("请先指定设备序列号");
        unSel();
    }else{
        lbl->setString("");
        pthread_create(&m_pid, NULL, sendOrder, &m_bi);
    }
}

void ConfBtn::setOrd(){
    //传入2个参数，分别为模式和命令，处于第7、8字节。得到完整的命令
    switch(m_tag){
        case 600:{m_strOrder = Global::getOrd(0x07, 0x01); break;}
        case 601:{m_strOrder = Global::getOrd(0x07, 0x02); break;}
        case 602:{m_strOrder = Global::getOrd(0x07, 0x03); break;}
        case 603:{m_strOrder = Global::getOrd(0x07, 0x04); break;}
        default: break;
    }
    //设置结构体，用于给线程传值
    setBi();
}