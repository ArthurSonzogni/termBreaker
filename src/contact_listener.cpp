#include "contact_listener.hpp"
#include "brick.hpp"

namespace term_breaker {

namespace {

void OnContact(b2BodyUserData data, float impulse) {
  auto* brick = reinterpret_cast<BrickBase*>(data.pointer);  // NOLINT
  if (brick != nullptr) {
    brick->OnContact(impulse);
  }
}

}  // namespace

void ContactListener::PostSolve(b2Contact* contact,
                                const b2ContactImpulse* impulse) {
  OnContact(contact->GetFixtureA()->GetBody()->GetUserData(),
            impulse->normalImpulses[0]);
  OnContact(contact->GetFixtureB()->GetBody()->GetUserData(),
            impulse->normalImpulses[0]);
}

}  // namespace term_breaker

// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
