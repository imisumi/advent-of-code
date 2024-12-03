#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <ranges>

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
		tokens.emplace_back(str.substr(startPos, delimPos - startPos));
		startPos = delimPos + delimiter.length();
	}
	tokens.emplace_back(str.substr(startPos));
	return tokens;
}

bool stringContainsAllowedChars(const std::string& str, const std::string& allowedChars)
{
	return str.find_first_not_of(allowedChars) == std::string::npos;
}

std::vector<std::string> getExpressions(std::string& line)
{
	std::vector<std::string> tokens;
	// size_t startPos = 0;

	for (size_t i = 0; i < line.size(); i++)
	{
		// size_t startPos = line.find_first_of("mul(", i);
		size_t startPos = line.find("mul(", i);
		size_t endPos = line.find_first_of(")", startPos);
		if (startPos == std::string::npos || endPos == std::string::npos)
		{
			break;
		}
		std::string token = line.substr(startPos, endPos - startPos + 1);
		size_t mulPos = token.find_last_of("mul(");
		token = token.substr(mulPos);
		if (token.size() < 5 || token.size() > 9)
		{
			continue;
		}
		if (token.find_first_not_of("mul(),0123456789") != std::string::npos)
		{
			continue;
		}
		tokens.emplace_back(token);
		i = endPos;
	}
	return tokens;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	int silver = 0;
	int gold = 0;
	for (auto& line : input)
	{
		std::vector<std::string> tokens = getExpressions(line);
		for (const auto& token : tokens)
		{
			// remove brackets
			std::string expression = token.substr(1, token.size() - 2);
			std::cout << expression << std::endl;
			std::vector<std::string> parts = split(expression, ",");
			int num1 = std::stoi(parts[0]);
			int num2 = std::stoi(parts[1]);
			silver += num1 * num2;
			// std::cout << expression << std::endl;
		}
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};
