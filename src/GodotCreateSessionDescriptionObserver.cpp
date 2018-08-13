#include "WebRTCPeer.hpp"

using namespace godot_webrtc;

WebRTCPeer::GodotCSDO::GodotCSDO(WebRTCPeer* parent)
{
  this->parent = parent;
}

void WebRTCPeer::GodotCSDO::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
  // std::string message = "CreateSessionDescriptionObserver::OnSuccess - Creating ";
  // message += desc->type() + " & setting local description";
  // parent->queue_signal("notify", message.c_str());

  // serialize this offer and send it to the remote peer:
  std::string sdp; // sdp = session description protocol
  desc->ToString(&sdp);
  godot::String type = desc->type().c_str();
  bool isOffer = type == "offer";
  parent->queue_signal("offer_created", sdp.c_str(), isOffer);
};

void WebRTCPeer::GodotCSDO::OnFailure(const std::string& error) {
  // UNUSED

  // std::string message = "CreateSessionDescriptionObserver::OnFailure: error = ";
  // message += error;
  // parent->queue_signal("notify", message.c_str());
};
