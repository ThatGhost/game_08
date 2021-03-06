#pragma once

namespace SLUtils {
	void WrightToFileInts(std::vector<int>& data,const std::string& path);
	void WrightToFileFloats(std::vector<float>& data,const std::string& path);
	std::vector<int> ReadFileForInt(std::string& path);
	std::vector<float> ReadFileForfloat(std::string path);
	std::vector<int> ReplaceValue(std::vector<int>& vector, int& value, int place);
}

bool AddEndScore(int& score);
void InitializeGameAssets();
void DeleteGameAssets();
Texture* GetTexture(const std::string& id);
std::vector<int> GetHighScores();
void resetScore();
