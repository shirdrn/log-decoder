package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class Double8 extends NumberValueHandler<Double> {
	
	public Double8(String type) {
		super(type, 8);
	}
	
	@Override
	public Double handle(byte[] buffer) {
		return value = ByteUtils.decodeUnsignedDouble(buffer);
	}

	@Override
	public Double valueOf(String value) {
		return Double.parseDouble(value);
	}
}