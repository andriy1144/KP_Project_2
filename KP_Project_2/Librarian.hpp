#pragma once
#include <string>

#include "Serializable.hpp"
#include "EntityObject.hpp"
#include "PersonInfo.hpp"

class Librarian : public PersonInfo{
private:
    int _id;
	int _libraryId;
    std::string _hallId;

public:
	Librarian(int id, int libraryId, const std::string& firstName,
		const std::string& lastName,
		const std::string& email,
		const std::string& phoneNumber,
		const std::string& address,
		const std::string& city,
		unsigned int age, const std::string& hallId);
	~Librarian();

    int getId() const;
	int getLibraryId() const;
    std::string getHallId() const;

	void setLibraryId(int libraryId);
	void setHallId(const std::string& hallId);


    std::string serialize() const override;
	std::string toString() const override;
};