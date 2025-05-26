#include "FileManager.hpp"
#include "FileException.hpp"

#include <fstream>
#include <iostream>
#include "ConsoleUtils.hpp"

FileManager::FileManager() = default;
FileManager::FileManager(const std::string& filename) : _filename(filename) {
	createFileIfNeeded();
}
FileManager::~FileManager() = default;

void FileManager::createFileIfNeeded() {
	std::ifstream fileToRead(_filename);
	if (!fileToRead.is_open()) {
		std::ofstream createdFile(_filename);
		if(!createdFile.is_open()) throw FileException(_filename, "Помилка створення файлу!");
		
		ConsoleUtils::printSuccess("Файл " + _filename + " успішно створений!");
		createdFile.close();
	}
	fileToRead.close();
}

void FileManager::saveToFile(const Serializable& data) {
	std::ofstream file(_filename, std::ios::app);
	if (!file.is_open()) throw FileException(_filename,"Невдалося відкрити файл для запису");
	file << data.serialize() << "\n";
	file.close();
}

void FileManager::updateFile(const std::vector<std::shared_ptr<Serializable>>& data) {
	std::ofstream file(_filename, std::ios::trunc);
	if (!file.is_open()) throw FileException(_filename, "Невдалося відкрити файл для запису");
	for (const auto& item : data) {
		file << item->serialize() << "\n";
	}
	file.close();
}