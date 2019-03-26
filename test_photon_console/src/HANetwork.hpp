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
#include "HAPhoton/HAPhotonConstants.hpp"

#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/Listener.h"

#include <vector>
#include <string>


//NS_HANETWORK_START

using EGCOMMON = ExitGames::Common::JString;

class HALogListener : public ExitGames::Common::BaseListener
{
public:
    HALogListener() {};
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override
    {
        std::cout << string.toString().trim().UTF8Representation().cstr() << std::endl;
    }
};


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

#define LOG_I(x)         EGLOG(ExitGames::Common::DebugLevel::INFO, x);
class HAListener : public ExitGames::LoadBalancing::Listener
{
    ExitGames::Common::Logger mLogger; // accessed by EGLOG()

public:
    HAListener()
    {
        //TODO:
        static HALogListener h;
        mLogger.setListener(h);
        mLogger.setDebugOutputLevel(4);
        LOG_I(L"コンストラクタン！");
    }
    virtual ~HAListener(void){};

    // receive and print out debug out here
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) {
        LOG_I(L"debug return. "+ string.toString());
    };
    
    // implement your error-handling here
    virtual void connectionErrorReturn(int errorCode) {
        LOG_I(L"connection error code.");
        std::cout << errorCode << std::endl;
    };
    virtual void clientErrorReturn(int errorCode) {
        LOG_I(L"client error code.%s");
        std::cout << errorCode << std::endl;
    };
    virtual void warningReturn(int warningCode) {
        LOG_I(L"warning return");
        std::cout << warningCode << std::endl;
    };
    virtual void serverErrorReturn(int errorCode) {
        LOG_I(L"server error return");
        std::cout << errorCode << std::endl;
    };
    
    // events, triggered by certain operations of all players in the same room
    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {};
    virtual void leaveRoomEventAction(int playerNr, bool isInactive) {};
    
    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) {
        EGLOG(ExitGames::Common::DebugLevel::INFO, L"イベントを受信した.");
    };
    
    // callbacks for operations on server
    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) {
        EGLOG(ExitGames::Common::DebugLevel::INFO, L"connect return.");
    };
    virtual void disconnectReturn(void) {
        EGLOG(ExitGames::Common::DebugLevel::INFO, L"disconnect return.");
    };
    virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {
        EGLOG(ExitGames::Common::DebugLevel::INFO, L"create room return.");
    };
    virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) {};
    virtual void joinLobbyReturn(void) {};
    virtual void leaveLobbyReturn(void) {};
};

//TODO:
class HANetworkLogic
{
public:
//    HANetworkLogic(){};
    HANetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion);

    void connect(void);
    void disconnect(void);
    void run(void);
    void createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);
    void join();
    void send();
private:
    ExitGames::LoadBalancing::Client mLoadBalancingClient;
    HAListener mListener; // your implementation of the ExitGames::LoadBalancing::Listener interface
    ExitGames::Common::Logger mLogger; // accessed by EGLOG()
};

HANetworkLogic::HANetworkLogic(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion)
    : mLoadBalancingClient(mListener, appID, appVersion)
{
    //TODO: staticとかやべーだろ。
    auto p = std::unique_ptr<HALogListener>();
    static HALogListener h;
    mLogger.setListener(h);
    mLogger.setDebugOutputLevel(4);
}

void HANetworkLogic::connect(void)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"EGLOG.");

    // connect() is asynchronous - the actual result arrives in the Listener::connectReturn() or the Listener::connectionErrorReturn() callback
    if(!mLoadBalancingClient.connect())
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void HANetworkLogic::disconnect(void)
{
    mLoadBalancingClient.disconnect();
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"disconnect.");
}

void HANetworkLogic::run(void)
{
    mLoadBalancingClient.service();
//    EGLOG(ExitGames::Common::DebugLevel::INFO, L"net logic run.");
}

void HANetworkLogic::createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers)
{
    mLoadBalancingClient.opCreateRoom(roomName, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers));
}

void HANetworkLogic::join()
{
    //TODO: search room.
    
    
    //is random.
//    // join random rooms easily, filtering for specific room properties, if needed
//    ExitGames::Common::Hashtable expectedCustomRoomProperties;
//
//    // custom props can have any name but the key must be string
//    expectedCustomRoomProperties.put(L"room01", 1);
//
//    // joining a random room with the map we selected before
//    mLoadBalancingClient.opJoinRandomRoom(expectedCustomRoomProperties);
}

void HANetworkLogic::send()
{
    //イベントコードは200以下にすること。
    nByte eventCode = 1; // use distinct event codes to distinguish between different types of events (for example 'move', 'shoot', etc.)
    assert(eventCode<=200);
    ExitGames::Common::Hashtable evData; // organize your payload data in any way you like as long as it is supported by Photons serialization
    bool sendReliable = false; // send something reliable if it has to arrive everywhere
    mLoadBalancingClient.opRaiseEvent(sendReliable, evData, eventCode);
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
