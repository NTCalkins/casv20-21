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

Vehicle::makeConnection()
{
  std::cout << "Connection URL: " << connection_url << std::endl;
  {
    Vehicle::prom = std::make_shared<std::promise<void>>();
    Vehicle::future_result = Vehicle::prom->get_future();

    std::cout << "Waiting to discover system..." << std::endl;
    mavsdk.subscribe_on_new_system([&Vehicle::mavsdk, Vehicle::prom]() {
        Vehicle::system = Vehicle::mavsdk.systems().at(0);

        if (Vehicle::system->is_connected()) {
            std::cout << "Discovered system" << std::endl;
            Vehicle::prom->set_value();
        } else {
            std::cout << "System timed out" << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(0);
        }
    });

    Vehicle::connection_result = Vehicle::mavsdk.add_any_connection(connection_url);
    handle_connection_err_exit(Vehicle::connection_result, "Connection failed: ");

    Vehicle::future_result.get();
  }
}

Vehicle::waitOnSystem()
{
  Vehicle::system = Vehicle::mavsdk.systems.at(0);
  Vehicle::action = Action{Vehicle::system};
  Vehicle::mission = Mission{Vehicle::system};
  Vehicle::telemetry = Telemetry{Vehicle::system};
  while (!Vehicle::telemetry.health_all_ok()) {
    std::cout << "Waiting for system to be ready" << std::endl;
    sleep_for(seconds(1));
  }
  std::cout << "System ready" << std::endl;
  return;
}

Vehicle::SetUpAltitudeMonitor()
{
  // Set up callback to monitor altitude while the vehicle is in flight
    Vehicle::telemetry.subscribe_position([](Telemetry::Position position) {
        std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "Altitude: " << position.relative_altitude_m << " m"
                  << NORMAL_CONSOLE_TEXT // set to default color again
                  << std::endl;
    });
    return;
}

Vehicle::TakeOff()
{
    // Take off
    std::cout << "Taking off..." << std::endl;
    const Action::Result takeoff_result = Vehicle::action.takeoff();
    if (takeoff_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Takeoff failed:" << takeoff_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }
}

Vehicle::Land()
{
    std::cout << "Landing..." << std::endl;
    const Action::Result land_result = Vehicle::action.land();
    if (land_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << land_result << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }
}

bool Vehicle::isInAir()
{
  return Vehicle::telemetry.in_air();
}

Vehicle::Arm()
{
      // Arm vehicle
    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = Vehicle::action.arm();

    if (arm_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Arming failed:" << arm_result << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }
}