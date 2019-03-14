#include "HANetwork.hpp"

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <sys/time.h>


namespace d3
{
	class Clock
	{
    private:
        std::chrono::system_clock::time_point _start;

    public:
        inline void start()
        {
            _start = std::chrono::system_clock::now();
        }
        inline void end()
        {
            auto end = std::chrono::system_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count();            
        }
	};
}


static long getCurrentMillSecond() {
    long lLastTime;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; // milliseconds
    return lLastTime;
}

void create_room()
{

}


int main(int argc, char** argv) 
{
	using std::string;
	using std::cin;
    using std::cout;

    cout << "Input Photon-App-Key";
	string str;
	cin >> str;
    cout << str;

    long _animationInterval = (1.0f/60.0f*1000.0f);

    long lastTime = 0L;
    long curTime = 0L;

	std::string key_input;
	while(1)
	{
        lastTime = getCurrentMillSecond();

        //　テストロジック
        {
            cout << "." ;
            cin >> key_input;
            if( key_input == "exit" )               { break; }
            else if( key_input == "create_room" )   { create_room(); }
            else if( key_input == "connect" )       {  }
        }

        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _animationInterval)
        {
            usleep((_animationInterval - curTime + lastTime)*1000);
        }
	}

	return 0;
}
