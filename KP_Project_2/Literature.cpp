#include "Literature.hpp"


Literature::Literature(int id, int libraryId, const std::string& title, const std::string& author, const std::string& type,
    const std::string& inventoryNumber, const std::string& location, bool isReadingRoomOnly, int issuePeriodDays, Date entryDate)
    : _id(id), _libraryId(libraryId), _title(title), _author(author), _type(type), _inventoryNumber(inventoryNumber), _location(location),
    _isReadingRoomOnly(isReadingRoomOnly), _issuePeriodDays(issuePeriodDays),
    _entryDate(entryDate){}


Literature::~Literature() = default;

int Literature::getId() const { return _id; }
int Literature::getLibraryId() const { return _libraryId; }
std::string Literature::getTitle() const { return _title; }
std::string Literature::getAuthor() const { return _author; }
std::string Literature::getType() const { return _type; }
std::string Literature::getInventoryNumber() const { return _inventoryNumber; }
bool Literature::getIsReadingRoomOnly() const { return _isReadingRoomOnly; }
int Literature::getIssuePeriodDays() const { return _issuePeriodDays; }
std::string Literature::getLocation() const { return _location; }
bool Literature::isArchived() const { return _isArchived; }
Date Literature::getEntryDate() const { return _entryDate; }
Date Literature::getArchiveDate() const { return _archiveDate; }

void Literature::setTitle(const std::string& title) { _title = title; }
void Literature::setAuthor(const std::string& author) { _author = author; }
void Literature::setType(const std::string& type) { _type = type; }
void Literature::setInventoryNumber(const std::string& inventoryNumber) { _inventoryNumber = inventoryNumber; }
void Literature::setLocation(const std::string& location) { _location = location; }
void Literature::setIsReadingRoomOnly(bool isReadingRoomOnly) { _isReadingRoomOnly = isReadingRoomOnly; }
void Literature::setIssuePeriodDays(int issuePeriodDays) { _issuePeriodDays = issuePeriodDays; }
void Literature::setEntryDate(const Date& entryDate) { _entryDate = entryDate; }
void Literature::setArchiveDate(const Date& archiveDate) { _archiveDate = archiveDate; }
void Literature::setArchived(bool archived, time_t date) {
    _isArchived = archived;
    _archiveDate = Date(date);
}

std::string Literature::serialize() const {
    return std::to_string(_id) + "|" + std::to_string(_libraryId) + "|" + _title + "|" + _author + "|" 
        + _type + "|" + _inventoryNumber + "|" + _location + "|" +
        std::to_string(_isReadingRoomOnly) + "|" + std::to_string(_issuePeriodDays) + 
        "|" + std::to_string(_entryDate.getDate()) + "|" + (_isArchived ? "1" : "0")
        + "|" + std::to_string(_archiveDate.getDate());
}
std::string Literature::toString() const {
	return "ID: " + std::to_string(_id) + ", Бібліотека ID: "+ std::to_string(_libraryId) + ", Назва: " + _title + ", Автор: " + _author + ", Видання: " + _type +
		", Інвентарний номер: " + _inventoryNumber + ", Локація(полиця): " + _location +
		", Тільки читальний зал: " + (_isReadingRoomOnly ? "Так" : "Ні") +
		", Термін для видачі(днів): " + std::to_string(_issuePeriodDays) + 
        ", Дата реєстрації: " + _entryDate.toString() +
        ", Списана: " + (_isArchived ? "Так, Дата списання: " + _archiveDate.toString() : "Ні");
}