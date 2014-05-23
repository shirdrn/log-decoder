package org.shirdrn.log.decoder.common;

import org.shirdrn.log.decoder.common.handler.ByteValueHandler;
import org.shirdrn.log.decoder.metadata.EventTypeAdditionMetadata;
import org.shirdrn.log.decoder.metadata.EventTypeMetadata;
import org.shirdrn.log.decoder.metadata.ParamTypeMetadata;
import org.shirdrn.log.decoder.metadata.PrimitiveTypeMetadata;
import org.shirdrn.log.decoder.metadata.SkipEventList;


public class Context {

	private byte[] fileByteBuf;
	private final Metadata<String, ByteValueHandler<?>> primitiveTypeMetadata;
	private final Metadata<Integer, ParamType> paramTypeMetadata;
	private final Metadata<EventTypeId, EventType> eventTypeMetadata;
	private final Metadata<Integer, AdditionType> additionTypeMetadata;
	private final SkipEventList skipEventList;
	
	public Context() {
		primitiveTypeMetadata = PrimitiveTypeMetadata.newInstance();
		paramTypeMetadata = ParamTypeMetadata.newInstance();
		eventTypeMetadata = EventTypeMetadata.newInstance();
		additionTypeMetadata = new EventTypeAdditionMetadata(eventTypeMetadata);
		skipEventList = SkipEventList.newInstance();
	}

	public byte[] getFileByteBuf() {
		return fileByteBuf;
	}

	public void setFileByteBuf(byte[] fileByteBuf) {
		this.fileByteBuf = fileByteBuf;
	}

	public Metadata<EventTypeId, EventType> getEventTypeMetadata() {
		return eventTypeMetadata;
	}

	public Metadata<String, ByteValueHandler<?>> getPrimitiveTypeMetadata() {
		return primitiveTypeMetadata;
	}

	public Metadata<Integer, ParamType> getParamTypeMetadata() {
		return paramTypeMetadata;
	}

	public Metadata<Integer, AdditionType> getAdditionTypeMetadata() {
		return additionTypeMetadata;
	}

	public SkipEventList getSkipEventList() {
		return skipEventList;
	}

}
