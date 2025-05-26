#include "IssuanceManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "EntityNotFoundException.hpp"
#include "ConsoleUtils.hpp"

#include "FileException.hpp"

const std::string IssuanceManager::FILE_NAME = "issues.txt";

IssuanceManager::IssuanceManager() : 
	_nextIssueId(1), _fileManager(FILE_NAME) {
	loadIssues();
}
IssuanceManager::~IssuanceManager() = default;

void IssuanceManager::loadIssues() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME,"Помилка відкриття файлу(файл незнайдений/недоступний)");

	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string issueId, readerId, literatureId, librarianId, issueDate, dueDate, isReturned;
			std::getline(ss, issueId, '|');
			std::getline(ss, readerId, '|');
			std::getline(ss, literatureId, '|');
			std::getline(ss, librarianId, '|');
			std::getline(ss, issueDate, '|');
			std::getline(ss, dueDate, '|');
			std::getline(ss, isReturned, '|');

			int sId = std::stoi(issueId);
			auto issue = std::make_shared<Issuance>(sId, std::stoi(readerId), std::stoi(literatureId), std::stoi(librarianId),
				std::stoll(issueDate), (std::stoll(dueDate) - std::stoll(issueDate)) / (24 * 60 * 60),
				(isReturned == "0" ? false : true));

			_issues.push_back(issue);
			if (sId >= _nextIssueId) {
				_nextIssueId = sId + 1;
			}
		}
		catch (...) {
			ConsoleUtils::printError("Помилка під час зчитування рядка файлу " + FILE_NAME);
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_issues.size()) + " видач завантажено з файлу.");

	file.close();
}

std::shared_ptr<Issuance> IssuanceManager::getIssuanceById(int id) const {
	for (const auto& issue : _issues) {
		if (issue->getIssueId() == id) {
			return issue;
		}
	}
	throw EntityNotFoundException("Видача", "Видачу з id - " + std::to_string(id) + " не знайдено!");
}

int IssuanceManager::printIssuesMenu() const {
	if (_issues.empty()) throw std::runtime_error("Видач немає!");

	std::cout << "Меню видач: " << std::endl;
	for (const auto& issue : _issues) {
		if (issue->isReturned()) continue;
		std::cout << issue->toString() << std::endl;
	}

	int id = ConsoleUtils::getInputInt("Введіть номер видачі з верхнього списку: ");

	return getIssuanceById(id)->getIssueId();
}

int IssuanceManager::issueLiterature(int readerId, int literatureId, int librarianId, int issuePeriodDays) {
	auto issue = std::make_shared<Issuance>(_nextIssueId, readerId, literatureId, librarianId, time(nullptr), issuePeriodDays, false);
	_issues.push_back(issue);
	try {
		_fileManager.saveToFile(*issue);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(std::vector<std::shared_ptr<Serializable>>(_issues.begin(), _issues.end()));
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати більшої кількості даних перезапустіть систему та спробуйте знову");
		}
	}
	return _nextIssueId++;
}

void IssuanceManager::closeIssue(int issueId) {
	std::shared_ptr<Issuance> issue = getIssuanceById(issueId);
	if (issue->isReturned()) throw std::runtime_error("Видача вже закрита!");
	issue->setIsReturned(true);

	try {
		_fileManager.updateFile(std::vector<std::shared_ptr<Serializable>>(_issues.begin(), _issues.end()));
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати більшої кількості даних перезапустіть систему та спробуйте знову");
	}
}

bool IssuanceManager::checkIfReaderHasCurrentIssues(int readerId) const {
	for (const auto& issue : _issues) {
		if (issue->getReaderId() == readerId && !issue->isReturned()) {
			return true;
		}
	}
	return false;
}

std::vector<std::shared_ptr<Issuance>> IssuanceManager::getIssues() const {
	return _issues;
}