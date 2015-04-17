#include <random>
#include <limits>
using namespace std;

std::random_device rd;
std::mt19937 mt(rd());



//valori default [0; infinity]
float getRandomFloat(float min = 0.0, float max = numeric_limits<float>::max() ) {
	//std::uniform_real_distribution<> dist(min, max);
	std::uniform_int_distribution<> dist((int)min, (int)max);

	//std::exponential_distribution<float> dist(1);
	//std::weibull_distribution<> dist;
	//return dist(mt)*100.0f;


	//queste distribuz. fanno valori troppo vicini tra loro

	return dist(mt);
}