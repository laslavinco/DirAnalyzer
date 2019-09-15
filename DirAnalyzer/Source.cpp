#include <iostream>
#include <Windows.h>
#include <vector>
#include <map>
#include <experimental/filesystem>
#include <exception>


namespace fs = std::experimental::filesystem;

class Analyse{

	std::map<double, std::string> dict;

public:
	const char* dir_path;
	int count;
	std::string search_ext = "";

public:

	Analyse(const char *path, int fileCount=0, char* extension=nullptr)
	{
		dir_path = path;
		count = fileCount;
		if (extension != nullptr)
			search_ext = extension;
	}

	void ScanDir(const char* directory)
	{
		for (auto& p : fs::recursive_directory_iterator(directory, fs::directory_options::follow_directory_symlink))
		{
			std::string dirpath;
			try {
				
				dirpath = p.path().string();
			}
			catch (std::exception e) {
				std::cout << "[! WARNING: ]" << std::endl << e.what() << " " << std::endl;
			}
			if (dirpath.length() > 0)
				if (fs::is_regular_file(dirpath))
					dict.insert(std::pair<double, std::string> (CalculateFileSize(dirpath), dirpath));
		}
		SortData();
		
	}

private:
	double CalculateFileSize(std::string filePath)
	{
		return fs::file_size(filePath) / 1024.0f / 1024.0f;
	}

	void SortData()
	{

		std::map <double, std::string>::reverse_iterator it;
		std::cout << (--dict.end())->first << " " << (--dict.end())->second << std::endl;
		int i = 0;
		if (count == 0)
			count = dict.size();

		for (it = dict.rbegin(); it != dict.rend(); it++)
		{
			if (i < count)
			{
				if (search_ext != "")
				{
					if (fs::path(it->second).extension() == search_ext)
						std::cout << it->first << " " << it->second << std::endl;
				}
				else
				{
					++i;
					std::cout << it->first << " " << it->second << std::endl;
				}
			}
			else
				break;
		}
	}

};


void DisplayHelp()
{
		std::cout << "\nUsage:\n"
		<< "\t-h or --help for help/usage\n"
		<< "\t-p or --path to the folder to scan (RECOMMENDED)\n"
		<< "\t-c or --count to show only those many largets files (OPTIONAL)\n"
		<< "\t-e or --ext to filter specific extension file type (OPTIONAL)\n"
		<< "\t-o or --output to export the data in a text file"
		<< std::endl;
}

void DisplayError()
{
	std::cout << "\nInvalid Arguments Passed\n" << std::endl;
	DisplayHelp();

}

int main(int argc, char **argv)
{

	std::vector <std::string> arguments;

	arguments.push_back(std::string("-h"));
	arguments.push_back(std::string("-p"));
	arguments.push_back(std::string("--path"));
	arguments.push_back(std::string("-c"));
	arguments.push_back(std::string("--count"));
	arguments.push_back(std::string("-e"));
	arguments.push_back(std::string("--ext"));
	arguments.push_back(std::string("-o"));
	arguments.push_back(std::string("-output"));
	arguments.push_back(std::string("-help")); //Keeping help and h seperate to just fix the indexing issues

	

	if (argc < 3)
	{
		DisplayError();
		return 1;
	}

	char* dir_path = nullptr;
	unsigned int number_of_files_to_show = 0;
	char * extension_filter = nullptr;
	char* output_path = nullptr;


	for (int i = 0; i < argc; i++)
	{

		if (argv[i] == std::string("-h") || argv[i] == std::string("--help"))
		{
			DisplayHelp();
			return 0;
		}

		if (argv[i] == std::string("-p") || argv[i] == std::string("--path"))
		{
			dir_path = argv[i+1];
		}

		if (argv[i] == std::string("-c") || argv[i] == std::string("--count"))
		{
			number_of_files_to_show = std::stoi(argv[i+1]);
		}

		if (argv[i] == std::string("-e") || argv[i] == std::string("--ext"))
		{
			extension_filter = argv[i+1];
		}

		if (argv[i] == std::string("-o") || argv[i] == std::string("--output"))
		{
			output_path = argv[i+1];
		}
	}

	if (dir_path != nullptr)
	{
		Analyse a(dir_path, number_of_files_to_show, extension_filter);
		a.ScanDir(a.dir_path);
	}
	else
	{
		DisplayError();
		return 1;
	}
	
	system("pause");
	return 0;
}