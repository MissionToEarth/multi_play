/* リアルタイム通信を管理する。
注意：中国本土への接続は色々な面倒があるらしい。（参考：https://doc.photonengine.com/ja-jp/realtime/current/connection-and-authentication/regions）

とりあえずPhotonを使う想定でのまとめ。

名前サーバーに接続確認。

利用可能なリージョンを取得する。（事前にリージョンが明らかな場合は不要）

マスターサーバーに接続。

ゲームサーバーに接続

*/

#pragma once


#ifndef HA
#define HA

#include "ha_boot.hpp"
#include <vector>
#include <string>

//#define USE_EG_PHOTON
//
//#ifdef USE_EG_PHOTON
#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/Listener.h"
//#endif


//NS_HANETWORK_START


#ifdef USE_EG_PHOTON
class NetworkSettings
{
    //リージョン設定：  https://doc.photonengine.com/ja-jp/realtime/current/connection-and-authentication/regions
    std::string regions = "ja";
};
#else
class NetworkSettings
{

};
#endif

// 貰ってくる available regions.
std::vector<std::string> requestAvailableRegions()
{
    std::vector<std::string> regions;

    //TODO: C# is loadBalancingClient.OpGetRegions()    
    
    return regions;
};


class HAListener : public ExitGames::LoadBalancing::Listener
{
public:
    virtual ~HAListener(void){};

    // receive and print out debug out here
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) {};
    
    // implement your error-handling here
    virtual void connectionErrorReturn(int errorCode) {};
    virtual void clientErrorReturn(int errorCode) {};
    virtual void warningReturn(int warningCode) {};
    virtual void serverErrorReturn(int errorCode) {};
    
    // events, triggered by certain operations of all players in the same room
    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {};
    virtual void leaveRoomEventAction(int playerNr, bool isInactive) {};
    
    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) {};
    
    // callbacks for operations on server
    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) {};
    virtual void disconnectReturn(void) {};
    virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinLobbyReturn(void) {};
    virtual void leaveLobbyReturn(void) {};
};

HAListener mls;
ExitGames::LoadBalancing::Client mLoadBalancingClient(mls, "", "");

//TODO:
class HANetworkLogic
{
public:
//    HANetworkLogic(){};
    HANetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion);

    void connect(void);
    void disconnect(void);
    void run(void);
private:
//    ExitGames::LoadBalancing::Client mLoadBalancingClient;
//    HAListener mListener; // your implementation of the ExitGames::LoadBalancing::Listener interface
//    ExitGames::Common::Logger mLogger; // accessed by EGLOG()
};

HANetworkLogic::HANetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion)
//    : mLoadBalancingClient(mListener, appID, appVersion)
{
}

void HANetworkLogic::connect(void)
{
    // connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
//    if(!mLoadBalancingClient.connect())
//        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void HANetworkLogic::disconnect(void)
{

}

void HANetworkLogic::run(void)
{

}

// class HANetworkLogic
// {

// public:
//     HANetworkLogic();
//     virtual ~HANetworkLogic();

//     // initialized
//     void init();

//     //
//     bool check_server_connection();


//     // ルーム作成

//     // ルーム
//     // ルームフィルタリング設定
//     // search room.

//     // connect room.

//     // send data.

//     // response event.

// private:
//     void connect_name_server();
// };

//NS_HANETWORK_END

#endif
