//#define __cplusplus  201703L
#include <iostream>
#include "lib.h"
#include <format>
#include <fstream>
//#include <bits/ios_base.h>


Log::Log(){};

//
// Изменение уровлня логирования по умолчанию. Если значение не задано, то будет установлено low
//
bool Log::changeLogLevel (log_level log_level_by_dflt){

    if (log_level_by_dflt != log_level::low & log_level_by_dflt != log_level::middle & log_level_by_dflt != log_level::high){

        std::cout << "Указанно неверное значение уровня логирования" << std::endl;
        std::cout << "Значение уровня логирования по умолчанию будет установлено в значение low" << std::endl;
        m_log_level_by_default = log_level::low;

        return true;
    }

    m_log_level_by_default = log_level_by_dflt;
    return true;
}

//
// Функция инициализации библиотеки
//
bool Log::Init(const std::string& journal, log_level log_level_by_dflt){

    if (!journal.c_str() || !journal.c_str()[0]){
        std::cout << "Передано некорректное имя журнала" << std::endl;
        return false;
    }

    // Проверяем путь - название файла было передано или целый существующий путь до файла
    const std::filesystem::path file(journal);
    
    
    if (!std::filesystem::exists(file)){

        std::cout << "Указан не абсолютный путь до файла журнала. Журнал " << journal.c_str()<< " будет создан в текущем каталоге" << std::endl;

        // Путь до текущего каталога
        auto curr_path = std::filesystem::current_path();
        std::cout << "Путь каталога в котором будет создан журнал " << curr_path.native() << std::endl;

        // Формируем абсолютный путь до текущего каталога + название файла. И пытаемся его открыть 
        std::ofstream f (curr_path.native() + "//" + journal, std::ios_base::app);
        if(f.is_open())
            f.close();

        else{
            std::cout << "Не удалось создать файл журнала. Укажите корректный" << std::endl;
            return false;
        }
        m_journalPath = curr_path.native() + "//" + journal;

    }

    else{
        std::ofstream f (file.native(), std::ios_base::app);
        if(f.is_open())
            f.close();

        else{
            std::cout << "Не удалось открыть файл журнала. Попробуйте снова"<< std::endl;
            return false;
        }
        m_journalPath = file;
    }
    

    if (!changeLogLevel(log_level_by_dflt))
        return false;
    
    return true;
}

//
// Функция логирования
//
bool Log::logging(const std::string& data, log_level log_level, std::time_t t) const {
    
    if (log_level != log_level::low & log_level != log_level::middle & log_level != log_level::high){
        
        std::cout << "Указанно неверное значение уровня логирования" << std::endl;
        std::cout << "Значение уровня логирования для данного события будет установлено в значение low" << std::endl;
        log_level = log_level::low;
    }

    // Если уровень логирования записи меньше чем уровень по умолчанию, то запись в журнал не осуществляется
    if (log_level < m_log_level_by_default)
        return true;
                                                                
    std::ofstream f (m_journalPath.native(), std::ios_base::app);
        if(f.is_open()){
            std::string s (std::string(std::ctime(&t)) + "log_level:" + std::to_string(static_cast<int>(log_level)) + "\r\n" + data);
            f << s << std::endl;
            f << std::endl;
            f.close();
        }

        else{
            std::cout << "Не удалось открыть файл журнала. Попробуйте снова" << std::endl;
            return false;
        }

    return true;
}

bool Log::logging(const std::string& data, std::time_t t) const {
    
    return logging(data, m_log_level_by_default, t);

}

