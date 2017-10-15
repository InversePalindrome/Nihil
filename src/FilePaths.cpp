/*
Copyright (c) 2017 InversePalindrome
Nihil - FilePaths.cpp
InversePalindrome.com
*/


#include "FilePaths.hpp"


Path::FilePath::FilePath(const std::string& fileName) :
	path(fileName)
{
}

std::string Path::FilePath::operator/(const std::string& extension)
{
	return (this->path / extension).generic_string();
}

extern Path::FilePath Path::files("DataFiles/");
extern Path::FilePath Path::games(files / "Games/");
extern Path::FilePath Path::levels(files / "Levels/");
extern Path::FilePath Path::blueprints(files / "Blueprints/");
extern Path::FilePath Path::animations(files / "Animations/");
extern Path::FilePath Path::particles(files / "Particles/");
extern Path::FilePath Path::GUI(files / "GUI/");
extern Path::FilePath Path::miscellaneous(files / "Miscellaneous/");
extern Path::FilePath Path::resources("Resources/");
extern Path::FilePath Path::textures(resources / "Textures/");
extern Path::FilePath Path::fonts(resources / "Fonts/");
extern Path::FilePath Path::images(resources / "Images/");
extern Path::FilePath Path::sounds(resources / "Sounds/");
extern Path::FilePath Path::music(resources / "Music/");