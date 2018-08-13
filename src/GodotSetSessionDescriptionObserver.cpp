#include "WebRTCPeer.hpp"

using namespace godot_webrtc;

WebRTCPeer::GodotSSDO::GodotSSDO(WebRTCPeer* parent)
{
  this->parent = parent;
}

void WebRTCPeer::GodotSSDO::OnSuccess() {
  // UNUSED

  // std::string message = "SetSessionDescriptionObserver::OnSuccess - doing nothing";
  // parent->queue_signal("notify", message.c_str());
  // std::cout << parent->name << " state: " << parent->peer_connection->signaling_state() << std::endl;
};

void WebRTCPeer::GodotSSDO::OnFailure(const std::string& error) {
  // UNUSED

  // std::string message = "SetSessionDescriptionObserver::OnFailure: error = ";
  // message += error;
  // parent->queue_signal("notify", message.c_str());
};
