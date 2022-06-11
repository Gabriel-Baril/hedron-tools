#pragma once
#include <QDate>
#include <vector>

struct TaskHistoryEntry
{
	uint32_t commitNumber;

};

struct TaskHistory
{
	std::vector<TaskHistoryEntry> entries;
};

struct BacklogTask
{
	std::string name;
	std::string type = "FEATURE";
	std::string priority = "MEDIUM";
	std::string resolver = "gbaril";
	std::string reviewer = "gbaril";
	std::string description = "";
	QDate creationDate = QDate::currentDate();
	QDate startDate = QDate::currentDate();
	QDate dueDate = QDate::currentDate();
	uint32_t effortPoint = 0;
	bool completed = false;
	// TaskHistory taskHistory;
};