#include "Subscription.hpp"

Subscription::Subscription(int id,int library_id, const std::string& name, float price, unsigned int books_limit)
	: _id(id),_library_id(library_id), _name(name), _price(price), _books_limit(books_limit) { }
Subscription::~Subscription() = default;

int Subscription::getId() const { return _id; }
std::string Subscription::getName() const { return _name; }
float Subscription::getPrice() const { return _price; }
unsigned int Subscription::getBooksLimit() const { return _books_limit; }
int Subscription::getLibraryId() const { return _library_id; }

void Subscription::setName(const std::string& name) { _name = name; }
void Subscription::setPrice(float price) { _price = price; }
void Subscription::setBooksLimit(unsigned int books_limit) { _books_limit = books_limit; }

std::string Subscription::serialize() const {
	return std::to_string(_id)
		+ "|" + std::to_string(_library_id) 
		+ "|" + _name 
		+ "|" + std::to_string(_price) 
		+ "|" + std::to_string(_books_limit);
}

std::string Subscription::toString() const {
	return "ID: " + std::to_string(_id) +
		", Бібліотека ID: " + std::to_string(_library_id) +
		", Назва: " + _name +
		", Ціна: " + std::to_string(_price) +
		", Ліміт книг: " + std::to_string(_books_limit);
}