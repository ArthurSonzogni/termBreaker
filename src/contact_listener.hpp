#ifndef LINE_RIDER_CONTACT_LISTENER_HPP
#define LINE_RIDER_CONTACT_LISTENER_HPP

#include <box2d/box2d.h>

namespace term_breaker {

class ContactListener : public b2ContactListener {
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};

}  // namespace term_breaker

#endif  // LINE_RIDER_CONTACT_LISTENER_HPP

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
