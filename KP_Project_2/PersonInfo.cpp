#include "PersonInfo.hpp"


PersonInfo::PersonInfo(const std::string& firstName, 
	const std::string& lastName, 
	const std::string& email, 
	const std::string& phoneNumber, 
	const std::string& address, 
	const std::string& city,
	unsigned int age)
	: _firstName(firstName), _lastName(lastName), _email(email),
	_phoneNumber(phoneNumber), _address(address), _city(city),
	_age(age){ }

void PersonInfo::setFirstName(const std::string& firstName) { _firstName = firstName; }
void PersonInfo::setLastName(const std::string& lastName) { _lastName = lastName; }
void PersonInfo::setEmail(const std::string& email) { _email = email; }
void PersonInfo::setPhoneNumber(const std::string& phoneNumber) { _phoneNumber = phoneNumber; }
void PersonInfo::setAddress(const std::string& address) { _address = address; }
void PersonInfo::setCity(const std::string& city) { _city = city; }
void PersonInfo::setAge(unsigned int age) { _age = age; }

std::string PersonInfo::getFirstName() const { return _firstName; }
std::string PersonInfo::getLastName() const { return _lastName; }
std::string PersonInfo::getEmail() const { return _email; }
std::string PersonInfo::getPhoneNumber() const { return _phoneNumber; }
std::string PersonInfo::getAddress() const { return _address; }
std::string PersonInfo::getCity() const { return _city; }
unsigned int PersonInfo::getAge() const { return _age; }

std::string PersonInfo::serialize() const {
	return _firstName + "|" + _lastName + "|" + _email + "|" +
		_phoneNumber + "|" + _address + "|" + _city + "|" +
		std::to_string(_age);
}

std::string PersonInfo::toString() const {
	return "Ім'я: " + _firstName + ", Прізвище: " + _lastName +
		", Елект. пошта: " + _email +
		", Номер телефону: " + _phoneNumber  +
		", Адреса: " + _address  +
		", Місто: " + _city  +
		", Вік: " + std::to_string(_age);
}