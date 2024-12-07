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

enum class Direction
{	
	NORTH,
	EAST,
	SOUTH,
	WEST
};

constexpr std::pair<int, int> getOffset(Direction direction)
{
	switch (direction)
	{
		case Direction::NORTH:	return {0, -1};
		case Direction::EAST:	return {1, 0};
		case Direction::SOUTH:	return {0, 1};
		case Direction::WEST:	return {-1, 0};
		default:				return {0, -1};
	}

}

int checkInfinLoop(const std::vector<std::vector<char>>& map, int start_x, int start_y, Direction direction)
{
	std::set<std::tuple<int, int, Direction>> visitedStates;
	int x = start_x;
	int y = start_y;
	auto [fdx, fdy] = getOffset(direction);
	direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
	while (true)
	{
		if (visitedStates.count({x, y, direction}) > 0)
		{
			return 1;
		}

		visitedStates.insert({x, y, direction});

		auto [dx, dy] = getOffset(direction);
		try
		{
			while (map.at(y + dy).at(x + dx) != '#')
			{
				if (x + dx == start_x + fdx && y + dy == start_y + fdy)
				{
					break ;
				}
				x += dx;
				y += dy;
				if (start_x == x && start_y == y && fdx == dx && fdy == dy)
				{
					return 1;
				}
			}
		}
		catch(const std::exception& e)
		{
			return 0;
		}
		
		direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
	}
	return 0;
}

void moveGuard(std::vector<std::vector<char>>& map, int x, int y, Direction direction, std::vector<std::vector<char>>& bonus_check)
{
	auto [dx, dy] = getOffset(direction);

	while (true)
	{
		try
		{
			map[y][x] = 'X';
			int forward_x = x + dx;
			int forward_y = y + dy;
			if (map.at(forward_y).at(forward_x) != '#' && map.at(forward_y).at(forward_x) != 'X')
			{
				if (checkInfinLoop(map, x, y, direction) == 1)
				{
					bonus_check[forward_y][forward_x] = 'O';
				}
			}
			else if (map.at(forward_y).at(forward_x) == '#')
			{
				return moveGuard(map, x, y, static_cast<Direction>((static_cast<int>(direction) + 1) % 4), bonus_check);
			}
			y += dy;
			x += dx;
		}
		catch(const std::exception& e)
		{
			return;
		}
	}
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	std::vector<std::vector<char>> map;

	int start_x = 0;
	int start_y = 0;
	for (auto& line : input)
	{
		std::vector<char> row;
		int x = 0;
		for (auto& c : line)
		{
			if (c == '^')
			{
				row.emplace_back('.');
				start_x = x;
				start_y = map.size();
			}
			else
			{
				row.emplace_back(c);
			}
			x++;
		}
		map.emplace_back(row);
	}

	std::vector<std::vector<char>> gold_obstacles = map;
	gold_obstacles[start_y][start_x] = 'O';

	moveGuard(map, start_x, start_y, Direction::NORTH, gold_obstacles);

	int silver = 0;
	int gold = 0;

	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 'X')
			{
				silver++;
			}
		}
	}

	for (int y = 0; y < gold_obstacles.size(); y++)
	{
		for (int x = 0; x < gold_obstacles[y].size(); x++)
		{
			if (gold_obstacles[y][x] == 'O')
			{
				gold++;
			}
		}
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold - 1 << std::endl; // -1 because because the guards starting position can not be counted as a gold obstacle
};
