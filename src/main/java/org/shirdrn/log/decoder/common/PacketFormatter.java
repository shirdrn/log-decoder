package org.shirdrn.log.decoder.common;

public interface PacketFormatter<T> {

	T format(EventPacket packet);
}
