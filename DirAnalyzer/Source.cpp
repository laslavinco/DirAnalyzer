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
				std::cout << "[! WARNING: ]" << std::endl;
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

	

	if (argc < 2)
	{
		DisplayError();
		return 1;
	}
	if (argv[1] == arguments[0] || argv[1] == arguments[arguments.size()-1])
	{

		DisplayHelp();
		return 0;
	}
	


	//char* dir_path = argv[2];
	char* dir_path;
	unsigned int number_of_files_to_show = 0;
	char * extension_filter = nullptr;
	char* output_path = nullptr;

	if (argv[1] != "-p")
	{
		char* dir_path = argv[1];
	}
	else
		char* dir_path = argv[2];


	if (argc > 4)
	{
		if (argv[3] == arguments[3] || argv[3] == arguments[4])
			number_of_files_to_show = std::stoi(argv[4]);
		else
		{
			std::cout << "Invalid argument number 2 " << argv[3] << " " << argv[4] << std::endl;
			return 1;
		}
	}

	if (argc > 5)
	{
		if (argv[5] == arguments[5] || argv[5] == arguments[6])
			extension_filter = argv[6];
		else
		{
			std::cout << "Invalid argument number 3 " << argv[4] << " " << argv[5] << std::endl;
			return 1;
		}
	}

	if (argc > 7)
	{
		if (argv[7] == arguments[7] || argv[6] == arguments[8])
			output_path = argv[7];
		else
		{
			std::cout << "Invalid argument number 4 " << argv[6] << " " << argv[7] << std::endl;
			return 1;
		}
	}
	
	Analyse a(dir_path, number_of_files_to_show, extension_filter);
	a.ScanDir(a.dir_path);
	system("pause");
	return 0;
}