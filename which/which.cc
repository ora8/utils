#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

std::vector<std::string> splitPath(const std::string &path)
{
	std::vector<std::string> paths;
	std::stringstream ss(path);
	std::string item;

	while (std::getline(ss, item, ';'))
	{
		paths.push_back(item);
	}

	return paths;
}

std::string findExecutable(const std::string &command)
{
	const char *pathEnv = std::getenv("PATH");
	if (!pathEnv)
	{
		return ""; // PATH not set
	}

	std::vector<std::string> paths = splitPath(pathEnv);

	for (const auto &dir : paths)
	{
		std::filesystem::path fullPath = std::filesystem::path(dir) / command;
		if (fullPath.extension().empty())
		{
			fullPath += ".exe";
		}
		if (std::filesystem::exists(fullPath) && std::filesystem::is_regular_file(fullPath) &&
			((std::filesystem::status(fullPath).permissions() & std::filesystem::perms::owner_exec) ==
			 std::filesystem::perms::owner_exec))
		{
			return fullPath.string();
		}
	}

	return ""; // Not found
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
		return 1;
	}

	std::string command = argv[1];
	std::string result = findExecutable(command);

	if (!result.empty())
	{
		std::cout << result << std::endl;
	}
	else
	{
		std::cout << "Command not found: " << command << std::endl;
	}

	return 0;
}