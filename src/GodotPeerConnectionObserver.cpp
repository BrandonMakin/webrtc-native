#include "WebRTCPeer.hpp"

using namespace godot_webrtc;

WebRTCPeer::GodotPCO::GodotPCO(WebRTCPeer* parent)
{
  this->parent = parent;
}

void WebRTCPeer::GodotPCO::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnSignalingChange - doing nothing");
}

void WebRTCPeer::GodotPCO::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnAddStream");
}

void WebRTCPeer::GodotPCO::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnRemoveStream");
}

void WebRTCPeer::GodotPCO::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnDataChannel");
}

void WebRTCPeer::GodotPCO::OnRenegotiationNeeded()
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnRenegotiationNeeded");
}

void WebRTCPeer::GodotPCO::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnIceConnectionChange");
}

void WebRTCPeer::GodotPCO::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
  // parent->queue_signal("notify", "PeerConnectionObserver::OnIceGatheringChange");
}

void WebRTCPeer::GodotPCO::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
  // Serialize the candidate and send it to the remote peer:

  godot::Dictionary candidateSDP;

  godot::String candidateSdpMidName = candidate->sdp_mid().c_str();
  int candidateSdpMlineIndexName = candidate->sdp_mline_index();
  std::string sdp;
  candidate->ToString(&sdp);
  godot::String candidateSdpName = sdp.c_str();

  parent->queue_signal("new_ice_candidate",
                      candidateSdpMidName,
                      candidateSdpMlineIndexName,
                      candidateSdpName
  );
}
