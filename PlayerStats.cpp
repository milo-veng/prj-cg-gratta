#include "PlayerStats.h"

const float PlayerStats::LIFE_DEC_PER_SEC = 0.5f;

PlayerStats::PlayerStats(int SCREEN_W, int SCREEN_H)
{

	overlay = new Overlay(SCREEN_W, SCREEN_H);
	points = 0;
	life = PlayerStats::MAX_LIFE;
}


PlayerStats::~PlayerStats()
{
}


//aggiorna la quantità di vita rimanente ogni secondo che passa, va chiamata ad ogni frame
void PlayerStats::updateLifeAmount(float deltaT) {

	elapsedTime += deltaT;

	//OutputDebugString( (std::to_string(elapsedTime) + "\n" ).c_str() );

	//controlla se è passato un secondo
	if (elapsedTime >= 1.0f) {
		//riduco la vita
		life -= PlayerStats::LIFE_DEC_PER_SEC;

		elapsedTime = 0.0f;
	}

}				


//presa uan gemma -> aggiorna life e points di conseguenza
void PlayerStats::gemCollected() {
	life += PlayerStats::LIFE_INC_AMOUNT_PER_GEM;

	points += PlayerStats::POINTS_PER_GEM;

	collectedGemsNum++;
}								

//presa maschera -> aggiorna lifef e points di conseguenza

void PlayerStats::maskCollected() {

	if (collectedMasksNum >= MAX_MASKS_NUM) {
		
		//ho 3 maschere cambio livello
		//if (levelsMgr->getActiveLevelNum() < LevelsMgr::TOTAL_LEVEL_NUM )
		
		levelsMgr->loadLevel( levelsMgr->getActiveLevelNum()+1 );

	}

	life = PlayerStats::MAX_LIFE;

	points += PlayerStats::POINTS_PER_MASK;

	collectedMasksNum++;

	//aggiorno overlay disegno # maschere
	overlay->maskPicked();

}				


void PlayerStats::maskRemoved() {

	if (collectedMasksNum <= 0)
		return;

	life -= PlayerStats::LIFE_INC_AMOUNT_PER_GEM;

	points -= PlayerStats::POINTS_PER_MASK;

	collectedMasksNum--;

	overlay->removeMask();

}