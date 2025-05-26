#pragma once

#include <string>
#include <vector>
#include <memory>

#include "FileManager.hpp"
#include "Subscription.hpp"

class SubscriptionManager {
private:
	static const std::string FILE_NAME;

	std::vector<std::shared_ptr<Subscription>> _subscriptions;
	int _nextSubscriptionId;

	void loadSubscriptions();
	FileManager _fileManager;
public:
	SubscriptionManager();
	~SubscriptionManager();

	int addSubscription(const std::string& name, float price, unsigned int books_limit, int libId);
	std::shared_ptr<Subscription> getSubscriptionById(int id) const;
	std::shared_ptr<Subscription> getSubscriptionByIdAndLibraryId(int id, int libraryId) const;

	void deleteSubscriptionById(int id);

	int printSubscriptionMenu();
	int printSubscriptionMenuByLibraryId(int libraryId) const;

	bool checkSubscriptionLimitValid(int bookCount, int subscriptionId) const;

	std::vector<std::shared_ptr<Subscription>> getSubscriptions() const;
	std::vector<std::shared_ptr<Subscription>> getSubscriptionsByLibraryId(int libraryId) const;
};