#include "resources.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace term_breaker {

namespace {
std::vector<smk::Sound> sounds;
size_t sound_index = 0;

std::string GetEnvironmentVariable(const char* env) {
  auto value = std::getenv(env);
  if (value)
    return value;
  return std::string();
}

}  // namespace

std::string ResourcePath() {
  static bool initialized = false;
  static std::string resource_path;
  if (initialized)
    return resource_path;
  initialized = true;

  auto SNAP = GetEnvironmentVariable("SNAP");

  std::vector<std::string> path_list = {
      // Application installed using snapcraft.
      SNAP + "/share/termBreaker/resources",

      // Application installed using "sudo make install"
      "/usr/local/share/termBreaker/resources",

      // Application installed using "sudo make install"
      "/usr/share/termBreaker/resources",

      // Code build and run inside ${CMAKE_CURRENT_DIRECTORY}/build
      "./resources",

      // Code build and run inside ${CMAKE_CURRENT_DIRECTORY}
      "../resources",

      // Code build for WebAssembly.
      "/resources",
  };

  for (auto& path : path_list) {
    auto file = std::ifstream(path + "/termBreaker");
    if (!file)
      continue;

    std::string line;
    if (std::getline(file, line) && line == "termBreaker") {
      resource_path = path;
    }
  }

  std::cerr << "Resource path = " << resource_path << std::endl;
  return resource_path;
}

smk::SoundBuffer sb_boing;

void LoadResources() {
  sb_boing = smk::SoundBuffer(ResourcePath() + "/sounds/bounce.wav");
}

void UnloadResources() {
  sb_boing = smk::SoundBuffer();
  sounds.clear();
}

void PlaySound(const smk::SoundBuffer& snd) {
  sounds.resize(100);
  sound_index = sound_index % sounds.size();
  sounds.at(sound_index) = smk::Sound(snd);
  sounds.at(sound_index).Play();
}

}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
