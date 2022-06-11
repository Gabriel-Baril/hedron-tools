#include "BacklogManager.h"
#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QLineEdit>
#include <stdlib.h>

#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <sstream>

// TODO: Maybe use nlohmann::json_pointer<nlohmann::json> ?
nlohmann::json& GetNodeRecursive(const std::vector<std::string>& filter, nlohmann::json& root, int currentIndex = 0)
{
	using namespace nlohmann;
	if (currentIndex == filter.size() - 1)
	{
		return root[filter[currentIndex]];
	}
	return GetNodeRecursive(filter, root[filter[currentIndex]], currentIndex + 1);
}

json_ptr GetJsonPtrFromPath(const std::string& path)
{
	return nlohmann::json_pointer<nlohmann::json::string_t>{ path };
}

nlohmann::json& GetTaskFromPath(nlohmann::json& root, const std::string& path)
{
	json_ptr _path = GetJsonPtrFromPath( path );
	return root[_path];
}

static BacklogTask BacklogTask_FromJson(const nlohmann::json& jsonBacklogTask)
{
	BacklogTask task;
	if (jsonBacklogTask.contains("name")) task.name = jsonBacklogTask["name"];
	if (jsonBacklogTask.contains("description")) task.description = jsonBacklogTask["description"];
	if (jsonBacklogTask.contains("completed")) task.completed = jsonBacklogTask["completed"];
	if (jsonBacklogTask.contains("resolver")) task.resolver = jsonBacklogTask["resolver"];
	if (jsonBacklogTask.contains("reviewer")) task.reviewer = jsonBacklogTask["reviewer"];
	if (jsonBacklogTask.contains("effort_point")) task.effortPoint = jsonBacklogTask["effort_point"];
	if (jsonBacklogTask.contains("type")) task.type = jsonBacklogTask["type"];
	if (jsonBacklogTask.contains("priority")) task.priority = jsonBacklogTask["priority"];
	
	if (jsonBacklogTask.contains("due_date"))
	{
		QString dueDate = QString::fromUtf8(jsonBacklogTask["due_date"].get<std::string>());
		task.dueDate = QDate::fromString(dueDate, "yyyy-MM-dd");
	}

	if (jsonBacklogTask.contains("creation_date"))
	{
		QString creationDate = QString::fromUtf8(jsonBacklogTask["creation_date"].get<std::string>());
		task.creationDate = QDate::fromString(creationDate, "yyyy-MM-dd");
	}

	if (jsonBacklogTask.contains("start_date"))
	{
		QString startDate = QString::fromUtf8(jsonBacklogTask["start_date"].get<std::string>());
		task.startDate = QDate::fromString(startDate, "yyyy-MM-dd");
	}

	return task;
}



static std::string BacklogTask_ToTaskViewString(const BacklogTask& task)
{
	std::string completedString = task.completed ? "✅" : "⬜";

	int lastSlashIndex = task.name.find_last_of('/');
	std::string name = task.name.substr(lastSlashIndex + 1, task.name.size());

	return completedString + " " + name;
}

BacklogManager::BacklogManager(QWidget *parent)
	: QMainWindow(parent), m_EditDialog(this)
{
	ui.setupUi(this);
	ReadBacklog(BACKLOG_FILE);
	CreateActions();

	m_AutoFillModel = new QStringListModel();
	m_AutoFillModel->setStringList({ "Core", "Game" });
	m_SearchTaskCompleter = new QCompleter(m_AutoFillModel, this);
	m_SearchTaskCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	m_SearchTaskCompleter->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);

	ui.SearchTaskField->setCompleter(m_SearchTaskCompleter);

	ui.TaskView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	QObject::connect(ui.TaskView, QOverload<const QPoint&>::of(&QListWidget::customContextMenuRequested), this, &BacklogManager::ShowContextMenu);

	QObject::connect(ui.SearchButton, &QPushButton::clicked, this, &BacklogManager::SearchButtonClicked);
	QObject::connect(ui.CreateTaskButton, &QPushButton::clicked, this, &BacklogManager::CreateTaskCallback);

	QObject::connect(ui.NextPage, &QPushButton::clicked, this, &BacklogManager::NextItemPage);
	QObject::connect(ui.PreviousPage, &QPushButton::clicked, this, &BacklogManager::PreviousItemPage);

	QObject::connect(ui.ShowCompletedCheckbox, &QCheckBox::stateChanged, this, &BacklogManager::ShowCompletedTaskCallback);
	QObject::connect(ui.SearchTaskField, &QLineEdit::returnPressed, this, &BacklogManager::RefreshTaskView);

	QObject::connect(ui.TaskView, &QListWidget::itemDoubleClicked, this, &BacklogManager::OnDoubleClickTaskItem);
	QObject::connect(&m_EditDialog, &QDialog::accepted, this, &BacklogManager::OnEditTaskAccepted);

	RefreshTaskView();
}

BacklogManager::~BacklogManager()
{
	if (m_SearchTaskCompleter)
	{
		delete m_SearchTaskCompleter;
	}

	if (m_AutoFillModel)
	{
		delete m_AutoFillModel;
	}

	delete deleteTaskAction;
}

void BacklogManager::ReadBacklog(const char* filename)
{
	std::ifstream file(filename);
	file >> m_JsonBacklog;
}

void BacklogManager::GetFlatTaskList(const nlohmann::json& rootNode, std::vector<BacklogTask>& outTastList)
{
	if (rootNode.contains("node_type"))
	{
		outTastList.emplace_back(BacklogTask_FromJson(rootNode));
		return;
	}

	for (const auto& value : rootNode.items())
	{
		GetFlatTaskList(rootNode[value.key()], outTastList);
	}
}

void BacklogManager::SearchButtonClicked(bool checked)
{
	RefreshTaskView();
}

void BacklogManager::UpdateTaskFromURIFilter(const std::string& uriFilter)
{
	m_CurrentTaskList.clear();
	if (uriFilter.empty())
	{
		GetFlatTaskList(m_JsonBacklog, m_CurrentTaskList);
	}
	else
	{
		GetFlatTaskList(GetTaskFromPath(m_JsonBacklog, uriFilter), m_CurrentTaskList);
	}
}

void BacklogManager::UpdateTaskView()
{
	ui.TaskView->clear();
	const int offset = m_CurrentItemPage * ITEM_PAGE_COUNT;
	const int loopLimit = (m_CurrentTaskList.size() - offset) < ITEM_PAGE_COUNT ? m_CurrentTaskList.size() % ITEM_PAGE_COUNT : ITEM_PAGE_COUNT;
	for (int i = 0;i < loopLimit; i++)
	{
		BacklogTask& task = m_CurrentTaskList[offset + i];
		if (!(!ui.ShowCompletedCheckbox->isChecked() && task.completed))
		{
			QListWidgetItem* item = new QListWidgetItem;
			item->setData(Qt::UserRole, QVariant(offset + i));
			item->setText(QString::fromUtf8(BacklogTask_ToTaskViewString(task)));
			ui.TaskView->addItem(item);
		}
	}
}

static void NormalizeTaskPath(std::string& path)
{
	if (path.empty()) return;

	std::replace(path.begin(), path.end(), '\\', '/');
	if (path[0] != '/')
	{
		path.insert(0, 1, '/');
	}

	if (path.back() == '/')
	{
		path.pop_back();
	}
}

void BacklogManager::RefreshTaskView()
{
	std::string path = ui.SearchTaskField->text().toStdString();
	NormalizeTaskPath(path);
	ui.SearchTaskField->setText(QString::fromUtf8(path));

	auto jsonPtr = GetJsonPtrFromPath(path);
	if (m_JsonBacklog.contains(jsonPtr))
	{
		UpdateTaskFromURIFilter(path);
		UpdateTaskView();
	}
	else
	{
		QMessageBox::StandardButton resBtn = QMessageBox::information(this, "Backlog Manager - Task View",
			tr("Invalid task category !\n"));
	}
}

void BacklogManager::OnDoubleClickTaskItem(QListWidgetItem* item)
{
	m_CurrentItemModified = item;
	int index = item->data(Qt::UserRole).toInt();
	m_EditDialog.SetBacklogTaskModel(&m_CurrentTaskList[index]);
	m_EditDialog.exec();
}

void BacklogManager::CreateTaskCallback()
{
	if (ui.TaskName->text().isEmpty()) return;
	json_ptr path = nlohmann::json_pointer<nlohmann::json::string_t>{ ui.TaskNameEdit->text().toStdString() };
	CreateTask(path);
}

void BacklogManager::ShowCompletedTaskCallback()
{
	RefreshTaskView();
}

void BacklogManager::OnEditTaskAccepted()
{
	int index = m_CurrentItemModified->data(Qt::UserRole).toInt();
	if (m_EditDialog.ShouldDelete())
	{
		DeleteTask(m_CurrentTaskList[index]);
		RefreshTaskView();
		m_MadeChangeToBacklog = true;
	}
	else if (m_EditDialog.TaskModified())
	{
		if (m_EditDialog.NameChanged())
		{
			RenameTask(m_CurrentTaskList[index], m_EditDialog.GetCurrentTaskName());
		}

		nlohmann::json& root = GetTaskFromPath(m_JsonBacklog, m_CurrentTaskList[index].name);
		root["resolver"] = m_CurrentTaskList[index].resolver;
		root["reviewer"] = m_CurrentTaskList[index].reviewer;
		root["priority"] = m_CurrentTaskList[index].priority;
		root["type"] = m_CurrentTaskList[index].type;
		root["effort_point"] = m_CurrentTaskList[index].effortPoint;
		root["due_date"] = m_CurrentTaskList[index].dueDate.toString(Qt::DateFormat::ISODate).toStdString();
		root["description"] = m_CurrentTaskList[index].description;
		root["completed"] = m_CurrentTaskList[index].completed;

		RefreshTaskView();
		m_MadeChangeToBacklog = true;
	}
}
 
void BacklogManager::CreateTask(const json_ptr& path)
{
	std::string taskName = path.back();
	nlohmann::json& taskParentNode = m_JsonBacklog[path.parent_pointer()];
	
	nlohmann::json taskJson;
	taskJson["node_type"] = 1;
	taskJson["name"] = ui.TaskNameEdit->text().toStdString();
	taskJson["type"] = ui.TaskType->currentText().toStdString();
	taskJson["priority"] = ui.TaskPriority->currentText().toStdString();
	taskJson["resolver"] = ui.ResolverEdit->text().toStdString();
	taskJson["reviewer"] = ui.ReviewerEdit->text().toStdString();
	taskJson["description"] = ui.TaskDescriptionArea->toPlainText().toStdString();

	std::string currentDateString = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
	taskJson["creation_date"] = currentDateString;

	if (ui.StartTaskButton->isChecked()) taskJson["start_date"] = currentDateString;
	else taskJson["start_date"] = "0000-00-00";

	taskJson["due_date"] = ui.DueDateEdit->date().toString(Qt::DateFormat::ISODate).toStdString();
	taskJson["effort_point"] = ui.EffortEdit->text().toUInt();

	taskJson["completed"] = false;

	taskParentNode[taskName] = taskJson;
	m_MadeChangeToBacklog = true;

	RefreshTaskView();
}

void BacklogManager::DeleteTask(const BacklogTask& task)
{
	auto taskPtr = GetJsonPtrFromPath(task.name);
	auto parent = taskPtr.parent_pointer();
	m_JsonBacklog[parent].erase(taskPtr.back());
	m_MadeChangeToBacklog = true;
}

void BacklogManager::RenameTask(BacklogTask& task, const std::string& newPath)
{
	auto newTaskPtr = GetJsonPtrFromPath(newPath);
	auto oldTaskPtr = GetJsonPtrFromPath(task.name);
	auto parent = oldTaskPtr.parent_pointer();
	m_JsonBacklog[newTaskPtr] = m_JsonBacklog[oldTaskPtr];
	m_JsonBacklog[parent].erase(oldTaskPtr.back());
	task.name = newPath;
	m_JsonBacklog[newTaskPtr]["name"] = newPath;
}

void BacklogManager::SaveBacklog()
{
	if (!m_MadeChangeToBacklog) return;

	std::ofstream file(BACKLOG_FILE);
	file << std::setw(4) << m_JsonBacklog << std::endl;
}

// https://stackoverflow.com/questions/17480984/qt-how-do-i-handle-the-event-of-the-user-pressing-the-x-close-button
void BacklogManager::closeEvent(QCloseEvent* event)
{
	if (!m_MadeChangeToBacklog) return;

	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Backlog Manager",
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);

	if (resBtn != QMessageBox::Yes)
	{
		event->ignore();
	}
	else
	{
		SaveBacklog();
		event->accept();
	}
}

void BacklogManager::NextItemPage()
{
	if ((m_CurrentItemPage + 1) * ITEM_PAGE_COUNT < m_CurrentTaskList.size())
	{
		m_CurrentItemPage++;
		UpdateTaskView();
	}
}

void BacklogManager::PreviousItemPage()
{
	if (m_CurrentItemPage > 0)
	{
		m_CurrentItemPage--;
		UpdateTaskView();
	}
}

void BacklogManager::ShowContextMenu(const QPoint& pos)
{
	QPoint globalPos = ui.TaskView->mapToGlobal(pos);

	QMenu menu;
	menu.addAction(deleteTaskAction);
	menu.exec(globalPos);
}

void BacklogManager::OnDeleteSelectedTasks()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Backlog Manager/Delete Task",
		tr("Do you want to remove the selected items?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);

	if (resBtn == QMessageBox::Yes)
	{
		for (auto& selection : ui.TaskView->selectedItems())
		{
			qWarning() << "Deleted task: " << selection->text();
			int index = selection->data(Qt::UserRole).toInt();
			DeleteTask(m_CurrentTaskList[index]);
		}
		RefreshTaskView();
	}
}

void BacklogManager::CreateActions()
{
	deleteTaskAction = new QAction("Delete", this);
	deleteTaskAction->setStatusTip("Delete selected tasks");
	QObject::connect(deleteTaskAction, &QAction::triggered, this, &BacklogManager::OnDeleteSelectedTasks);
}
