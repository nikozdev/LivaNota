#ifndef dLivaNota_Hpp
#define dLivaNota_Hpp
//headers
#include <cstdlib>
#include <functional>
//-//template library
#include <initializer_list>
#include <array>
#include <vector>
#include <unordered_map>
//-//strings
#include <string_view>
#include <sstream>
#include "fmt/format.h"
//-//input output
#include <iostream>
#include <fstream>
#include <filesystem>
namespace nLivaNota
{
namespace nFileSystem
{
using namespace std::filesystem;
}//namespace nFileSystem
}//namespace nLivaNota
//content
namespace nLivaNota
{
//aliases
using tPath = nFileSystem::path;
//typedef
struct tLine
{
	std::size_t			 vLineIter;
	std::string_view vLineText;
};
struct tPage
{
	std::size_t				 vPageIter;
	std::string_view	 vPageName;
	std::string_view	 vPageText;
	std::vector<tLine> vLineList;
};
struct tBook
{
	std::string				 vBookText;
	std::vector<tPage> vPageList;
};
//actions
auto fBookSave(tBook &vBook, const tPath &vPath = "/test/.livanota") -> bool;
auto fBookLoad(tBook &vBook, const tPath &vPath = "/test/.livanota") -> bool;
}//namespace nLivaNota
#endif//dLivaNota_Hpp
