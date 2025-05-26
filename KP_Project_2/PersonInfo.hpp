#pragma once

#include "EntityObject.hpp"
#include "Serializable.hpp"

class PersonInfo : public Serializable, public EntityObject{
protected:
	std::string _firstName;
	std::string _lastName;
	unsigned int _age;
	std::string _email;
	std::string _phoneNumber;
	std::string _address;
	std::string _city;
public:
	PersonInfo(const std::string& firstName, 
		const std::string& lastName, 
		const std::string& email, 
		const std::string& phoneNumber, 
		const std::string& address, 
		const std::string& city, 
		unsigned int age);

	virtual ~PersonInfo() = default;

	std::string getFirstName() const;
	std::string getLastName() const;
	std::string getEmail() const;
	std::string getPhoneNumber() const;
	std::string getAddress() const;
	std::string getCity() const;
	unsigned int getAge() const;

	void setFirstName(const std::string& firstName);
	void setLastName(const std::string& lastName);
	void setEmail(const std::string& email);
	void setPhoneNumber(const std::string& phoneNumber);
	void setAddress(const std::string& address);
	void setCity(const std::string& city);
	void setAge(unsigned int age);

	std::string serialize() const override;
	std::string toString() const override;
};