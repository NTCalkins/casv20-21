#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <functional>
#include <future>
#include <iostream>
#include <memory>

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace mavsdk;
using namespace std::chrono; // for seconds(), milliseconds()
using namespace std::this_thread; // for sleep_for()

class Vehicle
{
private:
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;
    std::string qgc_plan;
    auto prom;
    auto system;
    auto future_result;
    auto action;
    auto mission;
    auto telemetry;
public:
    
}

#endif