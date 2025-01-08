#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <SFML/Audio.hpp>
#include <filesystem>
#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>  // Pour std::rand et std::srand
#include <ctime>    // Pour std::time

void playMusicFromFolder(const std::string& folderPath) ;


#endif // MUSIC_HPP