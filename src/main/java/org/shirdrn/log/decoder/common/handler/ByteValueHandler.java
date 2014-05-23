package org.shirdrn.log.decoder.common.handler;

import org.shirdrn.log.decoder.EventDataPacket.ParamValue;
import org.shirdrn.log.decoder.common.ValueHandler;

public interface ByteValueHandler<D> extends ValueHandler<byte[], D> {

	ParamValue chooseBlock(int start, final byte[] fileByteBuffer);
	int byteCount();
}
