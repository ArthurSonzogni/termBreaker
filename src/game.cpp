#include "game.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <thread>
#include "board.hpp"
#include "resources.hpp"
#ifdef ENABLE_AUDIO
#include <smk/Audio.hpp>
#endif

namespace term_breaker {

using namespace ftxui;

namespace {

void ExecuteBoard(BoardConfig config) {
  auto screen = ScreenInteractive::Fullscreen();
  Board board(config);

  // This thread exists to make sure that the event queue has an event to
  // process at approximately a rate of 60 FPS
  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      const auto refresh_time = 1.0s / 60.0;
      std::this_thread::sleep_for(refresh_time);
      screen.PostEvent(Event::Custom);
    }
  });

  auto component = GameScreen(
      board, [] {}, screen.ExitLoopClosure());
  screen.Loop(component);

  refresh_ui_continue = false;
  refresh_ui.join();
}

void ExecuteWinScreen(int coins) {
  auto screen = ScreenInteractive::Fullscreen();
  auto component = WinScreen(coins, screen.ExitLoopClosure());
  screen.Loop(component);
}

void ExecuteMainMenu() {
  auto screen = ScreenInteractive::Fullscreen();
  auto menu = MainMenu([] {}, screen.ExitLoopClosure());
  screen.Loop(menu);
}

void ExecuteIntro(bool* enable_audio) {
  auto screen = ScreenInteractive::Fullscreen();
  auto component = Intro(enable_audio, screen.ExitLoopClosure());
  screen.Loop(component);
}

}  // namespace

// The component responsible for renderering the game board.
ftxui::Component GameScreen(Board& board,
                            std::function<void()> win,
                            std::function<void()> exit) {
  (void)win;
  (void)exit;
  auto component = Renderer([&] { return board.Draw(); });
  component |= CatchEvent([&](Event event) {  // NOLINT
    if (event == Event::Custom) {
      board.Step();
      return true;
    }

    return board.OnEvent(event);
  });
  return component;
}

// The main logic, moving the players in between the various screens.
void StartGame() {
#ifdef ENABLE_AUDIO
  smk::Audio audio;  // Initialize OpenAL.
#endif

  bool enable_audio = false;
  ExecuteIntro(&enable_audio);

  if (enable_audio)
    LoadResources();

  BoardConfig config;
  config.balls = 10;  // NOLINT

  while (true) {
    ExecuteMainMenu();
    ExecuteWinScreen(10);
    ExecuteBoard(config);
  }

  UnloadResources();
}
}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
