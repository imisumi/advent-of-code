
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <set>
#include <list>
#include <map>

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

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	std::map<uint64_t, uint64_t> map;
	for (auto& s : split(input[0], " "))
	{
		map[std::stoull(s)] = 1;
	}

	uint64_t silver = 0;
	uint64_t gold = 0;

	for (int blink = 0; blink < 75; blink++)
	{
		std::map<uint64_t, uint64_t> temp;
		for (auto& [key, amount] : map)
		{
			if (key == 0)
			{
				temp[1] += amount;
			}
			else
			{
				std::string s = std::to_string(key);
				if (s.length() % 2 == 0)
				{
					uint64_t left = std::stoull(s.substr(0, s.length() / 2));
					uint64_t right = std::stoull(s.substr(s.length() / 2));
					temp[left] += amount;
					temp[right] += amount;
				}
				else
				{
					temp[key * 2024] += amount;
				}
			}
		}
		map = std::move(temp);
		if (blink == 24)
		{
			std::ranges::for_each(map, [&silver](auto& pair) { silver += pair.second; });
		}
	}

	std::ranges::for_each(map, [&gold](auto& pair) { gold += pair.second; });

	std::cout << "Silver: " << silver << std::endl;
	std::cout << "Gold: " << gold << std::endl;
	return 0;
};
