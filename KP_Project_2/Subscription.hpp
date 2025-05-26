#pragma once

#include <string>

#include "Serializable.hpp"
#include "EntityObject.hpp"

class Subscription : public Serializable, public EntityObject {
private:
	int _id;
	int _library_id;
	std::string _name;
	float _price;
	unsigned int _books_limit;

public:
	Subscription(int id,int library_id, const std::string& name, float price, unsigned int books_limit);
	~Subscription();

	int getId() const;
	int getLibraryId() const;
	std::string getName() const;
	float getPrice() const;
	unsigned int getBooksLimit() const;

	void setName(const std::string& name);
	void setPrice(float price);
	void setBooksLimit(unsigned int books_limit);

	std::string serialize() const override;
	std::string toString() const override;
};