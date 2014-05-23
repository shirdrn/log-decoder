package org.shirdrn.log.decoder.common.handler;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.EventDataPacket.ParamValue;
import org.shirdrn.log.decoder.utils.ByteUtils;

public class StringValueHandler extends AbstractValueHandler<String> {

	private static final Log LOG = LogFactory.getLog(StringValueHandler.class);
	
	public StringValueHandler(String type) {
		super(type, 64);
	}
	
	protected StringValueHandler(String type, int byteCount) {
		super(type, byteCount);
	}

	@Override
	public String handle(byte[] buffer) {
		return value = ByteUtils.decodeString(buffer);
	}

	@Override
	public ParamValue chooseBlock(int start, final byte[] fileByteBuffer) {
		int end = -1;
		int count = start + byteCount;
		boolean found = false;
		for (int i = start; i < Math.min(count, fileByteBuffer.length); i++) {
			// '\0' terminate character
			if(fileByteBuffer[i] == 0) { 
				end = i;
				found = true;
				break;
			}
		}
		if(!found) {
			end = count;
		}
		LOG.debug("Param data pos: type = " + type + ", start = " + start + ", end = " + end);
		ParamValue value = new ParamValue(type, start, end);
		value.setTotalByteCount(end - start + 1);
		if(found) {
			value.setExistsTerminateChar(true);
		} else {
			value.setExistsTerminateChar(false);
		}
		return value;
	}

	@Override
	public String valueOf(String value) {
		this.value = value;
		return value;
	}

}
