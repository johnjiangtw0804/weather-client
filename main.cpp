#include <iostream>
#include <fstream>
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <sstream>
#include <cxxopts.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

    cxxopts::ParseResult arguments = options.parse(argc, argv);

    if (arguments.count("help") || arguments.unmatched().size() > 0) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    curlpp::Cleanup cleanup;
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
        // get IP
        curlpp::Easy IPRequest;
        std::ostringstream ip_response;
        IPRequest.setOpt(new curlpp::options::Url("https://api.ipify.org?format=json"));
        IPRequest.setOpt(new curlpp::options::WriteStream(&ip_response));
        IPRequest.perform();
        json data = json::parse(ip_response.str());
        std::string ip = data["ip"];

        std::cout << "Current IP address is: " << ip << std::endl;

        // IP to lat and long
        curlpp::Easy LongLatRequest;
        std::ostringstream loc_response;;
        LongLatRequest.setOpt(curlpp::options::Url("http://ip-api.com/json/" + ip));
        LongLatRequest.setOpt(curlpp::options::WriteStream(&loc_response));
        LongLatRequest.perform();

        data = json::parse(loc_response.str());
        g_lon = std::to_string((data["lon"].get<double>()));
        g_lat = std::to_string((data["lat"].get<double>()));
        std::cout << "Using coordinates: (" << g_lat << ", " << g_lon << ")\n";
    }

    try {
        curlpp::Easy myRequest;
        std::ostringstream weather_response;

        myRequest.setOpt(new curlpp::options::Url(OPEN_METEO_BASE_URL + "latitude=" + g_lat + "&longitude=" + g_lon +
                                                "&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m"));
        myRequest.setOpt(new curlpp::options::WriteStream(&weather_response));
        myRequest.perform();
        std::cout << weather_response.str() << std::endl;
        json data = json::parse(weather_response.str());
        // std::cout << "current weather in your area is " + data[]
    } catch(curlpp::RuntimeError & e) {
		std::cerr << e.what() << std::endl;
        exit(1);
	} catch(curlpp::LogicError & e) {
		std::cerr << e.what() << std::endl;
        exit(1);
	}
    return 0;
}
