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

std::vector<std::string> getSilverExpressions(const std::string& line)
{
	std::vector<std::string> tokens;

	for (size_t i = 0; i < line.size(); i++)
	{
		size_t startPos = line.find("mul(", i);
		size_t endPos = line.find(")", startPos);
		if (startPos == std::string::npos || endPos == std::string::npos)
		{
			break;
		}
		std::string token = line.substr(startPos, endPos - startPos + 1);
		token = token.substr(token.rfind("mul("));
		if (token.size() < 8 || token.size() > 12 || token.find_first_not_of("mul(),0123456789") != std::string::npos)
		{
			continue;
		}
		tokens.emplace_back(token);
		i = endPos;
	}
	return tokens;
}

bool g_MulEnabled = true;

std::vector<std::string> getGoldExpressions(const std::string& line)
{
	std::vector<std::string> tokens;

	for (size_t i = 0; i < line.size(); i++)
	{
		size_t enabledPos = line.find("do()", i);
		size_t disabledPos = line.find("don't()", i);


		size_t startPos = line.find("mul(", i);
		size_t endPos = line.find(")", startPos);
		if (startPos == std::string::npos || endPos == std::string::npos)
		{
			break;
		}
		std::string token = line.substr(startPos, endPos - startPos + 1);
		token = token.substr(token.rfind("mul("));
		if (token.size() < 8 || token.size() > 12 || token.find_first_not_of("mul(),0123456789") != std::string::npos)
		{
			continue;
		}

		if (enabledPos < startPos && enabledPos < endPos)
		{
			g_MulEnabled = true;
			i = enabledPos;
		}
		else if (disabledPos < startPos && disabledPos < endPos)
		{
			g_MulEnabled = false;
			i = disabledPos;
		}
		else if (g_MulEnabled)
		{
			tokens.emplace_back(token);
			i = endPos;
		}
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
		for (const auto& token : getSilverExpressions(line))
		{
			// remove "mul(" and ")"
			std::string expression = token.substr(4, token.size() - 4 - 1);
			std::vector<std::string> parts = split(expression, ",");
			
			silver += std::stoi(parts[0]) * std::stoi(parts[1]);
		}

		for (const auto& token : getGoldExpressions(line))
		{
			// remove "mul(" and ")"
			std::string expression = token.substr(4, token.size() - 4 - 1);
			std::vector<std::string> parts = split(expression, ",");
			
			gold += std::stoi(parts[0]) * std::stoi(parts[1]);
		}
		
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};
