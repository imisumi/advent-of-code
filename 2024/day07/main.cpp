#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <set>

std::vector<std::string> getFileInput(const std::filesystem::path& path)
{
	std::vector<std::string> input;
	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{
		input.emplace_back(std::move(line));
	}
	return input;
};
std::vector<std::string> split(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> tokens;
	size_t startPos = 0;
	size_t delimPos;

	while ((delimPos = str.find(delimiter, startPos)) != std::string::npos)
	{
		if (delimPos != startPos)
		{
			tokens.emplace_back(str.substr(startPos, delimPos - startPos));
		}
		startPos = delimPos + delimiter.length();
	}

	if (startPos < str.size())
	{
		tokens.emplace_back(str.substr(startPos));
	}
	return tokens;
}

void generateCombinations(std::vector<std::vector<char>>& result, std::vector<char>& current, const std::vector<char>& symbols, int n)
{
	if (current.size() == n)
	{
		result.push_back(current);
		return; 
	}

	for (char symbol : symbols)
	{
		current.push_back(symbol);
		generateCombinations(result, current, symbols, n); 
		current.pop_back(); 
	}
}

uint64_t checkGoldCalculation(uint64_t desiredNumber, const std::vector<uint64_t>& numbers)
{
	std::vector<std::vector<char>> order;
	std::vector<char> current;
	generateCombinations(order, current, {'+', '*', '|'}, numbers.size() - 1);
	for (const auto& row : order)
	{
		uint64_t sum = numbers[0];

		for (size_t i = 1; i < numbers.size(); i++)
		{
			if (row[i - 1] == '+')
			{
				sum += numbers[i];
			}
			else if (row[i - 1] == '*')
			{
				sum *= numbers[i];
			}
			else if (row[i - 1] == '|')
			{
				sum = std::stoull(std::to_string(sum) + std::to_string(numbers[i]));
			}
			if (sum > desiredNumber)
			{
				break ;
			}
		}
		if (sum == desiredNumber)
		{
			return sum;
		}
	}
	return 0;
}

uint64_t checkSilverCalculation(uint64_t desiredNumber, const std::vector<uint64_t>& numbers)
{
	std::vector<std::vector<char>> order;
	std::vector<char> current;
	generateCombinations(order, current, {'+', '*'},  numbers.size() - 1);

	for (const auto& row : order)
	{
		uint64_t sum = numbers[0];

		for (size_t i = 1; i < numbers.size(); i++)
		{
			if (row[i - 1] == '+')
			{
				sum += numbers[i];
			}
			else if (row[i - 1] == '*')
			{
				sum *= numbers[i];
			}
			if (sum > desiredNumber)
			{
				break ;
			}
		}
		if (sum == desiredNumber)
		{
			return sum;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	uint64_t silver = 0;
	uint64_t gold = 0;

	for (auto& line : input)
	{
		std::vector<std::string> tokens = split(line, ":");
		uint64_t desiredNumber = std::stoull(tokens[0]);

		std::vector<uint64_t> numbers;
		for (auto& number : split(tokens[1], " "))
		{
			numbers.emplace_back(std::stoull(number));
		}

		silver += checkSilverCalculation(desiredNumber, numbers);
		gold += checkGoldCalculation(desiredNumber, numbers);
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};
