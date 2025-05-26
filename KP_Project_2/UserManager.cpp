#include "UserManager.hpp"
#include "ConsoleUtils.hpp"
#include "FileException.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string UserManager::FILE_NAME = "users.txt";

UserManager::UserManager() : _fileManager(FILE_NAME) {
	loadUsers();
}
UserManager::~UserManager() = default;

void UserManager::loadUsers() {
	std::ifstream file(FILE_NAME);
    if (!file.is_open()) throw FileException(FILE_NAME, "Помилка відкриття файлу(файл незнайдений/недоступний)");
	
    std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string username, password, roleStr, librarianIdStr;
		
		if (!std::getline(ss, username, '|') ||
			!std::getline(ss, password, '|') ||
			!std::getline(ss, roleStr, '|') ||
			!std::getline(ss, librarianIdStr)) {
			std::cerr << "Помилка: некоректний формат рядка у файлі: " << line << "\n";
			continue;
		}
		
        try {
            int roleInt = std::stoi(roleStr);
            UserRole role;
            switch (roleInt) {
            case 0: role = UserRole::ADMIN; break;
            case 1: role = UserRole::LiBRARIAN; break;
            default:
                std::cerr << "Помилка: невідома роль " << roleInt << " для користувача " << username << "\n";
                continue;
            }

            int librarianId = -1;
            if (!librarianIdStr.empty()) {
                librarianId = std::stoi(librarianIdStr);
            }

            auto user = std::make_shared<User>(username, password, role, librarianId);
            _users.push_back(user);
        }
        catch (const std::exception& e) {
            ConsoleUtils::printError("Помилка при обробці користувача " + username + ": " + e.what());
        }
    }

    ConsoleUtils::printSuccess(std::to_string(_users.size()) + " користувачів завантажено з файлу.");
    file.close();
}

bool UserManager::login(const std::string& username, const std::string& password) {
    for (const auto& user : _users) {
        if (user->getUsername() == username) {
            if (user->authenticate(password)) {
                _currentUser = user;
                return true;
            }
        }
    }
    return false;
}

bool UserManager::isLogedIn() const { return _currentUser != nullptr; }
bool UserManager::isAdmin() const { return _currentUser && _currentUser->getRole() == UserRole::ADMIN; }

UserRole UserManager::getCurrentUserRole() const { return _currentUser->getRole(); }
int UserManager::getCurrentLibrarianId() const { return _currentUser->getLibrarianID(); }
std::shared_ptr<User> UserManager::getCurrentUser() const { return _currentUser; }

void UserManager::addUser(const std::string& username, const std::string& password, UserRole role, int librarianId = -1) {
    if(checkIfUserWithUsernameExists(username)) {
		throw std::runtime_error("Користувач з таким ім'ям вже існує.");
	}
    auto user = std::make_shared<User>(username, password, role, librarianId);
    _users.push_back(user);
    try {
        _fileManager.saveToFile(*user);
    }
    catch (FileException e) {
        ConsoleUtils::printError(e.what());
        try {
            _fileManager.updateFile(
                std::vector<std::shared_ptr<Serializable>>(
                    _users.begin(), _users.end())
            );
        }
        catch (FileException e) {
            ConsoleUtils::printError(e.what());
            ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
        }
    }
}

void UserManager::deleteUser(const std::string& username) {
    if (_currentUser->getUsername() == username) {
        throw std::runtime_error("Помилка: Не можна видалити власний обліковий запис.");
    }

    bool found = false;
    for (auto it = _users.begin(); it != _users.end(); ++it) {
        if ((*it)->getUsername() == username) {
            std::string confirm = ConsoleUtils::getInputString(
                "Ви впевнені, що хочете видалити " + username + "? (так/ні)");
            if (confirm != "так") return;
            
            it = _users.erase(it);  
            found = true;
            break; 
        }
    }

    if (!found) {
        throw std::runtime_error("Помилка: Не можна видалити власний обліковий запис.");
    }

    _fileManager.updateFile(
        std::vector<std::shared_ptr<Serializable>>(
            _users.begin(), _users.end())
    );

   ConsoleUtils::printSuccess("Користувача '" + username + "' успішно видалено.");
}

bool UserManager::checkIfUserWithUsernameExists(const std::string& username) const {
    for(const auto& user : _users) {
        if (user->getUsername() == username) {
            return true;
        }
	}
	return false;
}

bool UserManager::checkAdminsPermissions() const {
	return isLogedIn() && isAdmin();
}

std::vector<std::shared_ptr<User>> UserManager::getUsers() const {
    return _users;
}

void UserManager::printUsers() const {
	std::cout << "\x1B[34m" << "Список користувачів:\n" << "\033[0m";
    for (const auto& user : _users) {
		std::cout << user->toString() << "\n";
	}
}

void UserManager::printUsers(UserRole role) const {
    std::cout << "\x1B[34m" << "Список користувачів:\n" << "\033[0m";
    for (const auto& user : _users) {
        if (user->getRole() == role) {
            std::cout << user->toString() << "\n";
        }
    }
}