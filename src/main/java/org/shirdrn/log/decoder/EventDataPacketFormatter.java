package org.shirdrn.log.decoder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.EventDataPacket.ParamValue;
import org.shirdrn.log.decoder.common.Context;
import org.shirdrn.log.decoder.common.EventPacket;
import org.shirdrn.log.decoder.common.PacketFormatter;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;

public class EventDataPacketFormatter implements PacketFormatter<String> {

	private static final Log LOG = LogFactory.getLog(EventDataPacketFormatter.class);
	private final Context context;
	
	public EventDataPacketFormatter(Context context) {
		this.context =  context;
	}

	@Override
	public String format(EventPacket packet) {
		LOG.debug("Event: id = " + packet.getEventId() + ", name = " + packet.getEventName());
		ParamValue pv = packet.getParamHeader();
		StringBuffer valueBuf = new StringBuffer();
		valueBuf.append(packet.getEventId()).append(",")
			.append(packet.getEventName()).append(",")
			.append(packet.getEventTime()).append(",");
		if(packet.getAddition() != null) {
			valueBuf.append(packet.getAddition().getValue()).append(",");
		}
		while(pv != null) {
			String type = pv.getType();
			int start = pv.getStart();
			ByteValueHandler<?> handler = context.getPrimitiveTypeMetadata().get(type);
			int len = pv.getTotalByteCount();
			if(pv.isExistsTerminateChar()) {
				len = len - 1;
			}
			byte[] buf = new byte[len];
			System.arraycopy(context.getFileByteBuf(), start, buf, 0, len);
			Object value = handler.handle(buf);
			valueBuf.append(value).append(",");
			pv = pv.getNext();
		}
		String value = valueBuf.toString();
		if (value.endsWith(",")) {
			value = value.substring(0, value.length() - 1);
		}
		return value;
	}
	
}
