#pragma once

#include <vector>
#include<memory>

#include "Library.hpp"
#include "FileManager.hpp"

class LibraryManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Library>> _libraries;
	int _nextLibraryId;

	FileManager _fileManager;
	void loadLibraries();
public:
	LibraryManager();
	~LibraryManager();

	int addLibrary(const std::string& name, const std::string& address, const std::string& halls);
	std::shared_ptr<Library> getLibraryById(int id) const;

	void deleteLibraryById(int id);

	int printLibraryMenu();
	std::string printHallsByLibraryId(int libraryId) const;
	std::vector<std::shared_ptr<Library>> getLibraries() const;
};