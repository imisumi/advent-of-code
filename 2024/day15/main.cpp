
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

std::pair<std::vector<std::string>, std::vector<std::string>> split(const std::vector<std::string>& input, const std::string& delimiter)
{
	std::vector<std::string> part1;
	std::vector<std::string> part2;
	bool isPart1 = true;
	for (auto& s : input)
	{
		if (isPart1)
		{
			if (s.empty())
			{
				isPart1 = false;
				continue;
			}
			part1.emplace_back(s);
		}
		else
		{
			part2.emplace_back(s);
		}
	}
	return { part1, part2 };
}

void moveBlocks(std::vector<std::vector<char>>& grid, std::pair<int, int> pos, std::pair<int, int> dir)
{
	auto [x, y] = pos;
	auto [dx, dy] = dir;

	if (grid[y + dy][x + dx] == '#')
	{
		throw std::exception();
	}

	if (grid[y + dy][x + dx] == 'O')
	{
		moveBlocks(grid, { x + dx, y + dy }, dir);
	}
	else if (grid[y + dy][x + dx] == '[')
	{
		moveBlocks(grid, { x + dx, y + dy }, dir);
		if (dx == 0)
		{
			moveBlocks(grid, { x + dx + 1, y + dy }, dir);
		}
	}
	else if (grid[y + dy][x + dx] == ']')
	{
		moveBlocks(grid, { x + dx, y + dy }, dir);
		if (dx == 0)
		{
			moveBlocks(grid, { x + dx - 1, y + dy }, dir);
		}
	}

	if (grid[y + dy][x + dx] == '.')
	{
		std::swap(grid[y][x], grid[y + dy][x + dx]);
	}
}

std::pair<int, int> findStart(const std::vector<std::vector<char>>& grid, char marker)
{
	for (int y = 0; y < grid.size(); y++)
	{
		for (int x = 0; x < grid[y].size(); x++)
		{
			if (grid[y][x] == marker)
			{
				return {x, y};
			}
		}
	}
	return {-1, -1};
}

uint64_t calculateScore(const std::vector<std::vector<char>>& grid, char target)
{
	uint64_t score = 0;
	for (int y = 0; y < grid.size(); y++)
	{
		for (int x = 0; x < grid[y].size(); x++)
		{
			if (grid[y][x] == target)
			{
				score += (y * 100) + x;
			}
		}
	}
	return score;
}

void populateGrid(std::vector<std::vector<char>>& silver, std::vector<std::vector<char>>& gold, const std::vector<std::string>& map)
{
	silver.reserve(map.size());
	gold.reserve(map.size());
	for (auto& s : map)
	{
		std::vector<char> silverRow;
		std::vector<char> goldRow;
		silverRow.reserve(s.size());
		goldRow.reserve(s.size());
		for (auto c : s)
		{
			silverRow.emplace_back(c);
			switch (c)
			{
			case '#':
				goldRow.emplace_back('#');
				goldRow.emplace_back('#');
				break;
			case '.':
				goldRow.emplace_back('.');
				goldRow.emplace_back('.');
				break;
			case 'O':
				goldRow.emplace_back('[');
				goldRow.emplace_back(']');
				break;
			case '@':
				goldRow.emplace_back('@');
				goldRow.emplace_back('.');
				break;
			}
		}
		silver.emplace_back(std::move(silverRow));
		gold.emplace_back(std::move(goldRow));
	}
}

void moveRobot(std::vector<std::vector<char>>& grid, std::pair<int, int>& pos, std::pair<int, int> dir)
{
	auto copy = grid;

	try
	{
		moveBlocks(grid, pos, dir);
		pos.first += dir.first;
		pos.second += dir.second;
	}
	catch(const std::exception& e)
	{
		grid = copy;
	}
	
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	auto [map, instructions] = split(input, "");

	std::vector<std::vector<char>> silverGrid;
	std::vector<std::vector<char>> goldGrid;

	populateGrid(silverGrid, goldGrid, map);

	std::pair<int, int> silverStart = findStart(silverGrid, '@');
	std::pair<int, int> goldStart = findStart(goldGrid, '@');

	for (auto& s : instructions)
	{
		for (char c : s)
		{
			switch (c)
			{
			case '^':
				moveRobot(silverGrid, silverStart, { 0, -1 });
				moveRobot(goldGrid, goldStart, { 0, -1 });
				break;
			case 'v':
				moveRobot(silverGrid, silverStart, { 0, 1 });
				moveRobot(goldGrid, goldStart, { 0, 1 });
				break;
			case '>':
				moveRobot(silverGrid, silverStart, { 1, 0 });
				moveRobot(goldGrid, goldStart, { 1, 0 });
				break;
			case '<':
				moveRobot(silverGrid, silverStart, { -1, 0 });
				moveRobot(goldGrid, goldStart, { -1, 0 });
				break;
			}
		}
	}

	uint64_t silver = calculateScore(silverGrid, 'O');
	uint64_t gold = calculateScore(goldGrid, '[');

	std::cout << "Silver: " << silver << std::endl;
	std::cout << "Gold: " << gold << std::endl;
	return 0;
};