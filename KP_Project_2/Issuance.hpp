#pragma once

#include <ctime>
#include <string>

#include "EntityObject.hpp"
#include "Serializable.hpp"
#include "Date.hpp"

class Issuance : public Serializable, public EntityObject {
private:
    int _issueId;
    int _readerId;
    int _literatureId;
    int _librarianId;
    Date _issueDate;
    Date _dueDate;

	bool _isReturned;
public:
    Issuance(int issueId, int readerId, int literatureId, int librarianId, time_t issueDate, int issuePeriodDays, bool isReturned);
    Issuance(int issueId, int readerId, int literatureId, int librarianId, Date issueDate, int issuePeriodDays, bool isReturned);
    ~Issuance();

    int getIssueId() const;
    int getReaderId() const;
    int getLiteratureId() const;
    int getLibrarianId() const;
    Date getIssueDate() const;
    Date getDueDate() const;
    
	void setReaderId(int readerId);
	void setLiteratureId(int literatureId);
	void setLibrarianId(int librarianId);
	void setIssueDate(Date issueDate);
	void setDueDate(Date dueDate);

	void setIsReturned(bool isReturned);
	bool isReturned() const;

    std::string serialize() const override;
	std::string toString() const override;
};