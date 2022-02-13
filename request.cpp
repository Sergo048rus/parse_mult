//
// Created by osipo on 07.02.2022.
//

#include "request.h"

request_c::request_c(string URL)
{
    this -> URL = URL;
}

std::string request_c::get_res(std::string ip)
{
    return global_func::html_req(ip + MAIN_API, "/" + ip + "/");
}




void request_c::search_film()
{
    std::vector<film_t> film;
    string html_buff;
    cout<<"URL: "<<URL<<endl;
    html_buff = get_res(URL);

    char delim[21] = "<td class=l><a href=";
    char delim1[21] = "</a></td>";
    char delim2[21] = "html\" >";
    cout<<"Размер строки: "<< html_buff.length()<<endl;

    size_t pos = 0;
    size_t pos1;
    size_t pos2;
    bool search_continue = 1;
    int count = 0;


    vector<std::string> buf;
    vector<std::string> buf1;

    while(search_continue)                                  // Ищем все ссылки на фильмы/мультфильмы/фильмики
    {
        count++;
        pos = html_buff.find(delim, pos+20);
        pos1 = html_buff.find(delim1, pos);
        pos2 = html_buff.find(delim2, pos+strlen(delim));
        search_continue = (pos == std::string::npos)? FALSE : TRUE;
//        cout<<"Вхождение "<<count<<": "<<pos<<"\t"<<search_continue<<endl;
        if(!search_continue)
        {
            break;
        }

        string sub = html_buff.substr(pos + strlen(delim),pos1);
        buf = global_func::split(sub,'"');
        string sub1 = html_buff.substr(pos2 + strlen(delim2),pos1);
        buf1 = global_func::split(sub1,'<');
        char buf_d[1024];
        strcpy(buf_d,buf1[0].c_str());

//        std::cout <<global_func::cp1251_to_utf8(buf_d)<< std::endl;

        for (std::vector<std::string>::const_iterator i = buf.begin(); i != buf.end(); ++i)
        {
            film_t film_buf;
            string t = *i;
            size_t p = t.find("/info");
            if (p != std::string::npos)
            {
                film_buf.link.push_back(t);

                film_buf.name = global_func::cp1251_to_utf8(buf_d);
                film.push_back(film_buf);
                break;
            }
        }
    }
    optimalFilm(film);
    std::cout << "ФИНИШ Поиска" << std::endl;
}
void request_c::optimalFilm(std::vector<film_t> film)
{
    for (std::vector<film_t>::const_iterator i = film.begin(); i != film.end(); ++i)
    {
        film_t film_buf = *i;
        string str = film_buf.name;

        size_t pos = str.find("/");       // Ищем дробь, значит фильм состоит из частей
        if (pos != std::string::npos)
        {
            size_t pos1 = str.find(")",pos);
            size_t pos2 = str.find("(");
            if (pos2 != std::string::npos)
            {
                size_t pos3 = str.find("(",pos2 + 1); // Проверяем есть ли еще одни скобки перед скобкой с частью
                if(pos3 > pos2 && pos3 < pos)
                {
                    pos2 = pos3;
                }
                string sub = str.substr(pos+1,pos1-1);
                int s;
                std::istringstream ( sub ) >> s;

                film_buf.name = str.substr(0,pos2);        // Добавляем имя без скобок и пробела (Дополнительное название приходится отбрасывать)

//            cout << film_buf.name << str[pos] << s << endl;
                for(int k = 1; k < s; k++)       // Базовая ссылка уже добавлена
                {
                    ++i;
                    film_t f = *i;                      //TODO Проверить на наличие промежуточного фильма
                    size_t p = f.name.find("/");
                    if(p != std::string::npos)
                    {
                        film_buf.link.push_back(f.link[0]);
                    }
                    else
                    {
                        k--;                             // Так как лишний фильм нужно совершить еще одну итерацию
                        film_optimal.push_back(f);       // Это другой фильм нужно добавить отдельно
                    }
                }
            }

        }
        film_optimal.push_back(film_buf);          // Добавляем в оптимальный вектор

    }
}



void request_c::downloadFile(string path)
{
    for (std::vector<film_t>::const_iterator i = film_optimal.begin(); i != film_optimal.end(); ++i)
    {
        obj_parse_c obj_parse(URL,*i);
        obj_parse.download_data(path);
    }
}



void request_c::print_obj()
{
    int count = 0;
    for (std::vector<film_t>::const_iterator i = film_optimal.begin(); i != film_optimal.end(); ++i)
    {
        count++;
        film_t film = *i;
        cout<<"Фильм "<< count<< ": "<<film.name<<endl;

        for (std::vector<std::string>::const_iterator k = film.link.begin(); k != film.link.end(); ++k)
        {
            cout<<"Ссылка на "<< film.name<< ": "<<*k<<endl;
        }
    }
}



