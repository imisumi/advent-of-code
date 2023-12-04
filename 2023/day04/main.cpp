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
	{
		std::cerr << "Unable to open file: " << filename << std::endl;
	}

	return lines;
}

std::vector<std::string> splitString(const std::string &s, const std::string &delimiters)
{
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;

	while ((end = s.find_first_of(delimiters, start)) != std::string::npos)
	{
		if (end != start)
			tokens.push_back(s.substr(start, end - start));
		start = end + 1;
	}
	if (start < s.length())
		tokens.push_back(s.substr(start));
	return tokens;
}

int total_2 = 0;
int part_one = 0;
int part_two = 0;

void	recc(int& part_two, const std::vector<int>& numbers, int index, int size)
{
	for (int i = index; i < index + size; i++)
	{
		total_2++;
		recc(part_two, numbers, i + 1, numbers[i]);
	}
	return ;
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "give input file as argument" << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<std::string> fileLines = readFileLines(argv[1]);

	std::vector<int> found_numbers;

	for (int i = 0; i < fileLines.size(); i++)
	{
		std::vector<int> numbers;
		std::vector<int> numbers2;
		int j = 0;
		while (!std::isdigit(fileLines[i][j]))
			j++;
		while (std::isdigit(fileLines[i][j]))
			j++;
		int use_num2 = 0;
		while (j < fileLines[i].length())
		{
			if (fileLines[i][j] == '|')
				use_num2 = 1;
			if (std::isdigit(fileLines[i][j]))
			{
				int num = 0;
				while (std::isdigit(fileLines[i][j]))
				{
					num = num * 10 + (fileLines[i][j] - '0');
					j++;
				}
				if (use_num2 == 1)
					numbers2.push_back(num);
				else
					numbers.push_back(num);
			}
			j++;
		}
		int found = 0;
		for (int k = 0; k < numbers2.size(); k++)
		{
			for (int l = 0; l < numbers.size(); l++)
			{
				if (numbers2[k] == numbers[l])
					found++;
			}
		}
		found_numbers.push_back(found);
		if (found > 0)
		{
			int multiply = 1;
			for (int k = 0; k < found - 1; k++)
				multiply *= 2;
			part_one += multiply;
		}
	}
	for (int i = 0; i < found_numbers.size(); i++)
	{
		total_2++;
		if (found_numbers[i] == 0)
			continue;
		recc(part_two, found_numbers, i + 1, found_numbers[i]);
	}
	std::cout << "part 1 = " << part_one << std::endl;
	std::cout << "part 2 = " << total_2 << std::endl;
	return EXIT_SUCCESS;
}
