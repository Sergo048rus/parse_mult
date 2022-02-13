//
// Created by osipo on 09.02.2022.
//

#include "obj_parse_c.h"


void obj_parse_c::GetFile(const std::string& serverName,
             const std::string& getCommand,
             std::ofstream& outFile)
{
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    ip::tcp::resolver resolver(io_service);
    ip::tcp::resolver::query query(serverName, "http");
    ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    ip::tcp::resolver::iterator end;

    // Try each endpoint until we successfully establish a connection.
    ip::tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }

    boost::asio::streambuf request;
    std::ostream request_stream(&request);

    request_stream << "GET " << getCommand << " HTTP/1.0\r\n";
    request_stream << "Host: " << serverName << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);


    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
    {
    }

    // Write whatever content we already have to output.
    if (response.size() > 0)
    {
        outFile << &response;
    }
    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
    {
        outFile << &response;
    }
}



void obj_parse_c::check_title(string & data)      // Проверяем название фильма и корректируем (Запрещенные символы меняем на пробел)
{
    string replace_symbol = "_";
    // Запрещенные символы \ / : * ? " < > |
    string denied_symbols = "\\/:*?\"<>|";

    for(int i = 0; i<denied_symbols.length();i++)
    {
        size_t pos;
        do
        {
            pos = data.find(denied_symbols[i]);
            if (pos != std::string::npos)
            {
                if(pos > 0 & pos < data.length()-1)
                {
                    data = data.substr(0,pos) + replace_symbol + data.substr(pos + 1);
                }
                else if(pos == 0)
                {
                    data = replace_symbol + data.substr(pos + 1);
                }
                else
                {
                    data = data.substr(0, pos) + replace_symbol;
                }
            }
        } while (pos != std::string::npos);

        do
        {
            pos = data.find(".",data.length()-1);
            if (pos != std::string::npos)
            {
//                cout<<data<<endl;
                if(pos == data.length()-1)
                {
                    data = data.substr(0, pos);
                }
            }
        } while (pos != std::string::npos);

        if(data.find(" ",data.length()-1) != std::string::npos)
        {
            data = data.substr(0, data.length()-1);
        }
    }


}



// TODO вынести в функцию или переделать
bool obj_parse_c::get_jpg(string & data) {
    char delim[35] = "href=\"/ap/";
    char delim1[35] = ".image1.jpg";
    char delim2[35] = ".image2.jpg";
    char delim3[35] = ".image3.jpg";
    char delim4[35] = ".image4.jpg";
    char delim5[35] = ".image5.jpg";
    char delim6[35] = ".image6.jpg";

//    char delim1[35] = ".thumb1.jpg";
//    char delim2[35] = ".thumb2.jpg";
//    char delim3[35] = ".thumb3.jpg";
//    char delim4[35] = ".thumb4.jpg";
//    char delim5[35] = ".thumb5.jpg";
//    char delim6[35] = ".thumb6.jpg";


    //Находим первую картинку
    size_t pos = data.find(delim);
    size_t pos1 = data.find(delim1);
    if (pos1 != std::string::npos)
    {
        string sub = data.substr(pos, pos1);

        vector<std::string> vec = global_func::split(sub, '"');
        //    cout<<sub<<endl;
        URL_jpg.push_back(vec[1]);

        // Находим вторую картинку
        pos = data.find(delim, pos1);
        pos1 = data.find(delim2);

        sub = data.substr(pos, pos1);

        vec = global_func::split(sub, '"');

        URL_jpg.push_back(vec[1]);

        //    cout<<sub<<endl;

        // Находим третью картинку
        pos = data.find(delim, pos1);
        pos1 = data.find(delim3);

        sub = data.substr(pos, pos1);

        vec = global_func::split(sub, '"');

        URL_jpg.push_back(vec[1]);
        //    cout<<sub<<endl;
        // Находим 4 картинку
        pos = data.find(delim, pos1);
        pos1 = data.find(delim4);

        sub = data.substr(pos, pos1);

        vec = global_func::split(sub, '"');

        URL_jpg.push_back(vec[1]);

        // Находим 5 картинку
        pos = data.find(delim, pos1);
        pos1 = data.find(delim5);

        sub = data.substr(pos, pos1);

        vec = global_func::split(sub, '"');

        URL_jpg.push_back(vec[1]);

        // Находим 6 картинку
        pos = data.find(delim, pos1);
        pos1 = data.find(delim6);

        sub = data.substr(pos, pos1);

        vec = global_func::split(sub, '"');

        URL_jpg.push_back(vec[1]);

        //    for (std::vector<std::string>::const_iterator i = URL_jpg.begin(); i != URL_jpg.end(); ++i)
        //    {
        //        std::cout << *i << std::endl;
        //    }
        return true;
    }
    return false;
}

void obj_parse_c::parse_duration(string & data)
{
    char delim[100] = "Продолжительность:</strong></td><td>";
    char delim1[10] = "</tr>";

    char buf_с[100];

    strcpy(buf_с,global_func::utf8_to_cp1251(delim).c_str());
    size_t pos = data.find(buf_с);
    size_t pos1 = data.find(delim1,pos);

    string sub = data.substr(pos+strlen(buf_с),pos1);
//    cout<<sub<<endl;
    vector<std::string> vec = global_func::split(sub,'<');
    if(vec[0].length() == 0)
    {
        return;
    }
    else
    {
        format_dur = vec[0];
    }

//    cout<<format_dur<<endl;
    vector<std::string> dur = global_func::split(vec[0],':');

    int s = 0;
    std::istringstream ( dur[0] ) >> s;
    duration += s*60*60;
//    cout<<"STR"<<dur[0]<<"INT"<<s<<endl;
    std::istringstream ( dur[1] ) >> s;
    duration += s*60;
//    cout<<"STR"<<dur[0]<<"INT"<<s<<endl;
    std::istringstream ( dur[2] ) >> s;
    duration += s;
//    cout<<"STR"<<dur[0]<<"INT"<<s<<"DUR"<<duration<<endl;
//    cout<<"Продолжительность:\t"<<duration<<endl;

}
void obj_parse_c::parse_format(string & data)
{
    char delim[100] = "Видеоформат:</strong></td><td>";
    char buf_с[100];
    char buf_d[100];
    if(format_dur.length() == 0)
    {
        return;
    }
    strcpy(buf_с,global_func::utf8_to_cp1251(delim).c_str());
    size_t pos = data.find(buf_с);
    strcpy(buf_d,format_dur.c_str());
    size_t pos1 = data.find(buf_d,pos);
    string sub = data.substr(pos+strlen(buf_с),pos1);
//    cout<<sub<<endl;
    vector<std::string> vec = global_func::split(sub,' ');
    format = vec[1];
//    cout<<"Видеоформат:\t"<<format<<endl;
}

void obj_parse_c::parse_size(string & data)
{
    char delim[100] = "Размер:</strong></td><td>";
    char delim1[10] = "</td>";
    char buf_с[100];
    strcpy(buf_с,global_func::utf8_to_cp1251(delim).c_str());
    size_t pos = data.find(buf_с);
    size_t pos1 = data.find(delim1,pos);
    string sub = data.substr(pos+strlen(buf_с),pos1);
    vector<std::string> vec = global_func::split(sub,'<');

    int s;
    std::istringstream ( vec[0] ) >> s;
    size += s;
//    cout<<"Размер:\t"<<size<<endl;
}

string obj_parse_c::byte_convert()
{
    int Gb = size / (1024*1024*1024);
    int Mb = (size - Gb*(1024*1024*1024))/(1024*1024);
    int Kb = (size - Gb*(1024*1024*1024) - Mb*(1024*1024))/ 1024;
    return to_string(Gb) + "Gb " + to_string(Mb) + "Mb " + to_string(Kb) + "Kb";
}

string obj_parse_c::time_convert()
{

    int H = duration / (60*60);
    int M = (duration - H*(60*60))/(60);
    int S = duration - H*(60*60) - M*(60);
    return to_string(H) + ": " + to_string(M) + ": " + to_string(S);
}



void obj_parse_c::parse_link(string & data)
{
    char delim[100] = "Скачать HTTP link:</strong></td><td><a href=\"";
    char delim1[10] = "</tr>";
    char buf_с[100];
    strcpy(buf_с,global_func::utf8_to_cp1251(delim).c_str());
    size_t pos = data.find(buf_с);
    size_t pos1 = data.find(delim1,pos);
    string sub = data.substr(pos+strlen(buf_с),pos1);
    vector<std::string> vec = global_func::split(sub,'"');

    URL_link.push_back(category+MAIN_API+vec[0]);
//    cout<<"Ссылка:\t"<<category+MAIN_API+vec[0]<<endl;
}

obj_parse_c::obj_parse_c(std::string category,film_t film)
{
    string URL_argument = film.link[0];
    this -> category = category;

//    cout<<category+MAIN_API+URL_argument<<endl;



    string html_buff = global_func::html_req(category+MAIN_API, URL_argument);
    cout<<"Старт"<<endl;
    Title = film.name;
    check_title(Title);
    cout<<"Название фильма: "<<Title<<endl;
    cout<<endl;

    if(get_jpg(html_buff))
    {
        for (std::vector<std::string>::const_iterator i = film.link.begin(); i != film.link.end(); ++i)
        {
            string buf = global_func::html_req(category+MAIN_API, *i);
            parse_duration(html_buff);
            parse_link(buf);

            parse_size(buf);
        }

        parse_format(html_buff);
    }
    else
    {
        cout<<"Страница неверного формата"<<endl;       // Страница без данных, скорее всего была перемещена
    }




}

void obj_parse_c::download_data(std::string path)
{
//    cout<< "Старт записи в папку "<<Title<<"!"<<endl;
    string str = path+"/"+Title;
//    cout<<str<<endl;
    char buf_с[str.length()+10];
    strcpy(buf_с,str.c_str());
    str += "/";
    if (mkdir(buf_с))                // Создаем папку с фильмом
    {
        cout<< "Ошибка создания каталога: "<<Title<<endl;
        return;
    }


    // Скачивание картинок
    int count = 0;
    for (std::vector<std::string>::const_iterator i = URL_jpg.begin(); i != URL_jpg.end(); ++i)
    {
        count++;

        string image_name =  str+"image"+ to_string(count) +".jpg";
//        std::cout << "Картинка: "<<image_name<<std::endl;
        std::ofstream outFile(image_name, std::ofstream::out | std::ofstream::binary);
        if (!outFile) // если файл не был открыт
        {
            std::cout << "Файл не может быть открыт или создан!"<<std::endl;
        }

        GetFile(category+MAIN_API, *i, outFile);
        outFile.close();

    }

    // Создание файла info

    std::ofstream fout(str+"info.txt", std::ios_base::out | std::ios_base::trunc);


    fout<<"Продолжительность:\t"<<time_convert()<<endl;
    fout<<"Видеоформат:\t"<<format<<endl;
    fout<<"Размер:\t \t"<<byte_convert()<<endl;         // Переводим итоговый размер фильма
    fout.close();


    fout.open(str+"links.txt", std::ios_base::out | std::ios_base::trunc);


    for (std::vector<std::string>::const_iterator i = URL_link.begin(); i != URL_link.end(); ++i)
    {
        fout<<*i<<endl;
    }

    fout.close();

    cout<<"Файлы записаны"<<endl;
}



















