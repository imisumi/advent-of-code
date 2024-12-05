#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <ranges>
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
		tokens.emplace_back(str.substr(startPos, delimPos - startPos));
		startPos = delimPos + delimiter.length();
	}
	tokens.emplace_back(str.substr(startPos));
	return tokens;
}

// int main(int argc, char* argv[])
// {
// 	std::vector<std::string> input = getFileInput(argv[1]);

// 	std::map<int, std::vector<int>> graph;

// 	int silver = 0;
// 	int gold = 0;
// 	for (auto& line : input)
// 	{
// 		if (line.empty())
// 		{
// 			break ;
// 		}
// 		// std::cout << line << std::endl;
// 		std::vector<std::string> tokens = split(line, "|");
// 		// std::cout << "Tokens: " << tokens[0] << " " << tokens[1] << std::endl;
// 		graph[std::stoi(tokens[0])].emplace_back(std::stoi(tokens[1]));

// 	}

// 	for (auto& [key, value] : graph)
// 	{
// 		// std::sort(value.begin(), value.end());
// 		std::cout << key << ": ";
// 		for (auto& v : value)
// 		{
// 			std::cout << v << " ";
// 		}
// 		std::cout << std::endl;
// 	}

// 	std::cout << "silver: " << silver << std::endl;
// 	std::cout << "gold: " << gold << std::endl;
// };



int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	std::map<int, std::vector<int>> graph;

	std::vector<std::vector<int>> ordering;

	bool ordering_rules = false;
	for (auto& line : input)
	{
		if (line.empty())
		{
			ordering_rules = true;
			continue;
		}
		if (!ordering_rules)
		{
			std::vector<std::string> tokens = split(line, "|");
			graph[std::stoi(tokens[0])].emplace_back(std::stoi(tokens[1]));
		}
		else
		{
			std::vector<int> temp;
			for (auto& token : split(line, ","))
			{
				temp.emplace_back(std::stoi(token));
			}
			ordering.emplace_back(temp);
		}
	}

	std::vector<std::vector<int>> valid_ordering;
	std::vector<std::vector<int>> invalid_ordering;

	for (const auto& order : ordering)
	{
		int find_count = 0;
		int desired_count = (order.size() * (order.size() - 1)) / 2;

		bool is_valid = true;

		for (size_t i = 0; i < order.size() && is_valid; i++)
		{
			for (size_t j = i + 1; j < order.size(); j++)
			{
				if (std::ranges::find(graph[order[i]], order[j]) != graph[order[i]].end())
				{
					find_count++;
				}
				else
				{
					is_valid = false;
					break;
				}
			}
		}

		if (find_count == desired_count)
		{
			valid_ordering.emplace_back(order);
		}
		else
		{
			invalid_ordering.emplace_back(order);
		}
	}

	for (auto& order : invalid_ordering)
	{
		std::map<int, int> order_map;
		for (auto& o : order)
		{
			order_map[o] = std::count_if(order.begin(), order.end(),
				[&](const auto& oo) {
					return std::ranges::find(graph[o], oo) != graph[o].end();
				});
		}
		std::ranges::sort(order, 
			[&](const auto& a, const auto& b) {
				return order_map[a] < order_map[b];
			});
	}

	int silver = 0;
	int gold = 0;

	for (auto& order : valid_ordering)
	{
		silver += order[order.size() / 2];
	}
	for (auto& order : invalid_ordering)
	{
		gold += order[order.size() / 2];
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};