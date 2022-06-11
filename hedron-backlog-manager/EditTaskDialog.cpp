#include "EditTaskDialog.h"
#include <QMessageBox>

EditTaskDialog::EditTaskDialog(QWidget* parent)
	: QDialog(parent)
{
	m_Layout = new QVBoxLayout(parent);


	CreateTask = new QWidget();
	CreateTask->setObjectName(QString::fromUtf8("CreateTask"));
	verticalLayout_2 = new QVBoxLayout(CreateTask);
	verticalLayout_2->setSpacing(6);
	verticalLayout_2->setContentsMargins(11, 11, 11, 11);
	verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
	HeaderLayout = new QSplitter(CreateTask);
	HeaderLayout->setObjectName(QString::fromUtf8("HeaderLayout"));
	HeaderLayout->setOrientation(Qt::Horizontal);
	TaskName = new QLabel(HeaderLayout);
	TaskName->setText("Name");
	TaskName->setObjectName(QString::fromUtf8("TaskName"));
	TaskName->setMinimumSize(QSize(32, 0));
	TaskName->setMaximumSize(QSize(32, 16777215));
	HeaderLayout->addWidget(TaskName);
	TaskNameEdit = new QLineEdit(HeaderLayout);
	TaskNameEdit->setObjectName(QString::fromUtf8("TaskNameEdit"));
	TaskNameEdit->setMinimumSize(QSize(500, 0));
	HeaderLayout->addWidget(TaskNameEdit);
	TaskType = new QComboBox(HeaderLayout);
	TaskType->addItem(QString("BLOCKER"));
	TaskType->addItem(QString("BUG"));
	TaskType->addItem(QString("FEATURE"));
	TaskType->setObjectName(QString::fromUtf8("TaskType"));
	TaskType->setMinimumSize(QSize(80, 0));
	TaskType->setMaximumSize(QSize(80, 16777215));
	HeaderLayout->addWidget(TaskType);
	TaskPriority = new QComboBox(HeaderLayout);
	TaskPriority->addItem(QString("VERY LOW"));
	TaskPriority->addItem(QString("LOW"));
	TaskPriority->addItem(QString("MEDIUM"));
	TaskPriority->addItem(QString("HIGH"));
	TaskPriority->addItem(QString("VERY HIGH"));
	TaskPriority->setObjectName(QString::fromUtf8("TaskPriority"));
	TaskPriority->setMinimumSize(QSize(80, 0));
	TaskPriority->setMaximumSize(QSize(80, 16777215));
	HeaderLayout->addWidget(TaskPriority);

	verticalLayout_2->addWidget(HeaderLayout);

	splitter_6 = new QSplitter(CreateTask);
	splitter_6->setObjectName(QString::fromUtf8("splitter_6"));
	splitter_6->setOrientation(Qt::Horizontal);
	layoutWidget = new QWidget(splitter_6);
	layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
	horizontalLayout_3 = new QHBoxLayout(layoutWidget);
	horizontalLayout_3->setSpacing(6);
	horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
	horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
	horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
	ResolverHorizontalLayout = new QHBoxLayout();
	ResolverHorizontalLayout->setSpacing(6);
	ResolverHorizontalLayout->setObjectName(QString::fromUtf8("ResolverHorizontalLayout"));
	ResolverLabel = new QLabel(layoutWidget);
	ResolverLabel->setText("Resolver");
	ResolverLabel->setObjectName(QString::fromUtf8("ResolverLabel"));

	ResolverHorizontalLayout->addWidget(ResolverLabel);

	ResolverEdit = new QLineEdit(layoutWidget);
	ResolverEdit->setObjectName(QString::fromUtf8("ResolverEdit"));

	ResolverHorizontalLayout->addWidget(ResolverEdit);


	horizontalLayout_3->addLayout(ResolverHorizontalLayout);

	ReviewerHorizontalLayout = new QHBoxLayout();
	ReviewerHorizontalLayout->setSpacing(6);
	ReviewerHorizontalLayout->setObjectName(QString::fromUtf8("ReviewerHorizontalLayout"));
	ReviewerLabel = new QLabel(layoutWidget);
	ReviewerLabel->setText("Reviewer");
	ReviewerLabel->setObjectName(QString::fromUtf8("ReviewerLabel"));

	ReviewerHorizontalLayout->addWidget(ReviewerLabel);

	ReviewerEdit = new QLineEdit(layoutWidget);
	ReviewerEdit->setObjectName(QString::fromUtf8("ReviewerEdit"));

	ReviewerHorizontalLayout->addWidget(ReviewerEdit);


	horizontalLayout_3->addLayout(ReviewerHorizontalLayout);

	splitter_6->addWidget(layoutWidget);
	layoutWidget1 = new QWidget(splitter_6);
	layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
	horizontalLayout_6 = new QHBoxLayout(layoutWidget1);
	horizontalLayout_6->setSpacing(6);
	horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
	horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
	horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
	EffortLayout = new QHBoxLayout();
	EffortLayout->setSpacing(6);
	EffortLayout->setObjectName(QString::fromUtf8("EffortLayout"));
	EffortLabel = new QLabel(layoutWidget1);
	EffortLabel->setText("Effort");
	EffortLabel->setObjectName(QString::fromUtf8("EffortLabel"));

	EffortLayout->addWidget(EffortLabel);

	EffortEdit = new QLineEdit(layoutWidget1);
	EffortEdit->setObjectName(QString::fromUtf8("EffortEdit"));

	EffortLayout->addWidget(EffortEdit);


	horizontalLayout_6->addLayout(EffortLayout);

	DueDateLayout = new QHBoxLayout();
	DueDateLayout->setSpacing(6);
	DueDateLayout->setObjectName(QString::fromUtf8("DueDateLayout"));
	DueDateLabel = new QLabel(layoutWidget1);
	DueDateLabel->setText("Due Date");
	DueDateLabel->setObjectName(QString::fromUtf8("DueDateLabel"));

	DueDateLayout->addWidget(DueDateLabel);

	DueDateEdit = new QDateEdit(layoutWidget1);
	DueDateEdit->setObjectName(QString::fromUtf8("DueDateEdit"));

	DueDateLayout->addWidget(DueDateEdit);


	horizontalLayout_6->addLayout(DueDateLayout);

	splitter_6->addWidget(layoutWidget1);

	verticalLayout_2->addWidget(splitter_6);

	BodyLayout = new QSplitter(CreateTask);
	BodyLayout->setObjectName(QString::fromUtf8("BodyLayout"));
	BodyLayout->setOrientation(Qt::Vertical);
	TaskDescriptionLabel = new QLabel(BodyLayout);
	TaskDescriptionLabel->setText("Description");
	TaskDescriptionLabel->setObjectName(QString::fromUtf8("TaskDescriptionLabel"));
	BodyLayout->addWidget(TaskDescriptionLabel);
	TaskDescriptionArea = new QTextEdit(BodyLayout);
	TaskDescriptionArea->setObjectName(QString::fromUtf8("TaskDescriptionArea"));
	BodyLayout->addWidget(TaskDescriptionArea);

	verticalLayout_2->addWidget(BodyLayout);

	FooterLayout = new QHBoxLayout();
	FooterLayout->setSpacing(6);
	FooterLayout->setObjectName(QString::fromUtf8("FooterLayout"));

	DeleteButton = new QPushButton(CreateTask);
	DeleteButton->setText("Delete");
	DeleteButton->setObjectName(QString::fromUtf8("DeleteButton"));
	DeleteButton->setMinimumSize(QSize(100, 0));

	FooterLayout->addWidget(DeleteButton);

	SubmitButton = new QPushButton(CreateTask);
	SubmitButton->setText("Submit");
	SubmitButton->setObjectName(QString::fromUtf8("SubmitButton"));
	SubmitButton->setMinimumSize(QSize(700, 0));

	FooterLayout->addWidget(SubmitButton);

	CompleteTaskCheckBox = new QCheckBox;
	CompleteTaskCheckBox->setText("Completed");
	CompleteTaskCheckBox->setObjectName(QString::fromUtf8("CompleteTaskCheckBox"));

	FooterLayout->addWidget(CompleteTaskCheckBox);

	verticalLayout_2->addLayout(FooterLayout);

	m_Layout->addWidget(CreateTask);
	setLayout(m_Layout);

	QObject::connect(SubmitButton, &QPushButton::clicked, this, &EditTaskDialog::OnSubmit);
	QObject::connect(DeleteButton, &QPushButton::clicked, this, &EditTaskDialog::OnDelete);
	QObject::connect(TaskNameEdit, &QLineEdit::textChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(CompleteTaskCheckBox, &QPushButton::clicked, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(TaskDescriptionArea, &QTextEdit::textChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(ResolverEdit, &QLineEdit::textChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(ReviewerEdit, &QLineEdit::textChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(DueDateEdit, &QDateEdit::dateChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(EffortEdit, &QLineEdit::textChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(TaskType, &QComboBox::currentTextChanged, this, &EditTaskDialog::OnEditableElementChanged);
	QObject::connect(TaskPriority, &QComboBox::currentTextChanged, this, &EditTaskDialog::OnEditableElementChanged);
}

void EditTaskDialog::SetBacklogTaskModel(BacklogTask* model)
{
	m_BacklogTaskModel = model;

	ReviewerEdit->setText(QString::fromUtf8(m_BacklogTaskModel->reviewer));
	ResolverEdit->setText(QString::fromUtf8(m_BacklogTaskModel->resolver));
	EffortEdit->setText(QString::number(m_BacklogTaskModel->effortPoint));
	DueDateEdit->setDate(m_BacklogTaskModel->dueDate);
	TaskType->setCurrentText(QString::fromUtf8(m_BacklogTaskModel->type));
	TaskPriority->setCurrentText(QString::fromUtf8(m_BacklogTaskModel->priority));
	TaskNameEdit->setText(QString::fromUtf8(m_BacklogTaskModel->name));
	TaskDescriptionArea->setText(QString::fromUtf8(m_BacklogTaskModel->description));
	CompleteTaskCheckBox->setChecked(m_BacklogTaskModel->completed);

	m_LastTaskName = m_BacklogTaskModel->name;

	m_ShouldDelete = false;
	m_TaskModifiedCount = 0;
}

void EditTaskDialog::closeEvent(QCloseEvent* event)
{
	if (!TaskModified()) return;

	qWarning() << m_TaskModifiedCount;

	// TODO: Don't copy paste
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Edit Task",
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);

	if (resBtn != QMessageBox::Yes)
	{
		event->ignore();
		reject();
	}
	else
	{
		SaveTask();
		event->accept();
		accept();
	}

}

void EditTaskDialog::SaveTask()
{
	// TODO: We could conditionally save modified element depending on the different signals sent
	m_BacklogTaskModel->reviewer = ReviewerEdit->text().toUtf8();
	m_BacklogTaskModel->resolver = ResolverEdit->text().toUtf8();
	m_BacklogTaskModel->effortPoint = EffortEdit->text().toUInt();
	m_BacklogTaskModel->dueDate = DueDateEdit->date();
	m_BacklogTaskModel->type = TaskType->currentText().toUtf8();
	m_BacklogTaskModel->priority = TaskPriority->currentText().toUtf8();
	m_BacklogTaskModel->description = TaskDescriptionArea->toPlainText().toUtf8();
	m_BacklogTaskModel->completed = CompleteTaskCheckBox->isChecked();
}

void EditTaskDialog::OnSubmit()
{
	if (!TaskModified())
	{
		accept();
		return;
	}
	SaveTask();
	accept();
}

void EditTaskDialog::OnEditableElementChanged()
{
	m_TaskModifiedCount++;
}

void EditTaskDialog::OnDelete()
{
	m_ShouldDelete = true;
	accept();
}

EditTaskDialog::~EditTaskDialog()
{
	delete m_Layout;
	delete CreateTask;
}
