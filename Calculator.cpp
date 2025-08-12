#include "MainFrame.h"
#include "Calculator.h"


struct ReturnValues
{
	bool boolean;
	int position;
};
template <typename T>
ReturnValues FindElement(std::vector<std::vector<T>> array, T element)
{
	for (int i = 0; i < array.size(); i++)
		for (int j = 0; j < array[i].size(); j++)
			if (element == array[i][j])
				return ReturnValues{ .boolean = true, .position = i };

	return ReturnValues{ .boolean = false, .position = -1 };
}

void BreakUpExpression(std::string expression, std::vector<std::vector<int>>& operations_indeces)
{
	int found_at = 0;
	operations_indeces = { {/*+*/}, {/*-*/}, {/***/}, {/*/*/}, {/*^*/} };

	while ((found_at = expression.find("+", found_at + 1)) != std::string::npos)
		operations_indeces[0].push_back(found_at++);
	found_at = 0;

	while ((found_at = expression.find("-", found_at + 1)) != std::string::npos)
		operations_indeces[1].push_back(found_at++);
	found_at = 0;

	while ((found_at = expression.find("×", found_at + 1)) != std::string::npos)
		operations_indeces[2].push_back(found_at++);
	found_at = 0;

	while ((found_at = expression.find("÷", found_at + 1)) != std::string::npos)
		operations_indeces[3].push_back(found_at++);
	found_at = 0;

	while ((found_at = expression.find("^", found_at + 1)) != std::string::npos)
		operations_indeces[4].push_back(found_at++);

}

void UpdateExpression(std::string& expression, int starting_position, float new_value, bool completed_operation, 
	std::vector<std::vector<int>> operations_indeces)
{

	if (completed_operation)
	{
		int start = starting_position - 2, end = starting_position;
		ReturnValues values = { .boolean = false };
		for (; start >= 0 && !values.boolean; --start)
			values = FindElement(operations_indeces, start);
		if (start > 0)
			start += 2;
		else
			++start;

		values = { .boolean = false };
		for (; end < int(expression.length()) && !values.boolean; ++end)
			values = FindElement(operations_indeces, end);
		end -= 2;

		int area = end - start;
		expression.erase(expression.begin() + start, expression.begin() + start + area + 1);
		expression.insert(start, std::to_string(new_value));
	}
	else
	{
		int i = starting_position;
		ReturnValues values = { .boolean = false };
		for (; !values.boolean && i < int(expression.length()); ++i)
			values = FindElement(operations_indeces, i);
		--i;

		int size = i - starting_position;
		expression.erase(expression.begin() + starting_position, expression.begin() + starting_position + size);
		expression.insert(starting_position, std::to_string(new_value));
	}

}

void CalculateBlock(std::string& expression, std::vector<float>& numbers, std::vector<std::vector<int>>& operations_indeces, int operation)
{
	int i = operations_indeces[operation][0] - 1, start = i, size;
	ReturnValues values = { .boolean = false };
	for (; i >= 0 && !values.boolean; --i)
		values = FindElement(operations_indeces, i);
	if (i)
		++i;
	size = abs(i - start);

	switch (operation)
	{
	case 4:
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1] =
			pow(numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1], numbers[(operations_indeces[operation][0] + 1) / (size + 1)]);
		break;
	case 2:
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1] *= numbers[(operations_indeces[operation][0] + 1) / (size + 1)];
		break;
	case 3:
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1] /= numbers[(operations_indeces[operation][0] + 1) / (size + 1)];
		break;
	case 0:
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1] += numbers[(operations_indeces[operation][0] + 1) / (size + 1)];
		break;
	case 1:
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1] -= numbers[(operations_indeces[operation][0] + 1) / (size + 1)];
		break;
	}
	numbers.erase(numbers.begin() + (operations_indeces[operation][0] + 1) / (size + 1));
	UpdateExpression(expression, operations_indeces[operation][0] + 1,
		numbers[(operations_indeces[operation][0] + 1) / (size + 1) - 1], true, operations_indeces);
	BreakUpExpression(expression, operations_indeces);
}

float Calculate(std::string expression)
{
	std::vector<std::vector<int>> operations_indeces = { {/*+*/}, {/*-*/}, {/***/}, {/*/*/}, {/*^*/} };
	BreakUpExpression(expression, operations_indeces);
	std::vector<float> numbers;
	for (int i = 0, j = 0; i < expression.length(); ++i)
	{
		ReturnValues values = FindElement(operations_indeces, i);
		if (values.boolean || i == expression.length() - 1)
		{
			numbers.push_back(std::stof(expression.substr(j, i)));
			j = i + 1;
		}
	}

	for (int i = 0; i < numbers.size(); ++i)
	{
		UpdateExpression(expression, i * (std::to_string(numbers[i]).length() + 1), numbers[i], false, operations_indeces);
		BreakUpExpression(expression, operations_indeces);
	}

	while (operations_indeces[4].size() > 0)
		CalculateBlock(expression, numbers, operations_indeces, 4);
	
	while (operations_indeces[2].size() + operations_indeces[3].size() > 0)
	{
		int i = 0, j = 0;
		ReturnValues values = { .boolean = false };
		for (;!values.boolean && j < expression.length(); ++j)
			values = FindElement({ operations_indeces[2], operations_indeces[3] }, j);

		switch (values.position)
		{
		case 0:
			CalculateBlock(expression, numbers, operations_indeces, 2);
			break;
		case 1:
			CalculateBlock(expression, numbers, operations_indeces, 3);
			break;
		}
	}

	while (operations_indeces[0].size() + operations_indeces[1].size() > 0)
	{
		int i = 0, j = 0;
		ReturnValues values = { .boolean = false };
		for (;!values.boolean && j < expression.length(); ++j)
			values = FindElement({ operations_indeces[0], operations_indeces[1] }, j);

		switch (values.position)
		{
		case 0:
			CalculateBlock(expression, numbers, operations_indeces, 0);
			break;
		case 1:
			CalculateBlock(expression, numbers, operations_indeces, 1);
			break;
		}
	}

	float result = numbers[0];
	return result;
}