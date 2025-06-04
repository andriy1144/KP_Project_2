#pragma once
#include <string>

#include "Serializable.hpp"
#include "EntityObject.hpp"
#include "Date.hpp"

class Literature : public Serializable, public EntityObject{
private:
    int _id;
	int _libraryId;
    std::string _title;
    std::string _author;
    std::string _type;
    std::string _inventoryNumber;
    std::string _location;
    bool _isReadingRoomOnly;
    int _issuePeriodDays;
    bool _isArchived = false; 
    Date _entryDate; 
    Date _archiveDate = Date(0); 

public:
    Literature(int id, int libraryId, const std::string& title, const std::string& author, const std::string& type,
        const std::string& inventoryNumber, const std::string& location, bool isReadingRoomOnly, int issuePeriodDays, Date entryDate);
    
    ~Literature();
    
    int getId() const;
    int getLibraryId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getType() const;
    std::string getInventoryNumber() const;
    bool getIsReadingRoomOnly() const;
    int getIssuePeriodDays() const;
    std::string getLocation() const;
    bool isArchived() const;
    Date getEntryDate() const;
    Date getArchiveDate() const;
    
    void setArchived(bool archived, time_t date);
	void setLibraryId(int libraryId);
	void setTitle(const std::string& title);
	void setAuthor(const std::string& author);
	void setType(const std::string& type);
	void setInventoryNumber(const std::string& inventoryNumber);
	void setLocation(const std::string& location);
	void setIsReadingRoomOnly(bool isReadingRoomOnly);
	void setIssuePeriodDays(int issuePeriodDays);
	void setEntryDate(const Date& entryDate);
	void setArchiveDate(const Date& archiveDate);

    std::string serialize() const override;
	std::string toString() const override;
};