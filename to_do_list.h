#ifndef TO_DO_LIST_H
#define TO_DO_LIST_H

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "output_funcs.h"

using namespace std;

struct to_do_task{
private:
    string title,
           description;
    uint8_t priority,
            day,
            month,
            hour,
            minute;
    uint16_t year;

    void convert_0(ofstream &, uint8_t *);
    void convert_3(ofstream &, uint16_t *);
    void convert_1245(ofstream &, uint8_t *);
    void convert_67(ofstream &, string *);
    bool search_by_word(string, string);

public:
    to_do_task();
    to_do_task(string, string, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
    bool edit_str_parameter(uint8_t, string);
    bool edit_int_parameter(uint8_t, uint16_t);
    bool check_date();
    bool check_year();
    void generate_raw(ofstream &);
    void display_the_task(bool);
    uint64_t get_for_sort(bool);
    bool compare_data(uint8_t *_priority = nullptr,
                      uint8_t *_day = nullptr,
                      uint8_t *_month = nullptr,
                      uint16_t *_year = nullptr,
                      uint8_t *_hour = nullptr,
                      uint8_t *_minute = nullptr,
                      string *_title = nullptr,
                      string *_description = nullptr);
};

struct to_do_list{
private:
    to_do_task *task;
    to_do_list *next_task;

    template <typename T, typename Check = typename std::enable_if<
                                                    std::is_same<T, uint8_t>::value ||
                                                    std::is_same<T, uint16_t>::value>::type>
    static void read_data(ifstream &, T &, uint8_t);
    static void read_data(ifstream &, string &);

public:
    to_do_list(to_do_task *);
    to_do_list();
    ~to_do_list();
    void prepend(to_do_task *);
    void clear();
    static void erase(to_do_list *&, uint8_t);
    static void import_data(to_do_list *&);
    static void export_data(to_do_list *);
    static void display_the_list(to_do_list *, bool);
    static void swap_tasks(to_do_task *&, to_do_task *&);
    static uint8_t list_length(to_do_list *);
    static to_do_task *&task_ptr(to_do_list *, uint8_t);
    static void sort_by_case(to_do_list *&, bool = false);
    static void print_list_for_the_(to_do_list *, uint8_t, bool);

    static uint8_t count_results_of_search(to_do_list *, uint8_t *, uint8_t *, uint8_t *, uint16_t *, uint8_t *, uint8_t *, string *, string *, bool = false, bool = true);
    static uint8_t return_real_index_of_search(to_do_list *, uint8_t, uint8_t *, uint8_t *, uint8_t *, uint16_t *, uint8_t *, uint8_t *, string *, string *);

};

//void cout_info(uint8_t, bool = true);
int longestsub(string, string);

//  0  uint8_t  _priority,
//  1  uint8_t  _day,
//  2  uint8_t  _month,
//  3  uint16_t _year
//  4  uint8_t  _hour,
//  5  uint8_t  _minute,
//  6  string   _title,
//  7  string   _description,

#endif // TO_DO_LIST_H
