
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <ranges>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <iomanip> // For std::setw

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

std::vector<std::vector<char>> populateMap(const std::vector<std::string>& input)
{
	std::vector<std::vector<char>> map;
	for (auto& s : input)
	{
		std::vector<char> row;
		row.reserve(s.size());
		for (auto c : s)
		{
			row.emplace_back(c);
		}
		map.emplace_back(std::move(row));
	}
	return map;
}

std::pair<std::pair<int, int>, std::pair<int, int>> findStartEnd(const std::vector<std::vector<char>>& map)
{
	std::pair<int, int> start;
	std::pair<int, int> end;
	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 'S')
			{
				start = { x, y };
			}
			if (map[y][x] == 'E')
			{
				end = { x, y };
			}
		}
	}
	return { start, end };
}

std::pair<int, int> directions[] = { 
	{0, -1}, // North
	{1, 0},  // East
	{0, 1},  // South
	{-1, 0}  // West
};

std::set<std::tuple<int, int>> visited;

int endCost = std::numeric_limits<int>::max();

void calculateCost(std::vector<std::vector<char>>& map, std::vector<std::vector<int>>& cost, std::pair<int, int> start, std::pair<int, int> end, int lastCost, int direction)
{
	auto [x, y] = start;

	if (lastCost > endCost || visited.find({ x, y }) != visited.end() && (lastCost - 1001) > cost[y][x])
	{
		return;
	}
	visited.insert({ x, y });

	if (lastCost > 0 && lastCost < cost[y][x])
	{
		cost[y][x] = lastCost;
	}

	if (start == end)
	{
		if (lastCost < endCost)
		{
			endCost = lastCost;
		}
		return;
	}

	{
		// forward
		auto [dx, dy] = directions[direction];
		if (map[y + dy][x + dx] != '#')
		{
			calculateCost(map, cost, { x + dx, y + dy}, end, lastCost + 1, direction);
		}
	}
	// left
	{
		auto [dx, dy] = directions[(direction + 3) % 4];
		if (map[y + dy][x + dx] != '#')
		{
			calculateCost(map, cost, { x, y }, end, lastCost + 1000, (direction + 3) % 4);
		}
	}
	// right
	{
		auto [dx, dy] = directions[(direction + 1) % 4];
		if (map[y + dy][x + dx] != '#')
		{
			calculateCost(map, cost, { x, y }, end, lastCost + 1000, (direction + 1) % 4);
		}
	}
}

void partTwo(std::vector<std::vector<char>>& copy, std::vector<std::vector<int>>& cost, std::pair<int, int> start, std::pair<int, int> end, int skip = 1)
{
	if (end == start)
	{
		return;
	}
	auto [x, y] = end;
	for (int i = 0; i < 4; i++)
	{
		auto [dx, dy] = directions[i];
		if (cost[y + dy][x + dx] == cost[y][x] - 1)
		{
			copy[y + dy][x + dx] = 'O';
			partTwo(copy, cost, start, { x + dx, y + dy });
		}
		if (cost[y + dy][x + dx] == cost[y][x] - 1001)
		{
			if (skip == 1)
			{
				cost[y + dy][x + dx] = cost[y][x] -1;
				copy[y + dy][x + dx] = 'O';
				partTwo(copy, cost, start, { x + dx, y + dy } , 0);
			}
			else
			{
				copy[y + dy][x + dx] = 'O';
				partTwo(copy, cost, start, { x + dx, y + dy });
			}
		}
	}
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	std::vector<std::vector<char>> map = populateMap(input);

	std::vector<std::vector<int>> cost(map.size(), std::vector<int>(map[0].size(), std::numeric_limits<int>::max()));

	auto [start, end] = findStartEnd(map);

	calculateCost(map, cost, start, end, 0, 1);

	partTwo(map, cost, start, end);

	uint64_t silver = cost[end.second][end.first];
	uint64_t gold = 0;

	for (auto& s : map)
	{
		for (auto c : s)
		{
			if (c == 'O' || c == 'E' || c == 'S')
			{
				gold++;
			}
		}
	}

	std::cout << "Silver: " << silver << std::endl;
	std::cout << "Gold: " << gold << std::endl;
	return 0;
};
