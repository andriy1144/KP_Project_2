#include "SubscriptionManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"
#include "StringUtils.hpp"
#include "ConsoleUtils.hpp"


const std::string SubscriptionManager::FILE_NAME = "libraries_subscriptions.txt";

SubscriptionManager::SubscriptionManager() : 
	_nextSubscriptionId(1), _fileManager(FILE_NAME) { 
	loadSubscriptions();
}
SubscriptionManager::~SubscriptionManager() = default;

void SubscriptionManager::loadSubscriptions() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "������� �������� �����(���� �����������/�����������)");

	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, library_id, name, price, books_limit;

			std::getline(ss, id, '|');
			std::getline(ss, library_id, '|');
			std::getline(ss, name, '|');
			std::getline(ss, price, '|');
			std::getline(ss, books_limit, '|');

			int idInt = std::stoi(id);
			int libraryIdInt = std::stoi(library_id);
			double priceFloat = std::stof(price);
			int booksLimitInt = std::stoi(books_limit);

			auto subscription = std::make_shared<Subscription>(idInt, libraryIdInt, name, priceFloat, booksLimitInt);

			_subscriptions.push_back(subscription);
			if (idInt >= _nextSubscriptionId) {
				_nextSubscriptionId = idInt + 1;
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError("������� �� ��� ���������� ����� ����� " + FILE_NAME);
			ConsoleUtils::printError(e.what());
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_subscriptions.size()) + " ������������ ���������� ����������� � �����.");

	file.close();
}

int SubscriptionManager::addSubscription(const std::string& name, float price, 
	unsigned int books_limit, int libId) {
	
	auto subscription = std::make_shared<Subscription>(_nextSubscriptionId, libId, name, price, books_limit);

	_subscriptions.push_back(subscription);

	try {
		_fileManager.saveToFile(*subscription);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(
				std::vector<std::shared_ptr<Serializable>>(
					_subscriptions.begin(), _subscriptions.end())
			);
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
		}
	}

	return _nextSubscriptionId++;
}

std::shared_ptr<Subscription> SubscriptionManager::getSubscriptionById(int id) const {
	for (const auto& subscription : _subscriptions) {
		if (subscription->getId() == id) {
			return subscription;
		}
	}
	throw EntityNotFoundException("���������", "��������� � id " + std::to_string(id) + " �� ��������.");
}

std::shared_ptr<Subscription> SubscriptionManager::getSubscriptionByIdAndLibraryId(int id, int libraryId) const {
	for (const auto& subscription : _subscriptions) {
		if (subscription->getId() == id && subscription->getLibraryId() == libraryId) {
			return subscription;
		}
	}
	throw EntityNotFoundException("���������", "��������� � id " + std::to_string(id) + " �� ��������.");
}

void SubscriptionManager::deleteSubscriptionById(int id) {
	for (auto it = _subscriptions.begin(); it != _subscriptions.end(); ++it) {
		if ((*it)->getId() == id) {
			_subscriptions.erase(it);
			try {
				_fileManager.updateFile(
					std::vector<std::shared_ptr<Serializable>>(
						_subscriptions.begin(), _subscriptions.end())
				);
			}
			catch (FileException e) {
				ConsoleUtils::printError(e.what());
				ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
			}
			return;
		}
	}
	throw EntityNotFoundException("���������", "��������� � id " + std::to_string(id) + " �� ��������.");
}

bool SubscriptionManager::checkSubscriptionLimitValid(int bookCount, int subscriptionId) const {
	const auto& subscription = getSubscriptionById(subscriptionId);
	return bookCount >= subscription->getBooksLimit();
}

int SubscriptionManager::printSubscriptionMenu() {
	if (_subscriptions.empty()) throw std::runtime_error("���������� ����.");

	std::cout << "������� ���������� �������:" << std::endl;
	for (const auto& subscription : _subscriptions) {
		std::cout << subscription->toString();
		ConsoleUtils::printDelimeter('-', 100);
	}

	int choice = ConsoleUtils::getInputInt("������ ����� ���������� �� ��������� ������: ");

	return getSubscriptionById(choice)->getId();
}

int SubscriptionManager::printSubscriptionMenuByLibraryId(int libraryId) const {
	if (_subscriptions.empty()) throw std::runtime_error("���������� ����.");

	const auto& librarySubscriptions = getSubscriptionsByLibraryId(libraryId);

	if(librarySubscriptions.empty()) throw std::logic_error("���������� ��� �������� � id " + std::to_string(libraryId) + " ����.");

	std::cout << "������� ���������� ������� ��������:" << std::endl;
	for (const auto& subscription : librarySubscriptions) {
		std::cout << subscription->toString();
		ConsoleUtils::printDelimeter('-', 100);
	}

	int choice = ConsoleUtils::getInputInt("������ ����� ���������� �� ��������� ������: ");
	return getSubscriptionByIdAndLibraryId(choice, libraryId)->getId();
}

std::vector<std::shared_ptr<Subscription>> SubscriptionManager::getSubscriptions() const {
	return _subscriptions;
}

std::vector<std::shared_ptr<Subscription>> SubscriptionManager::getSubscriptionsByLibraryId(int libraryId) const {
	std::vector<std::shared_ptr<Subscription>> subscriptionsByLibraryId;
	for (const auto& subscription : _subscriptions) {
		if (subscription->getLibraryId() == libraryId) {
			subscriptionsByLibraryId.push_back(subscription);
		}
	}
	return subscriptionsByLibraryId;
}