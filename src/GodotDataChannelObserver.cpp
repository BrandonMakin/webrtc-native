#include "WebRTCPeer.hpp"

using namespace godot_webrtc;

WebRTCPeer::GodotDCO::GodotDCO(WebRTCPeer* parent) {
  this->parent = parent;
}

void WebRTCPeer::GodotDCO::OnMessage(const webrtc::DataBuffer& buffer) {
  const uint8_t* data = buffer.data.data<uint8_t>();
  uint8_t* memory_controlled_buffer = new uint8_t[buffer.data.size()];

  std::copy(data, data + buffer.data.size(), memory_controlled_buffer);
  parent->queue_packet(memory_controlled_buffer, buffer.data.size());
};

void WebRTCPeer::GodotDCO::OnStateChange() {
  // UNUSED
  // @TODO (NONESSENTIAL) consider using this to signal when
  // to change the state of NetworkedMultiplayerWebRTC instead
  // of polling to check the DataChannel's state (as it does now)
};

void WebRTCPeer::GodotDCO::OnBufferedAmountChange(uint64_t previous_amount) {
  // UNUSED
};
