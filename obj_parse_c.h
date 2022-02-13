//
// Created by osipo on 09.02.2022.
//

#ifndef TASK2_OBJ_PARSE_C_H
#define TASK2_OBJ_PARSE_C_H


#include "global_func.h"
#include "boost/beast.hpp"
#include <boost/asio.hpp>
#include "fstream"


using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

class obj_parse_c {

private:
    const std::string MAIN_API = ".arjlover.net";



    std::vector<std::string> URL_jpg;                   // Ссылки на картинки
    std::vector<std::string> URL_link;                  // Ссылки на скачивание
    string Title;                                       // Название фильма
    std::string category;                               // Категория Film/mult/filmik
    unsigned int duration = 0;
    string format = "";
    unsigned int size = 0;
    string format_dur = "";

    void GetFile(const std::string& serverName,
                 const std::string& getCommand,
                 std::ofstream& outFile);
    bool get_jpg(string & data);
    void check_title(string & data);
    void parse_duration(string & data);
    void parse_format(string & data);
    void parse_link(string & data);
    void parse_size(string & data);
    string byte_convert();
    string time_convert();



public:
    obj_parse_c(std::string category,film_t film);
    void download_data(std::string path);
//    ~obj_parse_c();





};


#endif //TASK2_OBJ_PARSE_C_H
