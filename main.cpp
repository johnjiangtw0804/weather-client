#include <iostream>
#include <fstream>
#include <string>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <sstream>

#include <cxxopts.hpp>

/**
 *  一個很簡單的 WeatherClient CLI 去用來查詢當下天氣，
 *  使用 Open-Meteo API 根據經緯度去查詢去查詢當日氣象資訊
 */
static const std::string OPEN_METEO_BASE_URL = "https://api.open-meteo.com/v1/forecast?";
static std::string g_lat = "52.52";
static std::string g_lon = "13.41";

int main(int argc, char** argv) {
    cxxopts::Options options("WeatherClient",
        "A weather report tool that fetches the current weather based on coordinates or IP geolocation.");
    options.add_options()
        ("i,ip", "Use IP-based location detection (1 = true, 0 = false)", cxxopts::value<int>()->default_value("1"))
        ("lon,longitude", "Longitude (e.g., -122.4194)", cxxopts::value<std::string>())
        ("lat,latitude", "Latitude (e.g., 37.7749)", cxxopts::value<std::string>())
        ("h,help", "Print help");

    auto arguments = options.parse(argc, argv);

    // if the arguments is not empty and we see some unknown options, we should exit right away
    if (arguments.count("help") || arguments.unmatched().size() > 0) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    bool use_ip = arguments["ip"].as<int>() == 1 ? true : false;
    if (!use_ip) {
        if (arguments["lon"].count() != 1 || arguments["lat"].count() != 1) {
            std::cerr << "Error: Must provide both  --latitude and --longitude when IP detection is off.\n";
            exit(1);
        }
        g_lat = arguments["latitude"].as<std::string>();
        g_lon = arguments["longitude"].as<std::string>();
        std::cout << "Using coordinates: (" << g_lat << ", " << g_lon << ")\n";
    } else {
        std::cout << "Using IP-based geolocation.\n";
    }

    try {
        curlpp::Easy myRequest;
        std::ostringstream response;

        std::string API_URL = OPEN_METEO_BASE_URL + "latitude=" + g_lat + "&longitude=" + g_lon +
         "&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m";
        myRequest.setOpt(new curlpp::options::Url(API_URL));
        myRequest.setOpt(new curlpp::options::WriteStream( &response));

        myRequest.perform();

        std:: cout << response.str() << std::endl;


        /**
         * json parser
         */

    } catch(curlpp::RuntimeError & e) {
		std::cerr << e.what() << std::endl;
        exit(1);
	}

	catch(curlpp::LogicError & e) {
		std::cerr << e.what() << std::endl;
        exit(1);
	}

    return 0;
}
