#ifndef OUTPUT_FUNCS_H
#define OUTPUT_FUNCS_H

#include <iostream>
#include <fstream>

using namespace std;

void get_line(uint8_t row_index, string path = "D:/Projects(QT)/TaskManager/locale.data"){
    ifstream _locale(path);
    char stream_char_temp = NULL;
    string content_str,
           content_str_temp;
    bool enter_flag = false;
    uint8_t index = 0;
    while (!_locale.eof() && index < row_index)
         if (_locale.get() == '\n')
             index++;
    while ((stream_char_temp = _locale.get()) && stream_char_temp != '\n' && !_locale.eof()){
        if(stream_char_temp == 'n' && enter_flag == true){
            content_str += '\n';
            enter_flag = false;
        }
        else if (stream_char_temp != '\\')
            content_str += stream_char_temp;
        else
            enter_flag = true;
    }
    cout << content_str;
    _locale.close();
}

void cout_info(uint8_t row_index, bool locale = true){
    if (locale == true)
        get_line(row_index);
    else
        get_line(row_index, "D:/Projects(QT)/TaskManager/interface.data");
}

#endif // OUTPUT_FUNCS_H
