#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "logo.hpp"

namespace term_breaker {

using namespace ftxui;

namespace {

Element IntroDecorator(Element buttons) {
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
  });

  auto document = vbox({
      Logo() | flex_shrink,
      separator(),
      description | flex_grow,
      buttons | center,
  });

  return document | border;
}
}  // namespace

ftxui::Component Intro(bool* enable_audio, std::function<void()> quit) {
  // Depending on the configuration and platform, we might or might not be able
  // to play audio. If we can, we let the player decide, except when running
  // inside the browser.
#if defined(__EMSCRIPTEN__)
  *enable_audio = true;
  auto buttons = Button("Start", quit, ButtonOption::Animated(Color::Green));
#elif ENABLE_AUDIO
  auto start_with_audio = [=] {
    *enable_audio = true;
    quit();
  };
  auto start_without_audio = [=] {
    *enable_audio = false;
    quit();
  };
  auto btn_option_audio = ButtonOption::Animated(Color::Blue);
  auto btn_option_asan = ButtonOption::Animated(Color::Red);

  auto buttons = Container::Horizontal({
      Button("Start with Audio", start_with_audio, btn_option_audio),
      Button("Start without Audio", start_without_audio, btn_option_asan),
  });
#else
  *enable_audio = false;
  auto buttons = Button("Start", quit, ButtonOption::Animated(Color::Yellow));
#endif  // ENABLE_AUDIO

  return buttons | IntroDecorator;
}

};  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
