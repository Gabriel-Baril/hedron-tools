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
#include <QDialog>
#include <QtWidgets/QVBoxLayout>

#include "json.hpp"

#include "BacklogTask.h"

class EditTaskDialog : public QDialog
{
	Q_OBJECT

public:
	EditTaskDialog(QWidget* parent = nullptr);
	~EditTaskDialog();

	void SetBacklogTaskModel(BacklogTask* model);
	void closeEvent(QCloseEvent* event) override;
	void SaveTask();
	inline bool TaskModified() { return m_TaskModifiedCount > 0; }
	inline bool ShouldDelete() { return m_ShouldDelete; }
	inline bool NameChanged() { return m_LastTaskName.compare(TaskNameEdit->text().toStdString()); }
	inline std::string GetCurrentTaskName() { return TaskNameEdit->text().toStdString(); }

	void OnSubmit();
	void OnEditableElementChanged();
	void OnDelete();
private:
	int m_TaskModifiedCount;
	bool m_ShouldDelete;
	std::string m_LastTaskName;

	BacklogTask* m_BacklogTaskModel;

	QVBoxLayout* m_Layout;
	QWidget* CreateTask;
	QVBoxLayout* verticalLayout_2;
	QSplitter* HeaderLayout;
	QLabel* TaskName;
	QLineEdit* TaskNameEdit;
	QComboBox* TaskType;
	QComboBox* TaskPriority;
	QSplitter* splitter_6;
	QWidget* layoutWidget;
	QHBoxLayout* horizontalLayout_3;
	QHBoxLayout* ResolverHorizontalLayout;
	QLabel* ResolverLabel;
	QLineEdit* ResolverEdit;
	QHBoxLayout* ReviewerHorizontalLayout;
	QLabel* ReviewerLabel;
	QLineEdit* ReviewerEdit;
	QWidget* layoutWidget1;
	QHBoxLayout* horizontalLayout_6;
	QHBoxLayout* EffortLayout;
	QLabel* EffortLabel;
	QLineEdit* EffortEdit;
	QHBoxLayout* DueDateLayout;
	QLabel* DueDateLabel;
	QDateEdit* DueDateEdit;
	QSplitter* BodyLayout;
	QLabel* TaskDescriptionLabel;
	QTextEdit* TaskDescriptionArea;
	QHBoxLayout* FooterLayout;
	QPushButton* SubmitButton;
	QCheckBox* CompleteTaskCheckBox;
	QPushButton* DeleteButton;
};