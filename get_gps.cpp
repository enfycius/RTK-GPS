#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <future>
#include <memory>
#include <thread>
#include "./test/conv.hpp"

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

std::shared_ptr<System> get_system(Mavsdk& mavsdk)
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<System>>{};
    auto fut = prom.get_future();

    mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        }
    });

    if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
        std::cerr << "No autopilot found.\n";
        return {};
    }

    return fut.get();
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk;
    Converter::Conv conv;
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = get_system(mavsdk);
    
    if (!system) {
        return 1;
    }

    auto telemetry = Telemetry{system};
    auto action = Action{system};

    const auto set_rate_result = telemetry.set_rate_position(1.0);

    conv.initialize(0.0, 0.0, 0.0);
    
    double north, east, down;

    telemetry.subscribe_position([&conv, &north, &east, &down](Telemetry::Position position) {
        std::cout << position.latitude_deg << "deg" << '\n';
        std::cout << position.longitude_deg << "deg" << '\n';
        std::cout << position.relative_altitude_m << "deg" << '\n';

        conv.geodetic2Ned(position.latitude_deg, position.longitude_deg, position.relative_altitude_m, &north, &east, &down);

        std::cout<<"north: "<<north<<'m'<<' '<<"east: "<<east<<'m'<<' '<<"down: "<<down<<'m'<<'\n';
    });

    // Check until vehicle is ready to arm
    while (telemetry.health_all_ok() != true) {
        std::cout << "Vehicle is getting ready to arm\n";
        sleep_for(seconds(1));
    }

    return 0;
}
