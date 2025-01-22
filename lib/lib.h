#pragma once
#include <string>
#include <filesystem>
#include <ctime>

//
// Уровень логирования сообщений
//
enum class log_level {
    low = 0,
    middle = 5,
    high = 10
};

class Log {

public:
    Log();

    //
    // Инициализация библиотеки логирования
    //
    bool Init(const std::string& journal, log_level log_level_by_default);

    //
    // Изменить уровень логирования по умолчанию
    //
    bool changeLogLevel (log_level log_level_by_default);

    //
    // Записать данные в журнал
    //
    bool logging(const std::string& data, log_level log_level, std::time_t t) const;

    bool logging(const std::string& data, std::time_t t) const;

private:
    log_level m_log_level_by_default;
    std::filesystem::path m_journalPath;

};