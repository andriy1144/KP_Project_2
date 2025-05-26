// NEW FILE ADDED
#pragma once

#include <vector>
#include <memory>

#include "User.hpp"
#include "FileManager.hpp"

class UserManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<User>> _users;
	std::shared_ptr<User> _currentUser = nullptr;

	FileManager _fileManager;
	void loadUsers();
	bool checkIfUserWithUsernameExists(const std::string& username) const;
public:
	UserManager();
	~UserManager();

	bool login(const std::string& username, const std::string& password);

	bool isLogedIn() const;
	bool isAdmin() const;
	bool checkAdminsPermissions() const;
	void printUsers() const;
	void printUsers(UserRole role) const;

	std::vector<std::shared_ptr<User>> getUsers() const;
	UserRole getCurrentUserRole() const;
	int getCurrentLibrarianId() const;
	std::shared_ptr<User> getCurrentUser() const;

	void addUser(const std::string& username, const std::string& password, UserRole role, int librarianId);
	void deleteUser(const std::string& username);
};