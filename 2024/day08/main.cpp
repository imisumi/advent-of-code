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

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	std::unordered_map<char, std::vector<std::pair<int, int>>> map;

	int y = 0;
	int x = 0;
	for (auto& line : input)
	{
		x = 0;
		for (auto& c : line)
		{
			if (std::isalnum(c))
			{
				map[c].emplace_back(x, y);
			}
			x++;
		}
		y++;
	}

	std::vector<std::vector<char>> silverGrid = std::vector<std::vector<char>>(y, std::vector<char>(x, '.'));
	std::vector<std::vector<char>> goldGrid = std::vector<std::vector<char>>(y, std::vector<char>(x, '.'));

	for (auto& [key, value] : map)
	{
		for (int i = 0; i < value.size(); i++)
		{
			auto [x, y] = value[i];
			for (int i = 0; i < value.size(); i++)
			{
				auto& [x2, y2] = value[i];
				if (x == x2 && y == y2)
				{
					continue;
				}
				auto [next_x, next_y] = value[i];
				int diff_x = next_x - x;
				int diff_y = next_y - y;
				try
				{
					silverGrid.at(y - diff_y).at(x - diff_x) = '#';
				}
				catch(const std::exception& e) {}
				try
				{
					int ty = y - diff_y;
					int tx = x - diff_x;
					while (true)
					{
						goldGrid.at(ty).at(tx) = '#';
						ty += diff_y;
						tx += diff_x;
					}
				}
				catch(const std::exception& e) {}
				try
				{
					int ty = y + diff_y;
					int tx = x + diff_x;
					while (true)
					{
						goldGrid.at(ty).at(tx) = '#';
						ty -= diff_y;
						tx -= diff_x;
					}
				}
				catch(const std::exception& e) {}
			}
		}
	}

	uint64_t silver = 0;
	uint64_t gold = 0;

	// count # in silverGrid
	std::ranges::for_each(silverGrid, [&silver](const auto& row)
	{
		silver += std::ranges::count(row, '#');
	});

	std::ranges::for_each(goldGrid, [&gold](const auto& row)
	{
		gold += std::ranges::count(row, '#');
	});

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};

