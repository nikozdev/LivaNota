#ifndef dLivaNota_Cpp
#define dLivaNota_Cpp
//headers
#include "LivaNota.hpp"
//content
namespace nLivaNota
{
//testing
#if defined(dLivaNota_MakeTest)
//-//typedef
using tTestKey = std::string_view;
using tTestOut = int;
using tTestFun = std::function<tTestOut(void)>;
using tTestTab = std::unordered_map<tTestKey, tTestFun>;
using tTestRef = tTestTab::iterator;
//-//consdef
static const tTestTab vTestTab = {
	{"Hello",
	 []()
	 {
		 fmt::println(stdout, "HelloWorld");
		 return EXIT_SUCCESS;
	 }},
	{"FileSystem",
	 []()
	 {
		 auto vAbsolutePath = nFileSystem::current_path();
		 auto vRelativePath = nFileSystem::relative(vAbsolutePath);
		 fmt::println(stdout, "RelativePath={:s}", vRelativePath.c_str());
		 fmt::println(
			 stdout, "ProjPathFound={:d}", std::filesystem::exists(dLivaNota_ProjPath)
		 );
		 fmt::println(
			 stdout, "DataPathFound={:d}", std::filesystem::exists(dLivaNota_ProjPath)
		 );
		 return EXIT_SUCCESS;
	 }},
};
#endif//ifd(dLivaNota_MakeTest)
}//namespace nLivaNota
//actions
int main(int vArgC, char **vArgV, char **vEnvi)
{
  nLivaNota::nFileSystem::current_path(dLivaNota_ProjPath);
#if defined(dLivaNota_MakeTest)
	if(vArgC == 3 && std::string_view(vArgV[1]) == "test")
	{
		auto vTestKey = std::string_view(vArgV[2]);
		auto vTestRef = nLivaNota::vTestTab.find(vTestKey);
		if(vTestRef == nLivaNota::vTestTab.end())
		{
			fmt::println(stderr, "invalid test key: {}", vTestKey);
			return EXIT_FAILURE;
		}
		else
		{
			fmt::println(stdout, "{}?", vTestKey);
			auto vTestOut = vTestRef->second();
			fmt::println(stdout, "{}!", vTestKey);
			return vTestOut;
		}
	}
	else
#endif//ifd(dLivaNota_MakeTest)
	{
		auto vIter = std::ostream_iterator<char *>(std::cout, "\n");
		std::copy(vArgV, vArgV + vArgC, vIter);
	}
	return EXIT_SUCCESS;
}
#endif//dLivaNota_Cpp
