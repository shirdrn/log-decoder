package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class Int32 extends NumberValueHandler<Integer> {
	
	public Int32(String type) {
		super(type, 4);
	}
	
	@Override
	public Integer handle(byte[] buffer) {
		return value = ByteUtils.decodeInt(buffer);
	}

	@Override
	public Integer valueOf(String value) {
		return this.value = Integer.parseInt(value);
	}
}