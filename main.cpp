#include <iostream>
#include "string"

#include <filesystem>

#include "request.h"



int main(int argc, char* argv[])
{

    string filmiki      = "filmiki";
    string multiki      = "multiki";
    string film         = "film";

    std::filesystem::path dirPath;
    if (argc > 1)
    {
        std::cout <<"Введеный путь: " << argv[1]<<std::endl;
        dirPath = argv[1];
    } else
    {
        std::cout << "Not arguments" << std::endl;
        dirPath =  std::filesystem::current_path();
    }

    string Path;
    stringstream ss;
    ss << dirPath;
    ss >> Path;

    // TODO кодировка ломает размерность
    size_t coder = Path.length()-2;
//    int coder = Path.length()-1;

    string Path_filmiki = Path.substr(1,coder) + "/" + filmiki;
    cout<<Path_filmiki<<endl;

    char buf_с[1024];
    strcpy(buf_с,Path_filmiki.c_str());

    if (mkdir(buf_с))                // Создаем папку
    {
        cout<< "Ошибка создания каталога: "<<filmiki<<endl;
    }

    string Path_multiki = Path.substr(1,coder) + "/" + multiki;
    cout<<Path_multiki<<endl;
    strcpy(buf_с,Path_multiki.c_str());

    if (mkdir(buf_с))                // Создаем папку
    {
        cout<< "Ошибка создания каталога: "<<multiki<<endl;
    }

    string Path_film = Path.substr(1,coder) + "/" + film;
    cout<<"Скачивание будет происходить в каталог: "<<Path_film<<endl;
    strcpy(buf_с,Path_film.c_str());

    if (mkdir(buf_с))                // Создаем папку
    {
        cout<< "Ошибка создания каталога: "<<film<<endl;
    }


//    cout<<50/60<<endl;


    // Создаем объекты для обработки html страницы
    request_c req_filmiki(filmiki);
    request_c req_multiki(multiki);
    request_c req_film(film);

    int menu = 1;
    while(menu !=0)
    {
        cout<<"Выберите что скачать:"<<endl;
        cout<<"1 - Мультики"<<endl;
        cout<<"2 - Фильмы"<<endl;
        cout<<"3 - Детские фильмы"<<endl;
        cout<<"0 - Выйти"<<endl;
        cout<<"Выберите пункт меню"<<endl;
        cin>>menu;

        if(menu == 1)
        {
            //         Parse multiki
            req_multiki.search_film();
            req_multiki.downloadFile( Path_multiki);
        }
        else if(menu == 2)
        {
            //         Parse film
            req_film.search_film();
            req_film.downloadFile( Path_film);
        }
        else if(menu == 3)
        {
        //         Parse filmiki
            req_filmiki.search_film();
            req_filmiki.downloadFile( Path_filmiki);
        }
    }


    return 0;
}
