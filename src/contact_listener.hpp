#ifndef LINE_RIDER_CONTACT_LISTENER_HPP
#define LINE_RIDER_CONTACT_LISTENER_HPP

#include <box2d/box2d.h>

class ContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact) override;
};

#endif //LINE_RIDER_CONTACT_LISTENER_HPP
