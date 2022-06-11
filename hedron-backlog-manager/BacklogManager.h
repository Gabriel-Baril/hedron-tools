#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BacklogManager.h"

#include <QJsonDocument>
#include <QStringList>
#include <QJsonObject>
#include <QDate>
#include <QListWidget>
#include <QCompleter>
#include <QStringListModel>
#include <QCloseEvent>

#include "EditTaskDialog.h"
#include "json.hpp"

#include "BacklogTask.h"

constexpr unsigned ITEM_PAGE_COUNT = 50;
constexpr const char* BACKLOG_FILE = "backlog.json";

using json_ptr = nlohmann::json_pointer<nlohmann::json::string_t>;

class BacklogManager : public QMainWindow
{
    Q_OBJECT

public:
    BacklogManager(QWidget *parent = nullptr);
    ~BacklogManager();
    void ReadBacklog(const char* filename);
	void GetFlatTaskList(const nlohmann::json& rootNode, std::vector<BacklogTask>& outTastList);

	void SearchButtonClicked(bool checked);
	void UpdateTaskFromURIFilter(const std::string& uriFilter);
	void UpdateTaskView();
	void RefreshTaskView();

	void OnDoubleClickTaskItem(QListWidgetItem* item);
	void CreateTaskCallback();
	void ShowCompletedTaskCallback();
	void OnEditTaskAccepted();
	
	void CreateTask(const json_ptr& path);
	void DeleteTask(const BacklogTask& task);
	void RenameTask(BacklogTask& task, const std::string& newPath);
	void SaveBacklog();

	void closeEvent(QCloseEvent* event) override;

	void NextItemPage();
	void PreviousItemPage();
	void ResetItemPage();

	void ShowContextMenu(const QPoint& pos);
	void OnDeleteSelectedTasks();
	void CreateActions();
private:
	QAction* deleteTaskAction;

	bool m_MadeChangeToBacklog = false;
	QListWidgetItem* m_CurrentItemModified;

    Ui::BacklogManagerClass ui;
	nlohmann::json m_JsonBacklog;
	
	EditTaskDialog m_EditDialog;
	QListWidget m_Widget;
	std::vector<BacklogTask> m_CurrentTaskList;
	QCompleter* m_SearchTaskCompleter;
	QStringListModel* m_AutoFillModel;

	int m_CurrentItemPage = 0;
};
