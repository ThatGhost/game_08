#include "pch.h"
#include "endScreens.h"
#include "irrKlang.h"
using namespace UI;
using namespace utils;
using namespace SLUtils;

extern float g_WindowHeight;
extern float g_WindowWidth;
extern InputS Input;
extern INT8 g_Scene;
extern MainGame g_MainGame;
extern irrklang::ISoundEngine* engine;

#pragma region WinScreen
void WinScreen::Draw() {
	DrawScreen(true, score, percent, HighScore);
}

void WinScreen::Update(float elapsedSec) {

}

#pragma endregion

#pragma region LoseScreen
void LoseScreen::Draw() {
	DrawScreen(false,score,percent);
}

void LoseScreen::Update(float elapsedSec) {

}
#pragma endregion

void DrawScreen(bool won, int& score, float& percent, bool highscore) {
	const Point2f leftBottom{ 425, 150 }, size{780,355};
	const int margin{ 20 };
	DrawTexture(*GetTexture("BackgroundMenu.png"), Point2f(0, 0));

	SetColor(38 / 255.0f, 32 / 255.0f, 60 / 255.0f);
	FillRect(leftBottom,size.x,size.y);

	if (won)FillText("you won",Point2f(leftBottom.x + size.x / 2 - 175, leftBottom.y + size.y - 65), 50,true);
	else FillText("you lost", Point2f(leftBottom.x + size.x / 2 - 190, leftBottom.y + size.y - 65), 50,true);

	const Point2f StatsArea{ leftBottom.x + margin,leftBottom.y + margin }, 
				  StatsSize{900 - StatsArea.x,430 - StatsArea .y};

	const Point2f ScoreArea{ StatsArea.x + StatsSize.x + margin, StatsArea.y}, 
				  ScoreSize{leftBottom.x + size.x - margin - ScoreArea.x, StatsSize.y};

	SetColor(1, 1, 1);
	DrawRect(Rectf(StatsArea.x,StatsArea.y,StatsSize.x,StatsSize.y),3);
	DrawRect(Rectf(ScoreArea.x, ScoreArea.y, ScoreSize.x, ScoreSize.y),3);

	//stats area
	FillText("score: "+std::to_string(score),Point2f(StatsArea.x + margin,StatsArea.y + StatsSize.y - 50),25,true);
	FillText("Tap %: " + std::to_string(int(percent * 100)),
			 Point2f(StatsArea.x + margin,StatsArea.y + StatsSize.y - 70 - margin), 25, true);
	if(highscore)
		FillText("High score!", Point2f(StatsArea.x + margin, StatsArea.y + 50), 40, true);

	//highscore area
	std::vector<int> scores{GetHighScores()};
	for (int i = int(scores.size()-1); i >= 0; i--)
	{
		int size{ 10 };
		if (i == 0)size += 10;
		FillText(std::to_string(scores[scores.size()-1-i]), Point2f(ScoreArea.x + margin, ScoreArea.y + ScoreSize.y - 65 - 20 * i), size, true);
	}

	float scaleButton{ 20 };
	if (UIButton(Point2f(g_WindowWidth / 2 - 275, scaleButton), GetTexture("return.png"), 50)) {
		g_Scene = 0;
		g_MainGame.reset();
	}
}

void SettingsMenu::Draw() {
	float margin{18};
	SetColor(38 / 255.0f, 32 / 255.0f, 60 / 255.0f, 1);
	FillRect(0,0, g_WindowWidth, g_WindowHeight); //bg

	//show input
	UITexture(Point2f(0,g_WindowHeight/2), GetTexture("Input.png"),100);

	//volume setting
	FillText("volume",Point2f(margin,g_WindowHeight/2-50),40,true);
	FillText("0",Point2f(260,g_WindowHeight/2 - 40),20,true);
	FillText("100",Point2f(g_WindowWidth - 100, g_WindowHeight/2 - 40),20,true);

	float v{ DrawSlider(volume,Point2f(330, g_WindowHeight / 2 - 40),Point2f(800,20)) };
	SetVolume(v);
	if (volume != v) {
		volume = v;
		std::vector<float> volumeData_{};
		volumeData_.push_back(v);
		WrightToFileFloats(volumeData_,"Savefiles/settings.txt");
	}

	//reset highscores
	if (UIButton(Point2f(margin, 150), GetTexture("reset.png"), 30)) {
		resetScore();
	}

	//return
	if (UIButton(Point2f(g_WindowWidth / 2 - 275, 20), GetTexture("return.png"), 50)) {
		Return();
	}
}

void SettingsMenu::Update(float elapsedSec) {

}

void SettingsMenu::Return() {
	g_Scene = m_Scene;
	engine->setAllSoundsPaused(false);
}

void SettingsMenu::start() {
	engine->setAllSoundsPaused(true);
	std::string path{"Savefiles/settings.txt"};
	volume = ReadFileForfloat(path)[0];
}

float SettingsMenu::DrawSlider(float prev, Point2f pos, Point2f size) {
	SetColor(201 / 255.0f, 106 / 255.0f, 56 / 255.0f);
	FillRect(pos.x,pos.y,size.x,size.y);
	FillEllipse(pos.x + size.x * prev, pos.y + size.y / 2, size.y * 1.5f, size.y * 1.5f);
	float output{prev};
	if (Input.ClickDown == Input.MB1)
	{
		if (Input.mousePos.x > pos.x && Input.mousePos.x < pos.x + size.x
		&&  Input.mousePos.y > pos.y - size.y / 2 && Input.mousePos.y < pos.y + size.y + size.y / 2) {
			output = (Input.mousePos.x - pos.x) / size.x;
		}
	}
	return output;
}