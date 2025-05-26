#define _CRT_SECURE_NO_WARNINGS
#include "Date.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>
#include<iostream>

Date::Date() = default;
Date::Date(time_t date) : _date(date) {}
Date::Date(const Date& other) : _date(other._date) {} 
Date::~Date() = default;
Date& Date::operator=(const Date& other) {   
    if (this != &other) {
        _date = other._date;
    }
    return *this;
}

std::string Date::formatDate() const {
    std::ostringstream oss;
    std::tm* tm = std::localtime(&_date);
    oss << std::put_time(tm, _format.c_str());
    return oss.str();
}
void Date::parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream iss(dateStr);
    iss >> std::get_time(&tm, _format.c_str());
    if (iss.fail()) {
        throw std::invalid_argument("Невірний формат дати. Очікуваний формат: " + _format);
    }
    _date = std::mktime(&tm);
}

time_t Date::getDate() const {
	return _date;
}

std::string Date::toString() const {
	return formatDate();
}
std::string Date::serialize() const {
	return std::to_string(_date);
}

time_t Date::getInputDate(const std::string& prompt) {
    std::cout << prompt << " (формат: YYYY-MM-DD): ";
    std::string dateStr;
    std::getline(std::cin, dateStr);
    int year, month, day;
    sscanf_s(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
    struct tm timeinfo = { 0 };
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    return mktime(&timeinfo);
}