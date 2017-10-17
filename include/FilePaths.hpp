/*
Copyright (c) 2017 InversePalindrome
Nihil - FilePaths.hpp
InversePalindrome.com
*/


#pragma once

#include <boost/filesystem/path.hpp>


namespace Path
{
	struct FilePath
	{
		FilePath(const std::string& fileName);

		std::string operator/(const std::string& extension);

		boost::filesystem::path path;
	};

	extern FilePath files;
	extern FilePath games;
	extern FilePath levels;
	extern FilePath blueprints;
	extern FilePath animations;
	extern FilePath particles;
	extern FilePath GUI;
	extern FilePath spriteInfo;
	extern FilePath dialogues;
	extern FilePath miscellaneous;
	extern FilePath resources;
	extern FilePath textures;
	extern FilePath fonts;
	extern FilePath images;
	extern FilePath sounds;
	extern FilePath music;
}