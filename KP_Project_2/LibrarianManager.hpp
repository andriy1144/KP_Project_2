#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Librarian.hpp"
#include "FileManager.hpp"

class LibrarianManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Librarian>> _librarians;
	int _nextLibrarianId;

	void loadLibrarians();
	FileManager _fileManager;
public:
	LibrarianManager();
	~LibrarianManager();

	int addLibrarian(const std::string& firstName,
		const std::string& lastName, const std::string& email, const std::string& phoneNumber,
		const std::string& address, const std::string& city, const int& age,
		const std::string& hallId, const int& libId);

	std::shared_ptr<Librarian> getLibrarianById(int id) const;
	void deleteLibrarianById(int id);

	int printLibrarianMenu();
	std::vector<std::shared_ptr<Librarian>> getLibrarians() const;
};