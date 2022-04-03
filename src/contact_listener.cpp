#include "contact_listener.hpp"
#include "brick.hpp"

namespace {

void OnContact(b2BodyUserData data) {
  auto* brick = reinterpret_cast<BrickBase*>(data.pointer);  // NOLINT
  if (brick != nullptr) {
    brick->OnContact();
  }
}

}  // namespace

void ContactListener::BeginContact(b2Contact* contact) {
  OnContact(contact->GetFixtureA()->GetBody()->GetUserData());
  OnContact(contact->GetFixtureB()->GetBody()->GetUserData());
}
