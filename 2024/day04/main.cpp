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

int extractXmas(std::vector<std::vector<char>>& map, int x, int y, int dx, int dy)
{
	std::string str;

	try
	{
		for (int i = 0; i < 4; i++)
		{
			str += map.at(x).at(y);
			x += dx;
			y += dy;
		}
	}
	catch(const std::exception& e){}
	return str == "XMAS" ? 1 : 0;
}
int extractXmases(std::vector<std::vector<char>>& map, int x, int y)
{
	try
	{
		if (map.at(x - 1).at(y - 1) == 'M' && map.at(x + 1).at(y + 1) == 'S' || 
			map.at(x - 1).at(y - 1) == 'S' && map.at(x + 1).at(y + 1) == 'M')
		{
			if (map.at(x + 1).at(y - 1) == 'M' && map.at(x - 1).at(y + 1) == 'S' ||
				(map.at(x + 1).at(y - 1) == 'S' && map.at(x - 1).at(y + 1) == 'M'))
			{
				return 1;
			}
		}
	}
	catch(const std::exception& e) {}
	return 0;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> input = getFileInput(argv[1]);

	int silver = 0;
	int gold = 0;
	std::vector<std::vector<char>> map;
	for (auto& line : input)
	{
		std::vector<char> row;
		for (auto& c : line)
		{
			row.emplace_back(c);
		}
		map.emplace_back(row);
	}

	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[x].size(); y++)
		{
			if (map[x][y] == 'X')
			{
				// north
				silver += extractXmas(map, x, y, 0, 1);
				// east
				silver += extractXmas(map, x, y, 1, 0);
				// south
				silver += extractXmas(map, x, y, 0, -1);
				// west
				silver += extractXmas(map, x, y, -1, 0);
				// north east
				silver += extractXmas(map, x, y, 1, 1);
				// south east
				silver += extractXmas(map, x, y, 1, -1);
				// south west
				silver += extractXmas(map, x, y, -1, -1);
				// north west
				silver += extractXmas(map, x, y, -1, 1);
			}
			else if (map[x][y] == 'A')
			{
				gold += extractXmases(map, x, y);
			}
		}
	}

	std::cout << "silver: " << silver << std::endl;
	std::cout << "gold: " << gold << std::endl;
};
