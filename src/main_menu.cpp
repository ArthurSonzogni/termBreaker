#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "logo.hpp"

namespace term_breaker {

using namespace ftxui;

namespace {

const std::vector<std::string> g_levels_name = {
    "Very easy level",  //
    "Easy level",       //
    "Medium level",     //
    "Hard level",       //
    "End game ",        //
    "Do not hope ",     //
};

const std::vector<int> g_prize = {
    2000,   //
    4000,   //
    8000,   //
    16000,  //
    32000,  //
    64000,  //
};

// A tab menu, with extra wide items.
MenuOption CustomMenuOption() {
  auto option = MenuOption::HorizontalAnimated();
  option.entries.transform = [](const EntryState& state) {
    Element e = text(state.label) | borderEmpty | borderEmpty;
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  return option;
};

Component PlayTab(std::function<void(int)> play) {
  struct Impl : public ComponentBase {
   public:
    Impl(std::function<void(int)> play) {
      auto on_click = [&, play] { play(index_); };
      menu_ = Menu(&g_levels_name, &index_);
      play_button_ = Button("Play", on_click,
                            ButtonOption::Animated(Color::Red, Color::White));

      auto layout = Container::Horizontal({
          menu_,
          play_button_,
      });

      auto renderer = Renderer(layout, [&] {
        return hbox({
            menu_->Render() | borderEmpty,
            vbox({
                text("Title: " + g_levels_name.at(size_t(index_))),
                text("Prize: " + std::to_string(g_prize.at(size_t(index_)))),
            }) | size(WIDTH, GREATER_THAN, 25) |
                border,
            play_button_->Render() | size(WIDTH, GREATER_THAN, 7) | borderEmpty,
        });
      });

      Add(renderer);
    }

   private:
    Component menu_;
    Component play_button_;
    int index_ = 0;
  };
  return Make<Impl>(play);
}

Component ShopTab() {
  return Renderer([&] { return text("coucou"); });
}

Component QuitTab(std::function<void()> quit) {
  return Button("Quit", quit, ButtonOption::Animated());
}

Element MainMenuDecorator(Element element) {
  return vbox({
      text("Term Breaker") | bold | center,
      element,
  });
}

}

Component MainMenu(std::function<void(int)> play, std::function<void()> quit) {
  static const std::vector<std::string> tab_entries_ = {
      "Play",
      "Shop",
      "Quit",
  };

  class Impl : public ComponentBase {
   private:
    int tab_index_ = 0;

   public:
    Impl(std::function<void(int)> play, std::function<void()> quit) {
      auto menu = Menu(&tab_entries_, &tab_index_, CustomMenuOption());
      auto tab_content = Container::Tab(
          {
              PlayTab(play),
              ShopTab(),
              QuitTab(quit),
          },
          &tab_index_);
      auto component = Container::Vertical({
          menu,
          tab_content,
      });
      component |= MainMenuDecorator;
      Add(component);
    }
  };

  return Make<Impl>(std::move(play), std::move(quit));
}

}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
