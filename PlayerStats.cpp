#include "PlayerStats.h"

#include "LevelsMgr.h"
#include "WindowMgr.h"
#include "MenuMgr.h"
#include "SoundMgr.h"
#include "Enemy.h"
#include "MyFPSCamera.h"
extern LevelsMgr *levelsMgr;
extern WindowMgr winMgr;
extern SoundMgr *sndMgr;
extern MenuMgr menu;
extern Enemy e;
extern MyFPSCamera camera;

const float PlayerStats::LIFE_DEC_PER_SEC = 5.0f;

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
		if( life - PlayerStats::LIFE_DEC_PER_SEC > 1 ) {
			life -= PlayerStats::LIFE_DEC_PER_SEC;
		}
		else {
			//vita a zero
			//############ GAME OVER!!!!!!##############

			logFile << "GAME OVER: life = 0" << endl;

			//ferma musica background
			sndMgr->stopBackgroundMusic();

			sndMgr->play("GAMEOVER");

			//non disegna, non gestisce collisioni, non muove camera.
			winMgr.paused = true;


			//mostra scherma game over
			menu.drawGameOver();


			//reset totale nel caso il giocatore inizi un altra partita
			life = MAX_LIFE;
			points = 0;
			collectedGemsNum = collectedMasksNum = 0;

			//rimetto al loro posto gemme e maschere
			levelsMgr->get()->getPickableObjMgr()->enableAllGemsAndMasks();

			//# mask raccolte = 0
			overlay->akus->resetPickedMasksNum();
			
			//rimetto a posto il fantasmino
			e.enableAndResetPos();

			//riposiz. giocatore
			camera.resetCameraPos();

		}

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


	life = PlayerStats::MAX_LIFE;

	points += PlayerStats::POINTS_PER_MASK;

	collectedMasksNum++;

	//aggiorno overlay disegno # maschere
	overlay->maskPicked();


	if (collectedMasksNum >= MAX_MASKS_NUM) {
		//############ WIN!!!!!!##############

		logFile << "WIN! # masks = 3" << endl;

			//ferma musica background
			sndMgr->stopBackgroundMusic();

			sndMgr->play("WIN");


			//non disegna, non gestisce collisioni, non muove camera.
			winMgr.paused = true;


			//mostra scherma game over
			menu.resetShowing();
			menu.drawWin();

			//reset totale nel caso il giocatore inizi un altra partita
			life = MAX_LIFE;
			points = 0;
			collectedGemsNum = collectedMasksNum = 0;

			//rimetto al loro posto gemme e maschere
			levelsMgr->get()->getPickableObjMgr()->enableAllGemsAndMasks();
			
			//rimetto a posto il fantasmino
			e.enableAndResetPos();

			//riposiz. giocatore
			camera.resetCameraPos();

			//# maschere prese = 0
			overlay->akus->resetPickedMasksNum();

		//ho 3 maschere cambio livello
		//if (levelsMgr->getActiveLevelNum() < LevelsMgr::TOTAL_LEVEL_NUM )
		
		//il caricamento del livello succ. non funziona. Crash. Memory leak da qualche parte.
		/*if (levelsMgr->getActiveLevelNum() == 1 && levelsMgr->get()->isLevelLoaded()) {
			//levelsMgr->loadLevel( levelsMgr->getActiveLevelNum()+1 );
		}*/

	}


}				


void PlayerStats::maskRemoved() {

	if (collectedMasksNum <= 0)
		return;

	life -= PlayerStats::LIFE_INC_AMOUNT_PER_GEM;

	points -= PlayerStats::POINTS_PER_MASK;

	collectedMasksNum--;

	overlay->removeMask();

}

//toglie punti e vita quando si scontra con fantasmino
void PlayerStats::enemyColliding() {
	life -= 50 * (PlayerStats::LIFE_INC_AMOUNT_PER_GEM);

	points -= 5 * (PlayerStats::POINTS_PER_GEM);

	collectedGemsNum -= 5;
}