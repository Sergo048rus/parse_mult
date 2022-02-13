//
// Created by osipo on 09.02.2022.
//
#include "global_func.h"

std::string global_func::cp1251_to_utf8(const char *str)
{
    std::string res;
    int result_u, result_c;
    result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if(!result_u ){return 0;}
    wchar_t *ures = new wchar_t[result_u];
    if(!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)){
        delete[] ures;
        return 0;
    }
    result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
    if(!result_c){
        delete [] ures;
        return 0;
    }
    char *cres = new char[result_c];
    if(!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)){
        delete[] cres;
        return 0;
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

std::string global_func::utf8_to_cp1251(const char *str)
{
    std::string res;
    WCHAR *ures = NULL;
    char *cres = NULL;

    int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    if (result_u != 0)
    {
        ures = new WCHAR[result_u];
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
        {
            int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
            if (result_c != 0)
            {
                cres = new char[result_c];
                if (WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
                {
                    res = cres;
                }
            }
        }
    }

    delete[] ures;
    delete[] cres;

    return res;
}



std::vector<std::string> global_func::split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}



std::string global_func::html_req(std::string main_ip,std::string sub_ip)
{

    boost::asio::io_context io;
    ip::tcp::resolver resolver(io);
    ip::tcp::socket socket(io);

    boost::asio::connect(socket, resolver.resolve(main_ip, "80"));

    http::request<http::string_body> req(http::verb::get, sub_ip, 11);

    req.set(http::field::host, main_ip);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);

    std::string response;
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(socket, buffer, res);
        response = boost::beast::buffers_to_string(res.body().data());
    }

    socket.shutdown(ip::tcp::socket::shutdown_both);
    return response;

}
