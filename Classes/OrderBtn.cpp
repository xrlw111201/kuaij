//
//  OrderBtn.cpp
//  kj
//
//  Created by liuhui on 15/12/10.
//
//

#include "OrderBtn.h"
#include "HelloWorldScene.h"
#include "Global.h"
#include <unistd.h>

Node* OrderBtn::s_pHomeScene = NULL;
int OrderBtn::s_nOrderSelBtnTag = 0;

void OrderBtn::setProperties(int index){
    m_tag = index;
    m_strOrder = "";
    memset(&m_pid, 0, sizeof(pthread_t));
    
    Size size = Director::getInstance()->getVisibleSize();
    
    m_scrWidth = size.width;
    m_scrHeight = size.height;

    char szName[20] = {0};
    sprintf(szName, "btn%d.png", index);
    initWithFile(szName);
    
    setBtnPos();
}

void OrderBtn::setBi(){
    m_bi.tag = m_tag;
    m_bi.strOrd = m_strOrder;
}

bool OrderBtn::init(){
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(OrderBtn::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void OrderBtn::setBtnPos(){
    switch(m_tag){
        case 100:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7394); break;}
        case 200:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.7394); setVisible(false); break;}
        case 101:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6144); break;}
        case 201:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.6144); setVisible(false); break;}
        case 102:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.4894); break;}
        case 202:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.4894); setVisible(false); break;}
        case 103:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3627); break;}
        case 203:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.3627); setVisible(false); break;}
        case 104:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2368); break;}
        case 204:{setPosition(m_scrWidth*0.5000, m_scrHeight*0.2368); setVisible(false); break;}
        default: break;
    }
    if(m_tag == s_nOrderSelBtnTag){
        setVisible(true);
    }
}

bool OrderBtn::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    
    if(m_tag < 200) return false;
    Vec2 localTouch = touch->getLocation();
    if(getBoundingBox().containsPoint(localTouch)){
        if(0 == OrderBtn::s_nOrderSelBtnTag){
            setVisible(true);
            OrderBtn::s_nOrderSelBtnTag = m_tag;
            _sendOrder();
        }
        else if(OrderBtn::s_nOrderSelBtnTag == m_tag){
            setVisible(false);
            OrderBtn::s_nOrderSelBtnTag = 0;
        }else{
            getParent()->getChildByTag(OrderBtn::s_nOrderSelBtnTag)->setVisible(false);
            setVisible(true);
            OrderBtn::s_nOrderSelBtnTag = m_tag;
            _sendOrder();
        }
    }
    return false;
}

void OrderBtn::_sendOrder(){
    
    Label* lbl = (Label*)s_pHomeScene->getChildByTag(9002);
    
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

void* OrderBtn::sendOrder(void* args){

    int sock = Global::getSocket();
    sockaddr_in sa = Global::getAddr();
    
    BTNINFO bi = *(BTNINFO*)args;
//    char tmp[20] = {0};
//    for(int i=0; i<18; i++){
//        sprintf(tmp, "0x%02x", bi.strOrd.at(i));
//        log("%s", tmp);
//    }
//    log("%s", bi.strOrd.c_str());
    sendto(sock, bi.strOrd.c_str(), 18, 0, (sockaddr*)&sa, sizeof(sockaddr));

    char buf[18] = {0};
    socklen_t fromlen;
    //前面设置了超时值，5秒后即使无响应也返回
    ssize_t recsize = recvfrom(sock, (void*)buf, 18, 0, (sockaddr*)&sa, &fromlen);
    
    Label* lbl = (Label*)s_pHomeScene->getChildByTag(9002);
    
    if(-1 == recsize /*|| buf[7] != bi.strOrd.at(7)*/){
        s_nOrderSelBtnTag = 0;
        if(s_pHomeScene->getChildByTag(bi.tag)->isVisible()){
            s_pHomeScene->getChildByTag(bi.tag)->setVisible(false);
        }
        lbl->setString("网络超时");
    }
    return NULL;
}

void OrderBtn::unSel(){
    s_nOrderSelBtnTag = 0;
    setVisible(false);
}

void OrderBtn::setOrd(){
    //传入2个参数，分别为模式和命令，处于第7、8字节。得到完整的命令
    switch(m_tag){
        case 200:{m_strOrder = Global::getOrd(0x01, 0x00); break;}
        case 201:{m_strOrder = Global::getOrd(0x02, 0x00); break;}
        case 202:{m_strOrder = Global::getOrd(0x03, 0x00); break;}
        case 203:{m_strOrder = Global::getOrd(0x04, 0x00); break;}
        case 204:{m_strOrder = Global::getOrd(0x05, 0x00); break;}
        default: break;
    }
    //设置结构体，用于给线程传值
    setBi();
}