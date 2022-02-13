//
// Created by osipo on 07.02.2022.
//

#ifndef TASK2_REQUEST_H
#define TASK2_REQUEST_H


#include <iostream>
#include "string"

#include "fstream"
#include "global_func.h"

#include <vector>


#include "obj_parse_c.h"







class request_c
{
private:
    std::vector<film_t> film_optimal;
//    std::vector<std::string> obj;       // Вектор с URL всех найденных фильмов/мультфильмов/фильмиков
    string URL;
    string get_res(string ip);
    const std::string MAIN_API = ".arjlover.net";

    void optimalFilm(std::vector<film_t> film);


public:
    request_c(std::string URL);
    void print_obj();
    void search_film();
    void downloadFile(string path);

};


#endif //TASK2_REQUEST_H
