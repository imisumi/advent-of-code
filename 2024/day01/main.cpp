#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

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

	std::vector<int> one;
	std::vector<int> two;

	for (auto& line : input)
	{
		one.emplace_back(std::stoi(line.substr(0, line.find_first_of(" "))));
		two.emplace_back(std::stoi(line.substr(line.find_last_of(" "))));
	}

	std::sort(one.begin(), one.end());
	std::sort(two.begin(), two.end());

	int silver = 0;
	int gold = 0;
	for (int i = 0; i < one.size(); i++)
	{
		gold += one[i] * std::count(two.begin(), two.end(), one[i]);
		silver += std::abs(one[i] - two[i]);
	}

	std::cout << silver << std::endl;
	std::cout << gold << std::endl;
};