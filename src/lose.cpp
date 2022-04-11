#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace term_breaker {
using namespace ftxui;

namespace {

Element Skull() {
  return vbox({
      text(R"(                    ______                       )"),
      text(R"(                 .-"      "-.                    )"),
      text(R"(                /            \                   )"),
      text(R"(    _          |              |          _       )"),
      text(R"(   ( \         |,  .-.  .-.  ,|         / )      )"),
      text(R"(    > "=._     | )(__/  \__)( |     _.=" <       )"),
      text(R"(   (_/"=._"=._ |/     /\     \| _.="_.="\_)      )"),
      text(R".(          "=._ (_     ^^     _)"_.="             )."),
      text(R"(              "=\__|IIIIII|__/="                 )"),
      text(R"(             _.="| \IIIIII/ |"=._                )"),
      text(R"(   _     _.="_.="\          /"=._"=._     _      )"),
      text(R"(  ( \_.="_.="     `--------`     "=._"=._/ )     )"),
      text(R"(   > _.="                            "=._ <      )"),
      text(R"(  (_/                                    \_)     )"),
      text(R"(                                                 )"),
  });
}

Element Decorate(Element inner) {
  return vbox({
      Skull(),
      text(""),
      text("You lose!") | center,
      text(""),
      text("You have collected 0 coins."),
      inner,
  });
};

}  // namespace

Component LoseScreen(std::function<void()> continuation) {
  auto component = Button("Continue", continuation, ButtonOption::Animated());
  return component | Decorate;
}

}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
