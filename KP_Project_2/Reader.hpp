#pragma once

#include <string>
#include <vector>

#include "PersonInfo.hpp"

class Reader : public PersonInfo {
protected:
    int _id;
    std::string _category;
    int _libId;
	int _subscriptionId;
	int _booksCount = 0;

    std::vector<std::pair<std::string, std::string>> _attributes;
public:
    Reader(int id,
        const std::string& firstName,
        const std::string& lastName,
        const std::string& email,
        const std::string& phoneNumber,
        const std::string& address,
        const std::string& city,
        unsigned int age, const std::string& category, int libraryId,
		int subscriptionId, std::vector<std::pair<std::string, std::string>> attributes);

    ~Reader();

    int getId() const;
    std::string getCategory() const;
    int getLibId() const;
	int getSubscriptionId() const;
    int getBooksCount() const;

	void setCategory(const std::string& category);
	void setLibId(int libraryId);
	void setSubscriptionId(int subscriptionId);
	void setBooksCount(int booksCount);

    void setAttribute(const std::string& key, const std::string& value);
    std::vector<std::pair<std::string, std::string>> getAttributes() const;

    std::string toString() const override;
    std::string serialize() const override;
};