/*
Создайте приложение «Список дел».
Приложение должно позволять:
■ У дела есть: +
• название; +
• приоритет; +
• описание; +
• дата, время исполнения. +
■ Добавление дел. +
■ Удаление дел. +
■ Редактирование дел. +
■ Поиск дел по: +
• названию; +
• приоритету; +
• описанию; +
• дате и времени исполнения. +
■ Отображение списка дел: +
• на день; +
• на неделю; +
• на месяц. +
■ При отображении должна быть возможность сортировки:+
• по приоритету; +
• по дате и времени исполнения. +
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include "to_do_list.h"
#include "output_funcs.h"

using namespace std;

bool flag_locale = false;
bool flag_autosave = false;
to_do_list *task_list = new to_do_list();

uint8_t *_priority = nullptr,
        *_day = nullptr,
        *_month = nullptr;
uint16_t *_year = nullptr;
uint8_t *_hour = nullptr,
        *_minute = nullptr;
string *_title = nullptr,
       *_description = nullptr;

int get_variant();
bool menu_rusure(); // Подвкрждение выбора
bool menu_settings(); // Настройки приложения
void change_parameter(to_do_task *&, uint8_t); // Быстрое изменение значений
bool menu_edit(to_do_task *& task); // Изменение параметров задания
bool menu_do_smth(uint8_t); // Изменение / Удаление задачи по реальному индексу
bool menu_select(int);
bool menu_search_tasks_in_list(); // Поиск заданий
bool menu_sort(); // Сортиовка
bool menu_first(int); // Запуск

int main(int argc, char* argv[])
{
    SetConsoleCP(65001);// установка кодовой страницы UTF-8 в поток ввода
    SetConsoleOutputCP(65001); // установка кодовой страницы UTF-8 в поток вывода


    to_do_list::import_data(task_list);

    while (menu_first(-1)) {}

    delete _priority,
    delete _day,
    delete _month,
    delete _year,
    delete _hour;
    delete _minute;
    delete _title;
    delete _description;

    //to_do_list::export_data(task_list);

    return 0;
}

int get_variant(){
    cout << '>';
    int var;
    cin >> var;
    return var;
}

bool menu_rusure(){
    int variant;

    system("cls");
    cout_info(24, flag_locale); // Are you sure?
    cout << "0. ";
    cout_info(25, flag_locale); // Нет
    cout << "1. ";
    cout_info(26, flag_locale); // Да
    variant = get_variant(); // получаем номер выбранного пункта меню

    if (variant == 1)
        return true;
    return false;
}

bool menu_settings(){
    int variant;

    system("cls");
    cout_info(34, flag_locale); // Settings
    cout << "1. ";
    cout_info(38, flag_locale); // Localization:
    cout_info(35 + (int)flag_locale, flag_locale);
    cout << "\n2. ";
    cout_info(39, flag_locale); // Auto-save:
    cout_info(35 + (int)flag_autosave, flag_locale);
    cout << "\n0. ";
    cout_info(4, flag_locale); // Back
    variant = get_variant(); // получаем номер выбранного пункта меню

    switch (variant) {
    case 1:
        flag_locale = !flag_locale;
        break;
    case 2:
        flag_autosave = !flag_autosave;
        break;
    case 0:
        return false;
    }
    menu_settings();
    return true;
}

void change_parameter(to_do_task *&task, uint8_t _case){
    int what = 1;
    switch (_case) {
    case 1: what = 28; break;
    case 2: what = 29; break;
    case 3: what = 15; break;
    case 4: what = 13; break;
    case 5: what = 14; break;
    case 6: what = 27; break;
    case 7: what = 3;  break;
    }
    system("cls");
    if (_case <= 5){
        int value;
        cout_info(what, flag_locale);
        cout << ' ';
        value = get_variant();
        while (!task->edit_int_parameter(_case, value)){
            system("cls");
            cout_info(45, flag_locale);
            cout_info(what, flag_locale);
            cout <<' ';
            value = get_variant();
        }
    }
    else {
        string content;
        cout_info(what, flag_locale);
        cout << " >";
        std::cin.ignore(32767, '\n'); // удаляем символ новой строки из входного потока данных
        getline(cin, content);
        //cin >> content;
        while (!task->edit_str_parameter(_case, content)){
            system("cls");
            cout_info(45, flag_locale);
            cout_info(what, flag_locale);
            cout << " >";
            //std::cin.ignore(32767, '\n');
            getline(cin, content);
        }
    }
}


bool menu_edit(to_do_task *&task){
    int variant;

    do {
        system("cls");

        task->display_the_task(flag_locale);

        cout << "\n1. ";
        cout_info(33, flag_locale); // Apply changes
        cout << "\n0. ";
        cout_info(4, flag_locale); // Back
        cout << '\n';

        cout_info(32, flag_locale); // What do you want to change?

        cout << "\n2. ";
        cout_info(1, flag_locale); // Priority
        cout << "\n3. ";
        cout_info(43, flag_locale); // Date
        cout << "\n4. ";
        cout_info(2, flag_locale); // Time
        cout << "\n5. ";
        cout_info(27, flag_locale); // title
        cout << "\n6. ";
        cout_info(3, flag_locale); // description
        cout << '\n';

        variant = get_variant();

        switch (variant) {
        case 2: change_parameter(task, 0); break;
        case 4: change_parameter(task, 4);
                change_parameter(task, 5); break;
        case 5: change_parameter(task, 6); break;
        case 6: change_parameter(task, 7); break;
        case 3:
            change_parameter(task, 1);
            change_parameter(task, 2);
            change_parameter(task, 3);
            while (!task->check_date()){
                cout_info(46, flag_locale); // There is no such date
                system("pasuse");
                change_parameter(task, 1);
                change_parameter(task, 2);
                change_parameter(task, 3);
            }
            break;
        }
    } while (variant != 0 && variant != 1);

    if (variant == 1)
        to_do_list::export_data(task_list);
    if (variant == 0)
        return false;
    return true;

}

bool menu_do_smth(uint8_t index){
    int variant;

    system("cls");
    cout << "1. ";
    cout_info(17, flag_locale); // Edit
    cout << "2. ";
    cout_info(18, flag_locale); // Delete
    cout << "0. ";
    cout_info(4, flag_locale); // Back

    variant = get_variant();

    if (variant == 0)
        return false;
    if (variant == 1){
        if (!menu_edit(to_do_list::task_ptr(task_list, index)))
            menu_do_smth(index);
    }
    if (variant == 2){
        if (menu_rusure())
            to_do_list::erase(task_list, index);
        else
            menu_do_smth(index);
    }

    return true;
}

bool menu_select(int c){
    int index = 0;
    if (c == 0){
        cout_info(49, flag_locale); // You don't have any available tasks
        system("pause");
        return false;
    }
    if (c > 1){
        cout_info(30, flag_locale);
        cin >> index;
        while (index >= c && index >= 0) {
            cout_info(31, flag_locale);
            cout_info(30, flag_locale);
            cin >> index;
        }
    }
    if (!menu_do_smth(to_do_list::return_real_index_of_search(task_list, index, _priority, _day, _month, _year, _hour, _minute, _title, _description)) == false)
        return false;
    return true;
}

bool menu_search_tasks_in_list(){
    int variant,
        value;
    string value_str;
    uint8_t c = 0;

    do{
        system("cls");

        cout << "1. ";
        cout_info(40, flag_locale); // Confirm filters
        cout << '\n';

        cout_info(8, flag_locale); // Search tasks in list

        cout_info(9, flag_locale); // by priority
        cout << "\t2. ";
        cout_info(1, flag_locale); // Priority
        cout << ": ";
        if (_priority != nullptr)
            cout << (int)*_priority;
        cout << '\n';

        cout_info(11, flag_locale); // by time:
        cout << "\t3. ";
        cout_info(15, flag_locale); // Year
        cout << ": ";
        if (_year != nullptr)
            cout << (int)*_year;
        cout << '\n';
        cout << "\t4. ";
        cout_info(28, flag_locale); // Day
        cout << ": ";
        if (_day != nullptr)
            cout << (int)*_day;
        cout << '\n';
        cout << "\t5. ";
        cout_info(29, flag_locale); // Month
        cout << ": ";
        if (_month != nullptr)
            cout << (int)*_month;
        cout << '\n';
        cout << "\t6. ";
        cout_info(14, flag_locale); // Hour
        cout << ": ";
        if (_hour != nullptr)
            cout << (int)*_hour;
        cout << '\n';
        cout << "\t7. ";
        cout_info(13, flag_locale); // Minute
        cout << ": ";
        if (_minute != nullptr)
            cout << (int)*_minute;
        cout << '\n';

        cout_info(12, flag_locale); // by the word in the:
        cout << "\t8. ";
        cout_info(27, flag_locale); // title
        cout << ": ";
        if (_title != nullptr)
            cout << *_title;
        cout << '\n';
        cout << "\t9. ";
        cout_info(3, flag_locale); // description
        cout << ": ";
        if (_description != nullptr)
            cout << *_description;
        cout << '\n';

        cout << "\n0. ";
        cout_info(4, flag_locale); // Back

        variant = get_variant();

        if (variant == 0)
            return false;

        if (variant >= 2 && variant <= 9){
            cout_info(42, flag_locale); // Pres "Enter" to cancel the parameter
            cout_info(10, flag_locale); // Enter the value
            if (variant <= 7)
                value = get_variant();
            else{
                cout << '>';
                cin >> value_str;
            }
            //cout << value;
            //system("pause");
        }

        switch (variant) {
        case 2:
            delete  _priority;
            if (value == -1)
                _priority = nullptr;
            else
                _priority = new uint8_t(value);
        break;
        case 3:
            delete  _year;
            if (value == -1)
                _year = nullptr;
            else
                _year = new uint16_t(value);
        break;
        case 4:
            delete  _day;
            if (value == -1)
                _day = nullptr;
            else
                _day = new uint8_t(value);
        break;
        case 5:
            delete  _month;
            if (value == '-')
                _month = nullptr;
            else
                _month = new uint8_t(value);
        break;
        case 6:
            delete  _hour;
            if (value == -1)
                _hour = nullptr;
            else
                _hour = new uint8_t(value);
        break;
        case 7:
            delete  _minute;
            if (value == -1)
                _minute = nullptr;
            else
                _minute = new uint8_t(value);
        break;
        case 8:
            delete  _title;
            if (value == -1)
                _title = nullptr;
            else
                _title = new string(value_str);
        break;
        case 9:
            delete  _description;
            if (value == -1)
                _description = nullptr;
            else
                _description = new string(value_str);
        break;


        }
    } while (variant != 0 && variant != 1);

    if (variant == 1){
        system("cls");
        cout << "1. ";
        cout_info(16, flag_locale); // Select a task
        cout << "2. ";
        cout_info(48, flag_locale); // Home
        cout << "0. ";
        cout_info(4, flag_locale); // Back
        cout << '\n';

        c = to_do_list::count_results_of_search(task_list, _priority, _day, _month, _year, _hour, _minute, _title, _description, true, flag_locale);
        cout_info(41, flag_locale); // Results:
        cout << (int)c << '\n';

        variant = get_variant();

        if (variant == 2)
            return false;
        else if (variant == 0)
            menu_search_tasks_in_list();
        else
            if (!menu_select(c))
                menu_search_tasks_in_list();
    }

    return true;
}

bool menu_sort(){
    int variant;

    system("cls");
    cout << "1. ";
    cout_info(9, flag_locale); // by priority
    cout << "2. ";
    cout_info(44, flag_locale); // by date
    cout << "0. ";
    cout_info(4, flag_locale); // Back
    variant = get_variant();

    if (variant == 0)
        return false;
    to_do_list::sort_by_case(task_list, (bool)(variant - 1));
    to_do_list::export_data(task_list);
    return true;
}

bool menu_first(int variant){

    system("cls");

    cout << "1. ";
    cout_info(6, flag_locale); // Print all tasks in list
    cout << "2. ";
    cout_info(7, flag_locale); // Do you want to sort the list
    cout << "3. ";
    cout_info(8, flag_locale); // Search tasks in list
    cout << "4. ";
    cout_info(16, flag_locale); // Select a task
    cout << "5. ";
    cout_info(19, flag_locale); // Add task to list
    cout << "6. ";
    cout_info(20, flag_locale); // Clear the entire list
    cout << "7. ";
    cout_info(34, flag_locale); // Settings
    cout << "0. ";
    cout_info(0, flag_locale); // Exit
    cout_info(47, flag_locale); // day, week, month,

    switch (variant) {
    case 0: // Exit
        if (menu_rusure())
            exit(0);
        break;
    case 1: // Print all tasks in list
        to_do_list::display_the_list(task_list, flag_locale);
        system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
        break;
    case 2:
        menu_sort();
        break;
    case 3: // Search tasks in list
        _priority = nullptr,
        _day = nullptr,
        _month = nullptr;
        _year = nullptr;
        _hour = nullptr,
        _minute = nullptr;
        _title = nullptr,
        _description = nullptr;
        menu_search_tasks_in_list();
        break;

    case 4: // Select a task
        to_do_list::display_the_list(task_list, flag_locale);
        _priority = nullptr,
        _day = nullptr,
        _month = nullptr;
        _year = nullptr;
        _hour = nullptr,
        _minute = nullptr;
        _title = nullptr,
        _description = nullptr;
        menu_select(to_do_list::list_length(task_list));
            //menu_first(4);
        break;

    case 5: { // Add task to list
        to_do_task *task = new to_do_task();
        if (menu_edit(task)){
            task_list->prepend(task);
            to_do_list::export_data(task_list);
        }
        else
            delete task;
        system("pause");
        break;
    }

    case 6: // Clear the entire list
        if (menu_rusure()){
            task_list->clear();
            to_do_list::export_data(task_list);
        }
        break;

    case 7: // App settings
        menu_settings();
        break;

    case 8: // Show just a half
    case 9: // day, week, month
    case 10:
        to_do_list::print_list_for_the_(task_list, variant, flag_locale);
        system("pause");
        break;
    default:
        variant = get_variant();
        menu_first(variant);
        break;
    }

    return true;
}
