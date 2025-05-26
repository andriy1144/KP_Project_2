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
    if (!file.is_open()) throw FileException(FILE_NAME, "������� �������� �����(���� �����������/�����������)");
	
    std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string username, password, roleStr, librarianIdStr;
		
		if (!std::getline(ss, username, '|') ||
			!std::getline(ss, password, '|') ||
			!std::getline(ss, roleStr, '|') ||
			!std::getline(ss, librarianIdStr)) {
			std::cerr << "�������: ����������� ������ ����� � ����: " << line << "\n";
			continue;
		}
		
        try {
            int roleInt = std::stoi(roleStr);
            UserRole role;
            switch (roleInt) {
            case 0: role = UserRole::ADMIN; break;
            case 1: role = UserRole::LiBRARIAN; break;
            default:
                std::cerr << "�������: ������� ���� " << roleInt << " ��� ����������� " << username << "\n";
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
            ConsoleUtils::printError("������� ��� ������� ����������� " + username + ": " + e.what());
        }
    }

    ConsoleUtils::printSuccess(std::to_string(_users.size()) + " ������������ ����������� � �����.");
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
		throw std::runtime_error("���������� � ����� ��'�� ��� ����.");
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
            ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
        }
    }
}

void UserManager::deleteUser(const std::string& username) {
    if (_currentUser->getUsername() == username) {
        throw std::runtime_error("�������: �� ����� �������� ������� �������� �����.");
    }

    bool found = false;
    for (auto it = _users.begin(); it != _users.end(); ++it) {
        if ((*it)->getUsername() == username) {
            std::string confirm = ConsoleUtils::getInputString(
                "�� �������, �� ������ �������� " + username + "? (���/�)");
            if (confirm != "���") return;
            
            it = _users.erase(it);  
            found = true;
            break; 
        }
    }

    if (!found) {
        throw std::runtime_error("�������: �� ����� �������� ������� �������� �����.");
    }

    _fileManager.updateFile(
        std::vector<std::shared_ptr<Serializable>>(
            _users.begin(), _users.end())
    );

   ConsoleUtils::printSuccess("����������� '" + username + "' ������ ��������.");
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
	std::cout << "\x1B[34m" << "������ ������������:\n" << "\033[0m";
    for (const auto& user : _users) {
		std::cout << user->toString() << "\n";
	}
}

void UserManager::printUsers(UserRole role) const {
    std::cout << "\x1B[34m" << "������ ������������:\n" << "\033[0m";
    for (const auto& user : _users) {
        if (user->getRole() == role) {
            std::cout << user->toString() << "\n";
        }
    }
}