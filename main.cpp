#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

const std::string defCodeTemplate = "#include <iostream>\n"
"\n"
"int main (int argc, char *argv[])\n"
"{\n"
"   std::cout << \"I personally think Cefault++ is a good name\" << std::endl;\n"
"}\n\0";

std::string GetDefCmakeTemplate(const std::string projName, const std::string cmakeVers)
{
    return "cmake_minimum_required(VERSION " + cmakeVers + ")\n"
           "project(" + projName + ")\n"
           "\n"
           "file(GLOB SOURCES ./src/*.cpp)\n"
           "add_executable(main ${SOURCES})\n\0";
}

void GiveError(std::string desc)
{
    std::cerr << "an error occured: " << desc << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    // Step 0: Get the options
    std::string projName; std::string cmakeVers;
    std::cout << "Enter your project name: "; std::getline(std::cin, projName);
    std::cout << "Enter your CMake version: "; std::getline(std::cin, cmakeVers);

    // Step 1: Create the directories
    const fs::path srcPth = fs::current_path().string() + "/src";
    const fs::path incPth = fs::current_path().string() + "/include";
    const fs::path buildPth = fs::current_path().string() + "/build";
    if(!fs::create_directory(srcPth)) GiveError("Could not create the src dir, make sure it does not already exist");
    if(!fs::create_directory(incPth)) GiveError("Could not create the include dir, make sure it does not already exist");
    if(!fs::create_directory(buildPth)) GiveError("Could not create the build dir, make sure it does not already exist");

    // Step 2: Create main.cpp and CMakeLists.txt
    std::ofstream mainFl(fs::current_path().string() + "/src/main.cpp");
    if(!mainFl.is_open()) GiveError("Could not create/write main.cpp");
    mainFl << defCodeTemplate;
    mainFl.close();

    std::ofstream cmakeFl(fs::current_path().string() + "/CMakeLists.txt");
    if(!cmakeFl.is_open()) GiveError("Could not create/write CMakeLists.txt");
    cmakeFl << GetDefCmakeTemplate(projName, cmakeVers);
    cmakeFl.close(); // BE SURE TO CLOSE THIS, it stays empty and not written till you close it, so "cmake .." will see it as empty 

    // Step 3: Build the default project if it is instructed to
    if(!(argc > 1 && static_cast<std::string>(argv[1]).compare("no-build") == 0)){
        if(system("cd ./build && cmake .. && cmake --build . && cd ..") != 0)
            GiveError("Could not finish the building process");
    }

    // Step 4: Success!
    std::cout << "Successfully created a default C++ project" << std::endl;

    return 0;
}
