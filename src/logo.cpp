#include "logo.hpp"

namespace term_breaker {
using namespace ftxui;
Element Logo() {
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

  return vbox({
      upper | center,
      lower | center,
  });
}

}  // namespace term_breaker
