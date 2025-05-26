#include "User.hpp"

#include "StringUtils.hpp"

User::User(const std::string& username, const std::string& password, UserRole role, int librarianId=-1) :
	_username(username), _password(password), _role(role), _librarianID(librarianId) {}
User::~User() = default;

bool User::authenticate(const std::string& password) const {
	return _password == password;
}

std::string User::getUsername() const { return _username; }
std::string User::getPassword() const { return _password; }
UserRole User::getRole() const { return _role; }
int User::getLibrarianID() const { return _librarianID; }

void User::setLibrarianID(int librarianId) { _librarianID = librarianId; }
void User::setUsername(const std::string& username) { _username = username; }
void User::setPassword(const std::string& password) { _password = password; }
void User::setRole(const UserRole& role) { _role = role; }

std::string User::toString() const {
	std::string roleStr = (_role == UserRole::ADMIN) ? "Адміністратор" : "Бібліотекар\nБібліотека ID: " + std::to_string(_librarianID);
	return "Нікнейм: " + _username +
		"\nРоль: " + roleStr;
}
std::string User::serialize() const {
	return _username + "|" + _password + "|" +
		std::to_string(static_cast<int>(_role)) + "|" +
		std::to_string(_librarianID);
}