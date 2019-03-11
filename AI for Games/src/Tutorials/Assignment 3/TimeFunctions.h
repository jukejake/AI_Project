#pragma once
#include <chrono> // high_resolution_clock
#include <utility>


typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template<typename F, typename... Args>
int funcTimeS(F func, Args&&... args) {
	TimeVar t1 = timeNow();
	int res = func(std::forward<Args>(args)...);
	std::cout << "Took[ " << (duration(timeNow() - t1) / 1000000000.0f) << "]" << std::endl;
	return res;
}
template<typename F, typename... Args>
std::pair<int, int> funcTimeD(F func, Args&&... args) {
	TimeVar t1 = timeNow();
	std::pair<int, int> res = func(std::forward<Args>(args)...);
	std::cout << "Took[ " << (duration(timeNow() - t1) / 1000000000.0f) << "]" << std::endl;
	return res;
}
template<typename F, typename... Args>
double funcTime(F func, Args&&... args) {
	TimeVar t1 = timeNow();
	func(std::forward<Args>(args)...);
	return (duration(timeNow() - t1) / 1000000000.0f);
}
