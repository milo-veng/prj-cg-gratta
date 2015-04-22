#pragma once

#include <cstring>
#include "Overlay.h"

class PlayerStats
{

	float life;				//1 - 100
	int points;

	int collectedGemsNum;
	int collectedMasksNum;

	float elapsedTime;

	Overlay *overlay;
	int SCREEN_W, SCREEN_H;
	


public:

	static const int LIFE_INC_AMOUNT_PER_GEM = 1;		//di quanto aumenta la vita quando prendo una gemma
	static const float LIFE_DEC_PER_SEC;						//di quanto diminuisce la vita ogni secondo che passa
	
	static const int POINTS_PER_GEM = 10;				//# di punti per gemma raccolta
	static const int POINTS_PER_MASK = 100;				//# di punti per mask raccolta



	float getLife() { return life;  }
	int getPoints() { return points; }
	int getCollectedGemsNum() { return collectedGemsNum; }
	int getCollectedMasksNum() { return collectedMasksNum; }


	void updateLifeAmount(float deltaT);				//aggiorna la quantità di vita rimanente, va chiamata ad ogni frame
	
	void gemCollected();								//presa uan gemma -> aggiorna life e points di conseguenza

	void maskCollected();								//presa maschera -> aggiorna lifef e points di conseguenza

	void drawLifeBar() { overlay->drawLife(life); }

	PlayerStats(int SCREEN_W, int SCREEN_H);
	~PlayerStats();
};

