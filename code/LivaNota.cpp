#ifndef dLivaNota_Cpp
#define dLivaNota_Cpp
//headers
#include "LivaNota.hpp"
//content
//actions
int main(int vArgC, char **vArgV, char **vEnvi)
{
  auto vStreamIter = std::ostream_iterator<char *>(std::cout, "\n");
	std::copy(vArgV, vArgV + vArgC, vStreamIter);
}//main
#endif//dLivaNota_Cpp
