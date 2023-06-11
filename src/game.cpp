#include "game.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
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

const std::vector<int> g_prize = {
    2000,   //
    4000,   //
    8000,   //
    16000,  //
    32000,  //
    64000,  //
};

void ExecuteBoard(BoardConfig config,
                  std::function<void()> win,
                  std::function<void()> lose,
                  std::function<void()> quit) {
  auto screen = ScreenInteractive::Fullscreen();
  auto exit = screen.ExitLoopClosure();
  auto on_quit = [=] {
    quit();
    exit();
  };
  auto on_win = [=] {
    win();
    exit();
  };
  auto on_lose= [=] {
    lose();
    exit();
  };

  Board board(config, on_win, on_lose);

  auto component = GameScreen(board, on_lose, on_quit);

  Loop loop(&screen, component);

  while (!loop.HasQuitted()) {
    loop.RunOnce();
    using namespace std::chrono_literals;
    const auto refresh_time = 1.0s / 60.0;
    std::this_thread::sleep_for(refresh_time);
    screen.PostEvent(Event::Custom);
  }
}

void ExecuteWinScreen(int coins) {
  auto screen = ScreenInteractive::Fullscreen();
  auto component = WinScreen(coins, screen.ExitLoopClosure());
  screen.Loop(component);
}

void ExecuteLoseScreen() {
  auto screen = ScreenInteractive::Fullscreen();
  auto component = LoseScreen(screen.ExitLoopClosure());
  screen.Loop(component);
}

void ExecuteMainMenu(BoardConfig& config,
                     std::function<void(int)> play,
                     std::function<void()> quit) {
  auto screen = ScreenInteractive::Fullscreen();
  auto exit = screen.ExitLoopClosure();
  auto play_and_exit = [play, exit](int level) {
    play(level);
    exit();
  };
  auto quit_and_exit = [quit, exit]() {
    quit();
    exit();
  };
  auto menu = MainMenu(config, play_and_exit, quit_and_exit);
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
                            std::function<void()> lose,
                            std::function<void()> quit) {
  auto button_back = Button("Back", lose, ButtonOption::Animated(Color::Blue));
  auto button_quit = Button("Quit", quit, ButtonOption::Animated(Color::Red));
  auto layout = Container::Vertical({
      button_back,
      button_quit,
  });

  auto component = Renderer(layout, [&, button_back, button_quit] {
    return board.Draw(button_back->Render(), button_quit->Render());
  });
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
  config.balls = 5;  // NOLINT
  int level_to_play = -1;

  bool quit = false;
  auto on_quit = [&] { quit = true; };

  int iterations = 0;

  while (!quit) {
    iterations++;
    bool level_selected = false;
    auto select_level = [&](int level) {
      level_selected = true;
      level_to_play = level;
    };

    // Skip the first menu, because it is fun starting playing the game
    // directly.
    if (iterations != 1) { // NOLINT
      ExecuteMainMenu(config, select_level, on_quit);
    }
    else {
      level_selected = true;
      level_to_play = 0;
    }

    if (quit) {
      break;
    }

    if (level_selected) {
      bool win = false;
      auto on_win = [&] { win = true; };
      auto on_lose = [&] { win = false; };

      config.difficulty = level_to_play;
      ExecuteBoard(config, on_win, on_lose, on_quit);

      if (quit) {
        break;
      }

      if (win) {
        ExecuteWinScreen(g_prize.at(size_t(level_to_play)));
        config.coins += g_prize.at(size_t(level_to_play));
      } else {
        ExecuteLoseScreen();
      }
    }
  }

  UnloadResources();
}
}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
