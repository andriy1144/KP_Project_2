#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Issuance.hpp"
#include "FileManager.hpp"

class IssuanceManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Issuance>> _issues;
	int _nextIssueId;

	void loadIssues();
	FileManager _fileManager;
public:
	IssuanceManager();
	~IssuanceManager();

	int printIssuesMenu() const;
	std::shared_ptr<Issuance> getIssuanceById(int issuanceId) const;

	int issueLiterature(int readerId, int literatureId, int librarianId, int loanPeriodDays);
	void closeIssue(int issuanceId);

	bool checkIfReaderHasCurrentIssues(int readerId) const;
	std::vector<std::shared_ptr<Issuance>> getIssues() const;
};