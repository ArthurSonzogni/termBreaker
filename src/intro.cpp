#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace term_breaker {

namespace {

using namespace ftxui;

Element logo() {
  auto upper = vbox({
      text(R"( _____ _____ ____  __  __ )"),
      text(R"(|_   _| ____|  _ \|  \/  |)"),
      text(R"(  | | |  _| | |_) | |\/| |)"),
      text(R"(  | | | |___|  _ <| |  | |)"),
      text(R"(  |_| |_____|_| \_\_|  |_|)"),
  });

  auto lower = vbox({
      text(R"( ____  ____  _____    _    _  _______ ____  )"),
      text(R"(| __ )|  _ \| ____|  / \  | |/ / ____|  _ \ )"),
      text(R"(|  _ \| |_) |  _|   / _ \ | ' /|  _| | |_) |)"),
      text(R"(| |_) |  _ <| |___ / ___ \| . \| |___|  _ < )"),
      text(R"(|____/|_| \_\_____/_/   \_\_|\_\_____|_| \_\)"),
  });

  auto description = Elements({
      text("Welcome to the term-breaker game!"),
      text("This is a game made for the"),
      text("C++ Best Practices Game Jam by @arthursonzogni"),
      text(""),
      text(
      text("Use your mouse to throw balls toward the blocks before they reach "
           "you"),
      text("Earn coins by completing levels"),
      text("Buy additional balls to grow your power"),
      text(""),
      text("Please press any key to start"),
      text(""),
      text("More info on https://github.com/ArthurSonzogni/term-breaker"),
  });

  for (auto& it : description)
    it |= center;

  auto document = vbox({
      upper | center,
      lower | center,
      separator(),
      vbox(std::move(description)),
  });

  return document | border;
}
}  // namespace

void Intro() {
  auto component = Renderer(logo);
  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
}

};  // namespace term_breaker
