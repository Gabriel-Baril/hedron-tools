#pragma once

#include "BacklogTask.h"
#include <string>
#include <regex>

using BacklogTaskFilterFunc = bool(*)(const BacklogTask&, void* filterData);

struct TaskDescriptionFilter
{
	enum class Type
	{
		EXACT_MATCH,
		PARTIAL_MATCH,
		REGEX_MATCH
	};

	bool caseInsensitive;
	Type type;
	std::string match;
};

struct TaskNameFilter
{
	enum class Type
	{
		EXACT_MATCH,
		PARTIAL_MATCH,
		PATH_REGEX_MATCH
	};

	bool caseInsensitive;
	Type type;
	std::string match;
};

enum class BacklogTaskFilter : uint32_t
{
	DESCRIPTION,
	NAME,
	EFFORT_POINT,
	COMPLETED,
	COUNT
};

constexpr uint32_t BacklogTaskFilter_Value(BacklogTaskFilter filter);
constexpr uint32_t BacklogTaskFilter_BitmapValue(BacklogTaskFilter filter);

void BacklogTask_Filter_Init();
void BacklogTask_Filter_Register(BacklogTaskFilter type, BacklogTaskFilterFunc callback);
bool BacklogTask_FilterBy_Description(const BacklogTask& task, void* filter);
bool BacklogTask_FilterBy_Name(const BacklogTask& task, void* filter);
bool BacklogTask_FilterBy_EffortPoint(const BacklogTask& task, void* filter);
bool BacklogTask_FilterBy_Completed(const BacklogTask& task, void* filter);