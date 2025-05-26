#define _CRT_SECURE_NO_WARNINGS
#include "Issuance.hpp"
#include <iomanip>
#include <sstream>


Issuance::Issuance(int issueId, int readerId, int literatureId, int librarianId, time_t issueDate, int issuePeriodDays, bool isReturned = false) :
    _issueId(issueId), _readerId(readerId), _literatureId(literatureId), _librarianId(librarianId), _issueDate(issueDate), _isReturned(isReturned) {
    _dueDate = Date(issueDate + issuePeriodDays * 24 * 60 * 60);
}

Issuance::Issuance(int issueId, int readerId, int literatureId, int librarianId, Date issueDate, int issuePeriodDays, bool isReturned = false)
    : _issueId(issueId), _readerId(readerId), _literatureId(literatureId), _librarianId(librarianId), _issueDate(issueDate), _isReturned(isReturned) {
    _dueDate = Date(issueDate.getDate() + issuePeriodDays * 24 * 60 * 60);
}

Issuance::~Issuance() = default;

int Issuance::getIssueId() const { return _issueId; }
int Issuance::getReaderId() const { return _readerId; }
int Issuance::getLiteratureId() const { return _literatureId; }
int Issuance::getLibrarianId() const { return _librarianId; }
Date Issuance::getIssueDate() const { return _issueDate; }
Date Issuance::getDueDate() const { return _dueDate; }

void Issuance::setReaderId(int readerId) { _readerId = readerId; }
void Issuance::setLiteratureId(int literatureId) { _literatureId = literatureId; }
void Issuance::setLibrarianId(int librarianId) { _librarianId = librarianId; }
void Issuance::setIssueDate(Date issueDate) { _issueDate = issueDate; }
void Issuance::setDueDate(Date dueDate) { _dueDate = dueDate; }


bool Issuance::isReturned() const { return _isReturned; }
void Issuance::setIsReturned(bool isReturned) { _isReturned = isReturned; }

std::string Issuance::serialize() const {
    return std::to_string(_issueId) + "|" + std::to_string(_readerId) + "|" + std::to_string(_literatureId) + "|" +
        std::to_string(_librarianId) + "|" + std::to_string(_issueDate.getDate()) + "|" + std::to_string(_dueDate.getDate()) 
        + "|" + (_isReturned ? "1" : "0");
}
std::string Issuance::toString() const {
	return "ID: " + std::to_string(_issueId) + ", Читач ID: " + std::to_string(_readerId) +
		", Література ID: " + std::to_string(_literatureId) + ", Бібліотекар ID: " + std::to_string(_librarianId) +
		", Дата видачі: " + _issueDate.formatDate() + ", Здати до: " + _dueDate.formatDate() 
        + ", Повернута: " + (_isReturned ? "Так" : "Ні");
}