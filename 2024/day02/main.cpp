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


bool isValidRange(std::vector<int>& nums)
{
	int prev = nums[0];

	bool increasing = nums[0] < nums[1];
	for (const auto& num : nums | std::views::drop(1))
	{
		if (increasing && num < prev)
		{
			return false;
		}
		if (!increasing && num > prev)
		{
			return false;
		}
		int diff = std::abs(num - prev);
		if (diff < 1 || diff > 3 || (num == prev))
		{
			return false;
		}
		prev = num;
	}
	return true;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	int silver = 0;
	int gold = 0;
	for (const auto& line : input)
	{
		std::vector<std::string> tokens = split(line, " ");
		std::vector<int> nums;
		for (const auto& token : tokens)
		{
			nums.emplace_back(std::stoi(token));
		}

		if (isValidRange(nums))
		{
			silver++;
			gold++;
		}
		else
		{
			for (int i = 0; i < nums.size(); i++)
			{
				std::vector<int> numsCopy = nums;
				numsCopy.erase(numsCopy.begin() + i);
				if (isValidRange(numsCopy))
				{
					gold++;
					break;
				}
			}
		}
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};
