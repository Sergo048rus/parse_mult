//
// Created by osipo on 09.02.2022.
//

#ifndef TASK2_GLOBAL_FUNC_H
#define TASK2_GLOBAL_FUNC_H
#include "string"
#include <iostream>
#include <locale>
#include "stringapiset.h"
#include <vector>
#include <sstream>

#include "boost/beast.hpp"
#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
namespace http = boost::beast::http;
namespace ip = boost::asio::ip;

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

struct film_t
{
    std::vector<std::string> link;
    std::string name;
};

namespace global_func
{
    std::string cp1251_to_utf8(const char *str);
    std::string utf8_to_cp1251(const char *str);
    std::vector<std::string> split(const std::string &s, char delim);

    std::string html_req(std::string main_ip,std::string sub_ip);

}




#endif //TASK2_GLOBAL_FUNC_H
