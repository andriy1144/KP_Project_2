#pragma once

#include <string>
#include <vector>
#include "Serializable.hpp"
#include <memory>

class FileManager {
private:
	std::string _filename;

	void createFileIfNeeded();
public:
	FileManager();
	FileManager(const std::string& filename);
	~FileManager();

	void saveToFile(const Serializable& data);
	
	void updateFile(const std::vector<std::shared_ptr<Serializable>>& data);
};