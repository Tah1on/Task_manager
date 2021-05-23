#include "to_do_list.h"

to_do_task::to_do_task(){
    SYSTEMTIME st;
    GetSystemTime(&st);

    priority = 9;
    day = (uint8_t)st.wDay;
    month = (uint8_t)st.wMonth;
    year = (uint16_t)st.wYear;
    hour = (uint8_t)st.wHour;
    minute = (uint8_t)st.wMinute;
    title = "Task unknown";
    description = "This is new task, please edit it";
}

to_do_task::to_do_task(string _title,
                       string _description,
                       uint8_t _priority,
                       uint8_t _day,
                       uint8_t _month,
                       uint8_t _hour,
                       uint8_t _minute,
                       uint16_t _year){


    priority = _priority;
    day = _day;
    month = _month;
    year = _year;
    hour = _hour;
    minute = _minute;
    title = _title;
    description = _description;
}

inline void to_do_task::convert_0(ofstream &file, uint8_t *output){
    file << (char)(*output + '0') << ',';
}
inline void to_do_task::convert_3(ofstream &file, uint16_t *output){
    uint16_t t = *output;
    for (uint16_t q = 1000; q >= 1; q /= 10){
        file << (char)(*output / q + '0');
        *output %= q;
    }
    file << ',';
    *output = t;
}
inline void to_do_task::convert_1245(ofstream &file, uint8_t *output){
    if (*output < 10)
        file << '0' << (char)(*output + '0') << ',';
    else
        file << (char)(*output / 10 + '0')
             << (char)(*output % 10 + '0')
             << ',';
}
inline void to_do_task::convert_67(ofstream &file, string *output){
    file << '"' << *output << '"';
}

void to_do_task::generate_raw(ofstream &data_file){
    convert_0(data_file, &priority);
    convert_1245(data_file, &day);
    convert_1245(data_file, &month);
    convert_3(data_file, &year);
    convert_1245(data_file, &hour);
    convert_1245(data_file, &minute);
    convert_67(data_file, &title);
    data_file << ',';
    convert_67(data_file, &description);
}
/*
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

void cout_info(uint8_t row_index, bool locale){
    if (locale == true)
        get_line(row_index);
    else
        get_line(row_index, "D:/Projects(QT)/TaskManager/interface.data");
}
*/
void to_do_task::display_the_task(bool flag_locale){
    cout << '\n' << title << ' ';
    cout_info(1, flag_locale);
    cout << ": " << (int)priority << "\n\t";
    cout_info(2, flag_locale);
    cout << ": " << (int)day << '.' << (int)month << '.' << year
         << "  " << (int)hour << ':' << (int)minute << "\n\t";
    cout_info(3, flag_locale);
    cout << ": " << description << '\n';
}


uint8_t days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint64_t to_do_task::get_for_sort(bool _case){
    if (!_case)
        return priority;
    else {
        uint64_t clock = 3153600 * year + 86400 * (day - 1);
        for (uint16_t y = 1; y <= year; y++)
            if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                clock += 86400;
        for (uint8_t m = 1; m < month; m++)
            clock += 86400 * days_in_month[m];
        clock += 3600 * hour + 60 * minute;
        return clock;
    }

}

bool to_do_task::check_date(){
    if (day <= days_in_month[month])
        return true;
    else if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month == 2 && day == 29)
        return true;
    return false;
}

//  0  uint8_t  _priority,
//  1  uint8_t  _day,
//  2  uint8_t  _month,
//  3  uint16_t _year
//  4  uint8_t  _hour,
//  5  uint8_t  _minute,
//  6  string   _title,
//  7  string   _description,

template <typename ElemType>
bool _assign(ElemType &whom, ElemType what){
    whom = what;
    return true;
}

bool to_do_task::edit_str_parameter(uint8_t parameter, string content){
    switch(parameter){
    case 6: return content.length() <= 70 ? _assign(title, content) : false; break;
    case 7: return content.length() <= 140 ? _assign(description, content) : false; break;
    }
    return false; // Если параметр будет другой
}

bool to_do_task::edit_int_parameter(uint8_t parameter, uint16_t content){
    switch(parameter){
    case 0: return (1 <= content && content <= 9) ? _assign(priority, (uint8_t)content) : false; break;
    case 1: return (1 <= content && content <= 31) ? _assign(day, (uint8_t)content) : false; break;
    case 2: return (1 <= content && content <= 12) ? _assign(month, (uint8_t)content) : false; break;
    case 3: return (2000 <= content && content <= 3000) ? _assign(year, content) : false; break;
    case 4: return (content < 24) ? _assign(hour, (uint8_t)content) : false; break;
    case 5: return (content < 60) ? _assign(minute, (uint8_t)content) : false; break;
    }
    return false; // Если параметр будет другой
}

int longestsub(string x, string y){
    const int n = x.length(),
              m = y.length();
    int lcs[n+1][m+1];
    int result=0;
    for(int i=0;i<n+1;i++){
        for(int j=0;j<m+1;j++){
            if(i==0||j==0){
                lcs[i][j]=0;
            }
            else if(x[i-1]==y[j-1]){
            lcs[i][j]=1+lcs[i-1][j-1];
            result =max(result,lcs[i][j]);
            }
            else lcs[i][j]=0;
        }
    }
    return result;
}

bool to_do_task::search_by_word(string word, string text){
    string word_temp = "";
    int ls;
    text += ' ';
    for (int i = 0; i < text.length(); i++){
        if (text[i] == ' '){
            if (word.length() > 2){
                ls = longestsub(word, word_temp);
                cout << '$' << ((float)ls / (float)word.length()) << '!' << ls << '?' << word_temp.length() << '\n';
                if ((float)ls / (float)(word.length()) > 0.5){

                    return true;
                }
                word_temp = "";
            }
            else if (word == word_temp)
                return true;
            word_temp = "";
        }
        else
            word_temp += text[i];
        cout << '<' << word_temp << '\n';
    }
    return false;
}

bool to_do_task::compare_data(uint8_t *_priority,
                              uint8_t *_day,
                              uint8_t *_month,
                              uint16_t *_year,
                              uint8_t *_hour,
                              uint8_t *_minute,
                              string *_title,
                              string *_description){
    bool res = true;
    if (_priority != nullptr)
        res *= (priority == *_priority);
    if (_day != nullptr)
        res *= (day == *_day);
    if (_month != nullptr)
        res *= (month == *_month);
    if (_year != nullptr)
        res *= (year == *_year);
    if (_hour != nullptr)
        res *= (hour == *_hour);
    if (_minute != nullptr)
        res *= (minute == *_minute);
    if (_title != nullptr){
        //res *= (title == *_title);
        //cout << '<' << *_title << _title->length() << title << '>';
        res *= search_by_word(*_title, title);
    }
    if (_description != nullptr)
        res *= search_by_word(*_description, description);
    return res;
}

//to_do_list---------------------------------------------------

to_do_list::to_do_list(to_do_task *_task){
    task = _task;
    next_task = nullptr;
}

to_do_list::to_do_list(){
    task = nullptr;
    next_task = nullptr;
}

to_do_list::~to_do_list(){
    delete task;
    delete next_task;
}

void to_do_list::prepend(to_do_task *_task){
    if (task == nullptr){
        task = _task;
    }
    else {
        if (next_task == nullptr)
            next_task = new to_do_list();
        to_do_list *list_temp = next_task;
        while (list_temp->task != nullptr){
            if (list_temp->next_task == nullptr)
                list_temp->next_task = new to_do_list();
            list_temp = list_temp->next_task;
        }
        list_temp->task = _task;
    }
}

// O(2*index + 3)
// 0 1(2)3
// 1 2 3 4 5 6
void to_do_list::erase(to_do_list *&list, uint8_t index){
    // Деструктор задачи не нужен, т.к. там нет динамически выделенной памяти.
    if (index == 0){
        list = list->next_task;
        return;
    }
    to_do_list *lst_right = list;
    for (uint8_t i = 0; i <= index; i++)
        lst_right = lst_right->next_task;
    to_do_list *link_to_first_elem = list;
    for (uint8_t i = 0; i < index - 1; i++)
        list = list->next_task;
    list->next_task = lst_right;
    list = link_to_first_elem;
}

void to_do_list::clear(){
    delete task;
    delete next_task;
    task = nullptr;
    next_task = nullptr;
}

template <typename T, typename Check>
void to_do_list::read_data(ifstream &file, T &input, uint8_t length){
    input = 0;
    uint16_t power = 1;
    for (uint8_t i = 1; i < length; i++)
        power *= 10;
    for (uint8_t i = 0; i < length; i++){
        input += (file.get() - '0') * (T)power;
        power /= 10;
    }
    file.get();
}

void to_do_list::read_data(ifstream &file, string &input){
    input = "";
    file.get();
    char stream_temp_char = file.get();
    while(stream_temp_char != '"') {
        input += stream_temp_char;
        stream_temp_char = file.get();
    }
    file.get();
}

void to_do_list::import_data(to_do_list *&list){
    string _title,
           _description;
    uint8_t _priority,
            _day,
            _month,
            _hour,
            _minute;
    uint16_t _year;

    ifstream data_file("D:/Projects(QT)/TaskManager/data.csv");

    while (!data_file.eof()){
        read_data(data_file, _priority, 1);
        read_data(data_file, _day, 2);
        read_data(data_file, _month, 2);
        read_data(data_file, _year, 4);
        read_data(data_file, _hour, 2);
        read_data(data_file, _minute, 2);
        read_data(data_file, _title);
        read_data(data_file, _description);
        to_do_task *task_temp = new to_do_task(_title,
                                               _description,
                                               _priority,
                                               _day,
                                               _month,
                                               _hour,
                                               _minute,
                                               _year);
        list->prepend(task_temp);
    }
    data_file.close();
}

void to_do_list::export_data(to_do_list *list){
    ofstream data_file("D:/Projects(QT)/TaskManager/data.csv", std::ofstream::trunc);
    if (list->task != nullptr)
        while(true){
            list->task->generate_raw(data_file);
            list = list->next_task;
            if (list != nullptr)
                data_file << '\n';
            else
                break;
        }
    data_file.close();
}

void to_do_list::display_the_list(to_do_list *list, bool flag_locale){
    uint8_t index = 0;
    if (list != nullptr && list->task != nullptr)
        do{
            cout << '\n';
            cout_info(23, flag_locale);
            cout << (int)index++;
            list->task->display_the_task(flag_locale);
            list = list->next_task;
        }while (list != nullptr);
}

void to_do_list::swap_tasks(to_do_task *&task_1, to_do_task *&task_2){
    to_do_task *task_temp = task_1;
    task_1 = task_2;
    task_2 = task_temp;
}

uint8_t to_do_list::list_length(to_do_list *list){
    uint8_t counter = 0;
    if (list != nullptr && list->task != nullptr)
        do{
            counter++;
            list = list->next_task;
        }while (list != nullptr);
    return counter;
}

to_do_task *&to_do_list::task_ptr(to_do_list *list, uint8_t index){
    if (list != nullptr && list->task != nullptr && index != 0)
        do{
            index--;
            list = list->next_task;
        }while (list != nullptr && index > 0);
    return list->task;
}

void to_do_list::sort_by_case(to_do_list *&list, bool _case){
    to_do_task *temp;
    int16_t item;
    for (uint16_t counter = 1; counter < to_do_list::list_length(list); counter++){
        temp = to_do_list::task_ptr(list, counter);
        item = counter - 1;
        while(item >= 0 && to_do_list::task_ptr(list, item)->get_for_sort(_case) > temp->get_for_sort(_case)){
            to_do_list::swap_tasks(to_do_list::task_ptr(list, item + 1), to_do_list::task_ptr(list, item));
            item--;
        }

    }
}

/*
void insertionSort(int *arrayPtr, int length) // сортировка вставками
{
    int temp, // временная переменная для хранения значения элемента сортируемого массива
        item; // индекс предыдущего элемента
    for (int counter = 1; counter < length; counter++)
    {
        temp = arrayPtr[counter]; // инициализируем временную переменную текущим значением элемента массива
        item = counter-1; // запоминаем индекс предыдущего элемента массива
        while(item >= 0 && arrayPtr[item] > temp) // пока индекс не равен 0 и предыдущий элемент массива больше текущего
        {
            arrayPtr[item + 1] = arrayPtr[item]; // перестановка элементов массива
            arrayPtr[item] = temp;
            item--;
        }
    }
}
*/

//Реализовать через ситч . фильтры и через эти 2 функции и шаблоны a b c



uint8_t to_do_list::count_results_of_search(to_do_list *list,
                                            uint8_t *_priority,
                                            uint8_t *_day,
                                            uint8_t *_month,
                                            uint16_t *_year,
                                            uint8_t *_hour,
                                            uint8_t *_minute,
                                            string *_title,
                                            string *_description,
                                            bool show,
                                            bool flag_locale){
    uint8_t counter = 0; //cout << "\ncount_results_of_search\n";
    if (list != nullptr && list->task != nullptr){
        if (show){
            do{
                if (list->task->compare_data(_priority, _day, _month, _year, _hour, _minute, _title, _description)){
                    cout << "\nTask #" << (int)counter << '\n';
                    list->task->display_the_task(flag_locale);
                    counter++;
                }
                list = list->next_task;
            }while (list != nullptr);
        }
        else
            do{
            if (list->task->compare_data(_priority, _day, _month, _year, _hour, _minute, _title, _description))
                counter++;
            list = list->next_task;
            }while (list != nullptr);
    }
    return counter;
}

uint8_t to_do_list::return_real_index_of_search(to_do_list *list,
                                                uint8_t index,
                                                uint8_t *_priority,
                                                uint8_t *_day,
                                                uint8_t *_month,
                                                uint16_t *_year,
                                                uint8_t *_hour,
                                                uint8_t *_minute,
                                                string *_title,
                                                string *_description){
    int counter = 0,
        real_index = -1;
    if (list != nullptr && list->task != nullptr)
        do{
        if (list->task->compare_data(_priority, _day, _month, _year, _hour, _minute, _title, _description))
            counter++;
        real_index++;
        if ((int)counter - 1 == index)
            return real_index;
        list = list->next_task;
    }while (list != nullptr);
    return 0;
}

void to_do_list::print_list_for_the_(to_do_list *list, uint8_t _case, bool flag_locale){
    SYSTEMTIME st;
    GetSystemTime(&st);

    uint8_t *_priority = nullptr,
            *_day = nullptr,
            *_month = nullptr;
    uint16_t *_year = nullptr;
    uint8_t *_hour = nullptr,
            *_minute = nullptr;
    string *_title = nullptr,
           *_description = nullptr;
    /*
    to_do_task task_filters("",
                            "",
                            uint8_t _priority,
                            uint8_t _day,
                            uint8_t _month,
                            uint8_t _hour,
                            uint8_t _minute,
                            uint16_t _year);
    Поправить
    */

    _month = new uint8_t((uint8_t)st.wMonth);
    _year = new uint16_t((uint16_t)st.wYear);
    if (_case == 8 || _case == 9)
        _day = new uint8_t((uint8_t)st.wDay);
    to_do_list::count_results_of_search(list, _priority, _day, _month, _year, _hour, _minute, _title, _description, true, flag_locale);
    if (_case == 9){
        int week = ++*_day + 7;
        for (; *_day <= week; ++*_day)
            to_do_list::count_results_of_search(list, _priority, _day, _month, _year, _hour, _minute, _title, _description, true, flag_locale);
    }
    cout << '\n';

    delete _day,
    delete _month,
    delete _year;
}

//..................

//  0  uint8_t  _priority,
//  1  uint8_t  _day,
//  2  uint8_t  _month,
//  3  uint16_t _year
//  4  uint8_t  _hour,
//  5  uint8_t  _minute,
//  6  string   _title,
//  7  string   _description,


