package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class UInt32 extends NumberValueHandler<Long> {
	
	public UInt32(String type) {
		super(type, 4);
	}
	
	@Override
	public Long handle(byte[] buffer) {
		return value = ByteUtils.decodeUnsignedInt(buffer);
	}

	@Override
	public Long valueOf(String value) {
		return Long.parseLong(value);
	}
}