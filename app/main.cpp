#include <iostream>
//#define __cplusplus 2017
#include <mutex>
#include <thread>
#include "../lib/lib.h"
#include <unistd.h>
#include <chrono>


#define DEBUG

std::mutex mtx;


void func(const std::string& data, int log_l, const Log &ctx)
{
    // Вычисляется текущее время
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    // Захватывается мьютекс
    std::lock_guard<std::mutex> lock(mtx);
    
    log_level level;
    switch (log_l){
        case 0:
            level = log_level::low;
            break;
        case 5:
            level = log_level::middle;
            break;
        case 10:
            level = log_level::high;
            break;
        default:
            std::cout << "Введено некорректное значение уровня логирование. Запись не произведена" << std::endl;
            return;
    }

    // Производится запись события в журнал
    ctx.logging(data, level, t);
    
}

int main(int argc, char* argv[]) {

    //
    // Считывание имя файла журнала и значение уровня логирования по умолчанию
    //
    std::string journal_str;
    std::cout << "Введите имя журнала: ";
    std::getline(std::cin, journal_str);
    

    std::string level; 
    std::cout << "Введите значение уровня логирования по умолчанию: ";
    std::getline(std::cin, level);
    //std::cout << std::endl;
    log_level log_level_by_default;

    if (level == "0")
        log_level_by_default = log_level::low;

    else if (level == "5")
        log_level_by_default = log_level::middle;

    else if(level == "10")
        log_level_by_default = log_level::high;
    
    else
    {
        std::cout << "Введено некорректное значение уровня логирование по умолчанию.\
                      Инициализация библиотеки не произведена. Попробуйте заново" << std::endl;
        return 1;
    }

    //
    // Инициализация библиотеки журналирования
    //
    Log log_m;
    if (!log_m.Init(journal_str, log_level_by_default)) {
        std::cout << "Не удалось корректно проинициализировать библиотеку журналирования" << std::endl;
        return 1;
    }

    while(true)
    {
        std::string log_str;
        std::cout << "Введите строку для логирования: ";
        std::getline(std::cin, log_str);
    

        std::string level; 
        std::cout << "Введите уровень логирования: ";
        std::getline(std::cin, level);


        int log_l;

        if (level == "0") 
            log_l = 0;
           
        else if (level == "5")
            log_l = 5;
            
        else if (level == "10")
            log_l = 10;

        else
        {
            std::cout << "Введено некорректное значение! Запись не произведена" << std::endl;
            continue;
        }

        //
        // Запускается поток, который производит запись в журнал
        //
        std::thread t(func, log_str, log_l, log_m);
        t.join();

    }

    return 0;
}