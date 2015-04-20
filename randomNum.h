#include <random>
#include <limits>
using namespace std;






//valori default [0; infinity]
float getRandomFloat(float min = 0.0, float max = numeric_limits<float>::max() ) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(min, max);


	return dist(mt);
}
