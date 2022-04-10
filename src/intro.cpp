#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace term_breaker {

using namespace ftxui;

namespace {

Element logo(Element buttons) {
  auto upper = vbox({
      text(R"( _____ _____ ____  __  __ )"),
      text(R"(|_   _| ____|  _ \|  \/  |)"),
      text(R"(  | | |  _| | |_) | |\/| |)"),
      text(R"(  | | | |___|  _ <| |  | |)"),
      text(R"(  |_| |_____|_| \_\_|  |_|)"),
  });

  auto lower = vbox({
      text(""),
      text(R"( ____  ____  _____    _    _  _______ ____  )"),
      text(R"(| __ )|  _ \| ____|  / \  | |/ / ____|  _ \ )"),
      text(R"(|  _ \| |_) |  _|   / _ \ | ' /|  _| | |_) |)"),
      text(R"(| |_) |  _ <| |___ / ___ \| . \| |___|  _ < )"),
      text(R"(|____/|_| \_\_____/_/   \_\_|\_\_____|_| \_\)"),
      text(""),
  });

  auto description = vbox({
    filler(), paragraphAlignCenter("Welcome to the term-breaker game!"),
        paragraphAlignCenter("This is a game made for the"),
        paragraphAlignCenter("C++ Best Practices Game Jam."),
        paragraphAlignCenter("This game was made by @arthursonzogni"), filler(),
        paragraphAlignCenter(
            "Use your mouse to throw balls toward the blocks before they reach "
            "you"),
        paragraphAlignCenter("Earn coins by completing levels"),
        paragraphAlignCenter("Buy additional balls to grow."),
        paragraphAlignCenter(""),
        paragraphAlignCenter("Please press any key to start") | blink, filler(),
        paragraphAlignCenter(
            "More info on https://github.com/ArthurSonzogni/term-breaker"),
        filler(),
#if !defined(__EMSCRIPTEN__)
        paragraphAlignCenter(
            "Due to the audio library used, you have to decide whether to "
            "support sounds or pass the ASAN checks"),
#endif
  });

  auto document = vbox({
      upper | center | flex_shrink,
      lower | center | flex_shrink,
      separator(),
      description | flex_grow,
      buttons | center,
  });

  return document | border;
}
}  // namespace

void Intro(bool* enable_audio) {
  auto screen = ScreenInteractive::Fullscreen();

#if defined(__EMSCRIPTEN__)
  *enable_audio = true;
  auto buttons = Button("Start", screen.ExitLoopClosure(),
                        ButtonOption::Animated(Color::Green));
#else
  auto start_with_audio = [&] {
    *enable_audio = true;
    screen.ExitLoopClosure()();
  };
  auto start_without_audio = [&] {
    *enable_audio = false;
    screen.ExitLoopClosure()();
  };
  auto btn_option_audio = ButtonOption::Animated(Color::Blue);
  auto btn_option_asan = ButtonOption::Animated(Color::Red);

  auto buttons = Container::Horizontal({
      Button("Start with Audio", start_with_audio, btn_option_audio),
      Button("Start with ASAN", start_without_audio, btn_option_asan),
  });
#endif

  auto component = buttons | logo;
  screen.Loop(component);
}

};  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
