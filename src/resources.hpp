#ifndef TERM_BREAKER_RESOURCES_HPP
#define TERM_BREAKER_RESOURCES_HPP

#include <memory>
#include <smk/Sound.hpp>
#include <smk/SoundBuffer.hpp>
#include <vector>

namespace term_breaker {

void PlaySound(const smk::SoundBuffer& snd);
std::string ResourcePath();

extern smk::SoundBuffer sb_boing;

void LoadResources();
void UnloadResources();

}  // namespace term_breaker

#endif  // TERM_BREAKER_RESOURCES_HPP

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
