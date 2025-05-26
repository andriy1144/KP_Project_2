#pragma once

#include <vector>
#include <memory>

#include "Literature.hpp"
#include "FileManager.hpp"

class LiteratureManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Literature>> _literature;
	std::vector<std::string> _literatureTypes;
	int _nextLiteratureId;

	FileManager _fileManager;
	void loadLiterature();
public:
	LiteratureManager();
	~LiteratureManager();

	int addLiterature(const std::string& title, const std::string& author, const std::string& type,
		const std::string& inventoryNumber, const std::string& location, bool isReadingRoomOnly,
		int libraryId, int issuePeriodDays);
	
	std::shared_ptr<Literature> getLiteratureById(int id) const;
	
	void archiveLiteratureById(int id);

	void addLiteratureType(const std::string& type);

	bool checkLiteratureIssueDaysValid(int id, int issuePeriodDays) const;

	void printLiteratureTypes() const;
	int printLiteratureMenu();

	std::vector<std::shared_ptr<Literature>> getLiterature() const;
};