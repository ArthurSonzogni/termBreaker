#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace term_breaker {

using namespace ftxui;

namespace {

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

Component PlayTab() {
  return Renderer([&] { return text("Play"); });
}

Component ShopTab() {
  return Renderer([&] { return text("coucou"); });
}

Component QuitTab(std::function<void()> quit) {
  return Button("Quit", quit);
}

}  // namespace

Component MainMenu(std::function<void()> play, std::function<void()> quit) {
  static const std::vector<std::string> tab_entries_ = {
      "Play",
      "Shop",
      "Quit",
  };

  class Impl : public ComponentBase {
   private:
    std::function<void()> play_;
    int tab_index_ = 0;

   public:
    Impl(std::function<void()> play, std::function<void()> quit) : play_(play) {
      auto menu = Menu(&tab_entries_, &tab_index_, CustomMenuOption());
      auto tab_content = Container::Tab(
          {
              PlayTab(),
              ShopTab(),
              QuitTab(quit),
          },
          &tab_index_);
      auto component = Container::Vertical({
          menu,
          tab_content,
      });
      Add(component);
    }
  };

  return Make<Impl>(std::move(play), std::move(quit));
}

}  // namespace term_breaker
