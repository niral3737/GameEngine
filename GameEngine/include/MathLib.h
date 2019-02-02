#pragma once
#include <vector>

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif // MATHLIBRARY_EXPORTS

namespace nMath
{
	class Functions
	{
	public:
		static MATHLIBRARY_API float average(std::vector<int> values);
		static MATHLIBRARY_API int max(std::vector<int> values);
		static MATHLIBRARY_API int min(std::vector<int> values);
		static MATHLIBRARY_API int sum(std::vector<int> values);
		static MATHLIBRARY_API bool isEven(int value);
		static MATHLIBRARY_API bool isOdd(int value);
	};
}