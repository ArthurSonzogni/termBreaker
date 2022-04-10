#include "game.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <thread>
#include "board.hpp"
#include "intro.hpp"
#include "resources.hpp"
#ifdef ENABLE_AUDIO
#include <smk/Audio.hpp>
#endif

namespace term_breaker {

using namespace ftxui;

namespace {
void Play(BoardConfig config) {
  auto screen = ScreenInteractive::Fullscreen();

  Board board(config);
  auto renderer = Renderer([&] { return board.Draw(); });
  renderer |= CatchEvent([&](Event event) {  // NOLINT
    if (event == Event::Custom) {
      board.Step();
      return true;
    }

    return board.OnEvent(event);
  });


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
  screen.Loop(renderer);

  refresh_ui_continue = false;
  refresh_ui.join();
}

void Win(int coins) {
  auto screen = ScreenInteractive::Fullscreen();
  auto component = WinScreen(coins, screen.ExitLoopClosure());
  screen.Loop(component);
}

void MainMenuScreen() {
  auto screen = ScreenInteractive::Fullscreen();
  auto menu = MainMenu([] {}, screen.ExitLoopClosure());
  screen.Loop(menu);
}

} // namespace

void StartGame(bool enable_audio) {
#ifdef ENABLE_AUDIO
  smk::Audio audio;  // Initialize OpenAL.
#else
  (void)enable_audio;
#endif

  if (enable_audio)
    LoadResources();

  BoardConfig config;
  config.balls = 10;  // NOLINT

  while(true) {
    Win(10);
    Play(config);
    MainMenuScreen();
  }


  UnloadResources();
}
}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
