#pragma once
#include <string>

#include "Serializable.hpp"
#include "EntityObject.hpp"

enum UserRole {
	ADMIN,
	LiBRARIAN
};

class User : public Serializable, public EntityObject {
	std::string _username;
	std::string _password;
	UserRole _role;
	int _librarianID;
public:
	User(const std::string& username, const std::string& password, UserRole role, int librarianId);
	~User();

	bool authenticate(const std::string& password) const;
	
	std::string getUsername() const;
	std::string getPassword() const;
	UserRole getRole() const;
	int getLibrarianID() const;

	void setUsername(const std::string& username);
	void setPassword(const std::string& password);
	void setRole(const UserRole& role);
	void setLibrarianID(int librarianId);

	std::string serialize() const override;
	std::string toString() const override;
};