#include "backlog_task_filter.h"

BacklogTaskFilterFunc s_task_filters[static_cast<uint32_t>(BacklogTaskFilter::COUNT)];

constexpr uint32_t BacklogTaskFilter_Value(BacklogTaskFilter filter)
{
	return static_cast<uint32_t>(filter);
}

constexpr uint32_t BacklogTaskFilter_BitmapValue(BacklogTaskFilter filter)
{
	return 1 << BacklogTaskFilter_Value(filter);
}

void BacklogTask_Filter_Init()
{
	BacklogTask_Filter_Register(BacklogTaskFilter::DESCRIPTION, BacklogTask_FilterBy_Description);
}

void BacklogTask_Filter_Register(BacklogTaskFilter type, BacklogTaskFilterFunc callback)
{
	s_task_filters[BacklogTaskFilter_Value(type)] = callback;
}

bool BacklogTask_Filter_ByName(const BacklogTask& task, void* filter)
{
	return false;
}

bool BacklogTask_FilterBy_Description(const BacklogTask& task, void* filter)
{
	TaskDescriptionFilter* descriptionFilter = static_cast<TaskDescriptionFilter*>(filter);

	if (descriptionFilter->type == TaskDescriptionFilter::Type::EXACT_MATCH)
	{
		if (descriptionFilter->caseInsensitive)
		{
			std::string descriptionFilterLower = descriptionFilter->match;
			std::string taskDescriptionLower = task.description;
			std::transform(descriptionFilter->match.begin(), descriptionFilter->match.end(), descriptionFilterLower.begin(), ::tolower);
			std::transform(task.description.begin(), task.description.end(), taskDescriptionLower.begin(), ::tolower);
			return taskDescriptionLower.compare(descriptionFilterLower);
		}

		return task.description.compare(descriptionFilter->match);
	}
	else if (descriptionFilter->type == TaskDescriptionFilter::Type::PARTIAL_MATCH)
	{
		if (descriptionFilter->caseInsensitive)
		{
			std::string descriptionFilterLower = descriptionFilter->match;
			std::string taskDescriptionLower = task.description;
			std::transform(descriptionFilter->match.begin(), descriptionFilter->match.end(), descriptionFilterLower.begin(), ::tolower);
			std::transform(task.description.begin(), task.description.end(), taskDescriptionLower.begin(), ::tolower);
			return taskDescriptionLower.find_first_of(descriptionFilterLower);
		}

		return task.description.find_first_of(descriptionFilter->match);
	}
	else if (descriptionFilter->type == TaskDescriptionFilter::Type::REGEX_MATCH)
	{
		return regex_match(task.description, std::regex(descriptionFilter->match));
	}

	return false;
}

static std::string String_ToLower_Copy(const std::string& str)
{
	std::string copy;
	copy.reserve(str.size());
	std::transform(str.begin(), str.end(), copy.begin(), ::tolower);
	return copy;
}

bool BacklogTask_FilterBy_Name(const BacklogTask& task, void* filter)
{
	TaskNameFilter* nameFilter = static_cast<TaskNameFilter*>(filter);

	if (nameFilter->type == TaskNameFilter::Type::EXACT_MATCH)
	{
		if (nameFilter->caseInsensitive)
		{
			return String_ToLower_Copy(task.name).compare(String_ToLower_Copy(nameFilter->match));
		}

		return task.name.compare(nameFilter->match);
	}
	else if (nameFilter->type == TaskNameFilter::Type::PARTIAL_MATCH)
	{
		if (nameFilter->caseInsensitive)
		{
			return String_ToLower_Copy(task.name).find_first_of(String_ToLower_Copy(nameFilter->match));
		}

		return task.description.find_first_of(nameFilter->match);
	}

	return false;
}


