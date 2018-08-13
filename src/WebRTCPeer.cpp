#include "WebRTCPeer.hpp"

using namespace godot_webrtc;

void WebRTCPeer::set_write_mode(godot_int mode) {

}

godot_int WebRTCPeer::get_write_mode() const {
	return 0;
}

bool WebRTCPeer::was_string_packet() const {
	return false;
}

godot_int WebRTCPeer::get_connection_state() const {
	return 0;
}

godot_error WebRTCPeer::create_offer() {
	return GODOT_FAILED;
}

godot_error WebRTCPeer::set_remote_description(const char *type, const char *sdp) {
	return GODOT_FAILED;
}

godot_error WebRTCPeer::set_local_description(const char *type, const char *sdp) {
	return GODOT_FAILED;
}

godot_error WebRTCPeer::add_ice_candidate(const char *sdpMidName, int sdpMlineIndexName, const char *sdpName) {
	return GODOT_FAILED;
}

godot_error WebRTCPeer::poll() {
	return GODOT_FAILED;
}

godot_error WebRTCPeer::get_packet(const uint8_t **r_buffer, int &r_len) {
	printf("Get packet");
	r_len = 0;
	return GODOT_OK;
}

godot_error WebRTCPeer::put_packet(const uint8_t *p_buffer, int p_len) {
	printf("Put packet");
	return GODOT_OK;
}

godot_int WebRTCPeer::get_available_packet_count() const {
	printf("Get packet count");
	return 2;
}

godot_int WebRTCPeer::get_max_packet_size() const {
	printf("Get max packet size");
	return 1024;
}

void WebRTCPeer::_register_methods() { }

void WebRTCPeer::_init() {
	printf("Binding PacketPeer interface");
	register_interface(&interface);

	// initialize variables:
  mutex_signal_queue = new std::mutex; // @FIXME figure out how to add mutexes in gdnative
  mutex_packet_queue = new std::mutex; // @FIXME figure out how to add mutexes in gdnative
  packet_queue_size = 0;

	// create a PeerConnectionFactoryInterface:
	signaling_thread = new rtc::Thread;
  signaling_thread->Start();
  pc_factory = webrtc::CreateModularPeerConnectionFactory(
    nullptr, // rtc::Thread* network_thread,
    nullptr, // rtc::Thread* worker_thread,
    signaling_thread,
    nullptr, // std::unique_ptr<cricket::MediaEngineInterface> media_engine,
    nullptr, // std::unique_ptr<CallFactoryInterface> call_factory,
    nullptr  // std::unique_ptr<RtcEventLogFactoryInterface> event_log_factory
  );
  if (pc_factory.get() == nullptr) { // PeerConnectionFactory couldn't be created. Fail the method call.
		godot_print_error("PeerConnectionFactory could not be created", "_init", "WebRTCPeer.cpp", 80);
		// return GODOT_FAILED;
	}

	// create PeerConnection configuration and add the ice servers:
	webrtc::PeerConnectionInterface::RTCConfiguration configuration;
	webrtc::PeerConnectionInterface::IceServer ice_server;

	ice_server.uri = "stun:stun.l.google.com:19302"; // @FIXME allow user to input ice servers
	configuration.servers.push_back(ice_server);

	// create a PeerConnection object:
  peer_connection = pc_factory->CreatePeerConnection(configuration, nullptr, nullptr, &pco);
	if (peer_connection.get() == nullptr) { // PeerConnection couldn't be created. Fail the method call.
		godot_print_error("PeerConnection could not be created", "_init", "WebRTCPeer.cpp", 101);
		// return GODOT_FAILED;
	}

	// create a DataChannel
	webrtc::DataChannelInit data_channel_config;
	data_channel_config.negotiated = true; // True if the channel has been externally negotiated
	data_channel_config.id = 0;

	data_channel = peer_connection->CreateDataChannel("channel", &data_channel_config);
	// @TODO (NONESSENTIAL) create data_channel check. fail function call if data_channel isn't created
	data_channel->RegisterObserver(&dco);


}

WebRTCPeer::WebRTCPeer() :  dco(this)
													, pco(this)
                        	// , ptr_csdo(new rtc::RefCountedObject<GD_CSDO>(this))
                        	// , ptr_ssdo(new rtc::RefCountedObject<GD_SSDO>(this))
{
}

WebRTCPeer::~WebRTCPeer() {
	if (_owner) {
		printf("Unbinding PacketPeer interface");
		register_interface(NULL);
	}

	delete mutex_signal_queue;
	mutex_signal_queue = NULL;

	delete mutex_packet_queue;
	mutex_packet_queue = NULL;
}

void WebRTCPeer::queue_signal(godot::String p_name, const godot::Variant &p_arg1, const godot::Variant &p_arg2, const godot::Variant &p_arg3, const godot::Variant &p_arg4, const godot::Variant &p_arg5)
{
  mutex_signal_queue->lock();
  signal_queue.push(
    [this, p_name, p_arg1, p_arg2, p_arg3, p_arg4, p_arg5]{
      emit_signal(p_name, p_arg1, p_arg2, p_arg3, p_arg4, p_arg5);
    }
  );
  mutex_signal_queue->unlock();
}

void WebRTCPeer::queue_packet(uint8_t* buffer, int buffer_size)
{
  mutex_packet_queue->lock();
  packet_queue.push(buffer);
  packet_sizes_queue.push(buffer_size);
  ++packet_queue_size;
  mutex_packet_queue->unlock();
}
