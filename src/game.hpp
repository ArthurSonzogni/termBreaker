#ifndef TERM_BREAKER_GAME_HPP
#define TERM_BREAKER_GAME_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>
#include "board.hpp"

namespace term_breaker {

void StartGame();

// Exposed for testing.
ftxui::Component Intro(bool* enable_audio, std::function<void()> quit);
ftxui::Component GameScreen(Board& board);
ftxui::Component WinScreen(int coins, std::function<void()> continuation);
ftxui::Component MainMenu(std::function<void()> play,
                          std::function<void()> quit);

}  // namespace term_breaker

#endif  // TERM_BREAKER_GAME_HPP

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
