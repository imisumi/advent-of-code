#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> readFileLines(const std::string& filename)
{
	std::vector<std::string> lines;
	std::ifstream file(filename);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
			lines.push_back(line);
		file.close();
	}
	else
		std::cerr << "Unable to open file: " << filename << std::endl;
	return lines;
}

std::vector<uint64_t> stringToUintVectr(const std::string& str)
{
	std::vector<uint64_t> nums;
	for (int i = 0; i < str.size(); i++)
	{
		while (!std::isdigit(str[i]))
			i++;
		uint64_t num = 0;
		while (std::isdigit(str[i]))
		{
			num = num * 10 + str[i] - '0';
			i++;
		}
		nums.push_back(num);
	}
	return nums;
}

uint64_t maxWinRate(uint64_t time, uint64_t distToBeat)
{
	uint64_t wins = 0;
	for (int i = time - 1; i > 0; i--)
	{
		uint64_t dist = i * (time - i);
		wins += (dist > distToBeat) ? 1 : 0;
	}
	return wins;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "give input file as argument" << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<std::string> fileLines = readFileLines(argv[1]);

	std::vector<uint64_t> part_one_times;
	std::vector<uint64_t> part_one_dist;
	part_one_times = stringToUintVectr(fileLines[0]);
	part_one_dist = stringToUintVectr(fileLines[1]);

	//! PART ONE
	uint64_t part_one = 1;
	for (int i = 0; i < part_one_times.size(); i++)
		part_one = part_one * maxWinRate(part_one_times[i], part_one_dist[i]);
	std::cout << "part 1 = " << part_one << std::endl;

	//! PART TWO
	uint64_t part_two_input[2];
	for (int i = 0; i < fileLines.size(); i++)
	{
		std::string str = "";
		for (int j = 0; j < fileLines[i].size(); j++)
		{
			if (std::isdigit(fileLines[i][j]))
				str += fileLines[i][j];
		}
		part_two_input[i] = std::stoull(str);
	}
	std::cout << "part 2 = " << maxWinRate(part_two_input[0], part_two_input[1]) << std::endl;
	return EXIT_SUCCESS;
}
