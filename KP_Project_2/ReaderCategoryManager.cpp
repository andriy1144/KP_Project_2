#include "ReaderCategoryManager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"
#include "ConsoleUtils.hpp"
#include "StringUtils.hpp"

const std::string ReaderCategoryManager::FILE_NAME = "reader_categories.txt";

ReaderCategoryManager::ReaderCategoryManager() : 
	_nextReaderCategoryId(1), _fileManager(FILE_NAME) {
	loadReaderCategories();
}
ReaderCategoryManager::~ReaderCategoryManager() = default;

void ReaderCategoryManager::loadReaderCategories(){
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "������� �������� �����(���� �����������/�����������)");

	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, name, attributes;

			std::getline(ss, id, '|');
			std::getline(ss, name, '|');
			std::getline(ss, attributes, '|');

			std::vector<std::string> attrs = StringUtils::split(attributes,';');

			int categoryId = std::stoi(id);
			auto category = std::make_shared<ReaderCategory>(categoryId, name, attrs);

			_readerCategories.push_back(category);
			if (categoryId >= _nextReaderCategoryId) {
				_nextReaderCategoryId = categoryId + 1;
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError("������� �� ��� ���������� ����� ����� " + FILE_NAME);
			ConsoleUtils::printError(e.what());
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_readerCategories.size()) + " ��������� �������� ����������� � �����.");
	
	file.close();
}

std::shared_ptr<ReaderCategory> ReaderCategoryManager::getReaderCategoryById(int id) const {
	for(const auto& category : _readerCategories) {
		if (category->getId() == id) {
			return category;
		}
	}
	throw EntityNotFoundException("�������� ������", "�������� ������ � id - " + std::to_string(id) + " �� ��������!");
}

std::pair<std::string, std::vector<std::string>> ReaderCategoryManager::showCategoriesMenuAndGetAttrs() const {
	if (_readerCategories.empty()) throw std::runtime_error("������� ������ ������!");

	std::cout << "������� ������� �������:" << std::endl;
	for (const auto& category : _readerCategories) {
		std::cout << "\t" << category->getId() << ". " << category->getName() << std::endl;
	}

	int choice = ConsoleUtils::getInputInt("������ ����� ������� �� ��������� ������: ");

	std::pair<std::string, std::vector<std::string >> categoryResult;
	for (const auto& category : _readerCategories) {
		if (category->getId() == choice) {
			categoryResult.first = category->getName();
			categoryResult.second = category->getCategoryAttributes();
			return categoryResult;
		}
	}
	
	throw EntityNotFoundException("�������� ������", "�������� ������ � id - " + std::to_string(choice) + " �� ��������!");
}

int ReaderCategoryManager::printReaderCategoriesMenu() const {
	std::cout << "���� �������� �������:" << std::endl;
	for(const auto& category : _readerCategories) {
		std::cout << "\t" << category->getId() << ". " << category->getName() << std::endl;
	}

	int choice = ConsoleUtils::getInputInt("������ ����� ������� ������: ");
	return getReaderCategoryById(choice)->getId();
}

std::vector<std::pair<std::string, std::string>> ReaderCategoryManager::getInputCategoryAttributes(std::vector<std::string> attributesToGet) const {
	std::vector<std::pair<std::string, std::string>> attributes;
	for (const auto& attr : attributesToGet) {
		std::string value = ConsoleUtils::getInputString("������ " + attr + ": ");
		attributes.push_back({ attr, value });
	}
	return attributes;
}

int ReaderCategoryManager::addReaderCategory(const std::string& name, const std::string attributes) {
	std::vector<std::string> attrs = StringUtils::split(attributes, ',');
	
	auto category = std::make_shared<ReaderCategory>(_nextReaderCategoryId, name, attrs);
	_readerCategories.push_back(category);
	
	try {
		_fileManager.saveToFile(*category);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(
				std::vector<std::shared_ptr<Serializable>>(
					_readerCategories.begin(), _readerCategories.end())
			);
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
		}
	}
	return _nextReaderCategoryId++;
}

void ReaderCategoryManager::deleteReaderCategoryById(int id) {
	for (auto it = _readerCategories.begin(); it != _readerCategories.end(); ++it) {
		if ((*it)->getId() == id) {
			_readerCategories.erase(it);
			try {
				_fileManager.updateFile(
					std::vector<std::shared_ptr<Serializable>>(
						_readerCategories.begin(), _readerCategories.end())
				);
			}
			catch (FileException e) {
				ConsoleUtils::printError(e.what());
				ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
			}
			return;
		}
	}
	throw EntityNotFoundException("�������� ������", "�������� ������ � id - " + std::to_string(id) + " �� ��������!");
}

std::vector<std::shared_ptr<ReaderCategory>> ReaderCategoryManager::getReaderCategories() const {
	return _readerCategories;
}



