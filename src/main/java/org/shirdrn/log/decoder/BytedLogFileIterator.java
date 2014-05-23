package org.shirdrn.log.decoder;

import java.util.Arrays;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.EventDataPacket.ParamValue;
import org.shirdrn.log.decoder.common.Addition;
import org.shirdrn.log.decoder.common.AdditionType;
import org.shirdrn.log.decoder.common.Context;
import org.shirdrn.log.decoder.common.EventPacket;
import org.shirdrn.log.decoder.common.EventType;
import org.shirdrn.log.decoder.common.EventTypeId;
import org.shirdrn.log.decoder.common.FieldType;
import org.shirdrn.log.decoder.common.ParamType;
import org.shirdrn.log.decoder.common.ParamType.FieldArray;
import org.shirdrn.log.decoder.common.ParamType.FieldInfo;
import org.shirdrn.log.decoder.common.ParamType.FieldPointer;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;
import org.shirdrn.log.decoder.utils.FixLengthBufferCache;
import org.shirdrn.log.decoder.utils.ByteUtils;

public class BytedLogFileIterator implements Iterator<EventPacket> {

	private static final Log LOG = LogFactory.getLog(BytedLogFileIterator.class);
	private final Iterable<EventPacket> iterableBytedLogFile;
	private final BytedLogFile bytedLogFile;
	private final Context context;
	
	private static final byte[] PKT_FLAG = ByteUtils.hexStringToBytes("0F020F02");
	private static final int LEN_PKT_FLAG = PKT_FLAG.length;
	private static final byte[] PKT_FLAG_BUF = new byte[4]; // 4 bytes
	private static final byte[] QUAD_EMPTY_BYTE_BUF = new byte[4]; // 4 bytes, empty buffer
	
	private EventPacket dataPacket;
	private int currentIndex = 0;
	
	public BytedLogFileIterator(Iterable<EventPacket> bytesLogFile) {
		this.iterableBytedLogFile = bytesLogFile;
		this.bytedLogFile = (BytedLogFile) iterableBytedLogFile;
		this.context = this.bytedLogFile.getContext();
	}
	
	private void processPacket() {
		dataPacket = new EventDataPacket(context);
		dataPacket.setStartPos(currentIndex);
		// parse packet flag
		int fixPacketHeaderByteCount = LEN_PKT_FLAG; // 4 bytes
		copy(PKT_FLAG_BUF, fixPacketHeaderByteCount);
		boolean pktFlagFound = Arrays.equals(PKT_FLAG, PKT_FLAG_BUF);
		if(pktFlagFound) {
			LOG.debug("========== PACKET BEGIN =========");
			// parse event id: 2 bytes
			currentIndex += fixPacketHeaderByteCount;
			int eventIdByteCount = 2;
			byte[] buf = FixLengthBufferCache.allocate(eventIdByteCount);
			copy(buf, eventIdByteCount);
			int eventId = ByteUtils.decodeUnsignedShort(buf);
			LOG.debug("eventId = " + eventId);
			// check whether event id is in the skip list 
			if(context.getSkipEventList().isInSkipList(eventId)) {
				LOG.debug("Event is in skip list: eventId = " + eventId);
				currentIndex += eventIdByteCount;
//				// locate next packet flag
				// and set the correct pointer
				int nextPktFlagPos = locateNextPktFlag();
				currentIndex = nextPktFlagPos;
				dataPacket = null;
				return;
			}
			
			// parse packet length: 2 bytes
			currentIndex += eventIdByteCount;
			int pktLenByteCount = 2; 
			buf = FixLengthBufferCache.allocate(pktLenByteCount);
			copy(buf, pktLenByteCount);
			int pktLen = ByteUtils.decodeUnsignedShort(buf);
			LOG.debug("pktLen = " + pktLen);
			
			// event time: 4 bytes
			String eventTime = "";
			currentIndex += pktLenByteCount;
			int eventTimeByteCount = 4;
			buf = FixLengthBufferCache.allocate(eventTimeByteCount);
			copy(buf, eventTimeByteCount);
			if(!Arrays.equals(buf, QUAD_EMPTY_BYTE_BUF)) {
				eventTime = ByteUtils.decodeTime(buf);
			}
			dataPacket.setEventTime(eventTime);
			LOG.info("eventTime = " + eventTime);
			
			// anlyze packet body
			currentIndex += eventTimeByteCount;
			int pktBodyByteCount = pktLen - eventTimeByteCount - 
					pktLenByteCount - eventIdByteCount - fixPacketHeaderByteCount;
			LOG.info("packetBodyByteCount = " + pktBodyByteCount);
			
			int nextPktFlagPos = locateNextPktFlag();
			
			analyze(eventId, nextPktFlagPos, pktBodyByteCount);
			LOG.debug("========== PACKET ENDED =========");
		} else {
			currentIndex++;
		}
	}

	private final void copy(byte[] buf, int length) {
		byte[] fileByteBuf = bytedLogFile.getFileByteBuf();
		System.arraycopy(fileByteBuf, currentIndex, buf, 0, length);
	}
	
	private void analyze(int eventId, int nextPktFlagPos, int packetBodyByteCount) {
		EventType eventType = null;
		AdditionType at = context.getAdditionTypeMetadata().get(eventId);
		EventTypeId eventTypeId = new EventTypeId();
		eventTypeId.setEventId(eventId);
		// exists additional information
		if(at != null) {
			String additionType = at.getType();
			ByteValueHandler<?> handler = context.getPrimitiveTypeMetadata().get(additionType);
			int start = currentIndex;
			ParamValue paramValue = handler.chooseBlock(start, bytedLogFile.getFileByteBuf());
			byte[] b = new byte[handler.byteCount()];
			copy(b, handler.byteCount());
			Object value = handler.handle(b);
			LOG.debug("additionType = " + additionType + ", additionValue = " + value);
			Addition addition = new Addition();
			addition.setEventId(eventId);
			addition.setType(additionType);
			addition.setValue(value.toString());
			eventTypeId.setAddition(addition);
			dataPacket.setAddition(addition);
			eventType = context.getEventTypeMetadata().get(eventTypeId);
			currentIndex += handler.byteCount();
			dataPacket.addParamValue(paramValue);
		} else {
			eventType = context.getEventTypeMetadata().get(eventTypeId);
		}
		
		String eventName = context.getEventTypeMetadata().get(eventTypeId).getName();
		dataPacket.setEventName(eventName);
		LOG.debug("eventName = " + eventName);
		dataPacket.setEventId(eventId);
		
		int paramCount = eventType.getParamCount();
		for (int i = 0; i < paramCount; i++) {
			FieldInfo fi = eventType.get(i);
			int code = fi.getFieldType().getCode();
			if(code == FieldType.NORMAL.getCode()) {
				analyzeParam(fi);
			} else if(code == FieldType.ARRAY.getCode()) {
				FieldArray fa = fi.getExtra();
				for (int j = 0; j < fa.getLength(); j++) {
					analyzeParam(fi);
				}
			} else if(code == FieldType.POINTER.getCode()) {
				FieldPointer fp = (FieldPointer) fi.getExtra();
				int pointerLen = fi.getByteCount();
				byte[] b = new byte[pointerLen];
				copy(b, pointerLen);
				// is a empty pointer?
				if(isEmptyBuf(b)) {
					currentIndex += pointerLen;
					continue;
				} else {
					// process nested strutct type
					ParamType pt = fp.getParamType();
					processNestedType(eventType, pt, nextPktFlagPos);
				}
			}
		}
		// set data packet end pos
		dataPacket.setEndPos(currentIndex - 1);
		LOG.debug("Data packet detail: eventId = " + dataPacket.getEventId() + 
			", startPos = " + dataPacket.getStartPos() + 
			", endPos = " + dataPacket.getEndPos() + 
			", byteBuf = " + Arrays.toString(dataPacket.getBytes()));
	}

	private void processNestedType(EventType eventType, ParamType pt, int nextPktFlagPos) {
		for (int k = 0; k < pt.getParamCount(); k++) {
			if(bytedLogFile.getFileByteBuf()[currentIndex] == 0
					&& currentIndex + 1 == nextPktFlagPos) {
				LOG.debug("Skip terminate character(a NULL position)...");
				break;
			}
			FieldInfo ptField = pt.get(k);
			analyzeParam(ptField);
		}
	}
	
	private boolean isEmptyBuf(byte[] buf) {
		boolean empty = true;
		for(byte b : buf) {
			if(b != 0) {
				empty = false;
				break;
			}
		}
		return empty;
	}

	private void analyzeParam(FieldInfo fi) {
		ByteValueHandler<?> handler = context.getPrimitiveTypeMetadata().get(fi.getType());
		int start = currentIndex;
		ParamValue paramValue = handler.chooseBlock(start, bytedLogFile.getFileByteBuf());
		paramValue.setFileByteBuffer(bytedLogFile.getFileByteBuf());
		paramValue.setHandler(handler);
		LOG.debug("Param data value: type = " + fi.getType() + ", byteCount = " + fi.getByteCount() + ", value = " + paramValue.valueOf());
		currentIndex += paramValue.getTotalByteCount();
		dataPacket.addParamValue(paramValue);
	}

	@Override
	public boolean hasNext() {
		return currentIndex >= 0 
				&& currentIndex < bytedLogFile.getFileByteBuf().length;
	}

	@Override
	public EventPacket next() {
		try {
			processPacket();
		} catch (Exception e) {
			dataPacket = null;
			LOG.error(e);
		}
		return dataPacket;
	}

	private int locateNextPktFlag() {
		// parse packet flag
		int nextPktFlagPos = -1;
		int fixPacketHeaderByteCount = LEN_PKT_FLAG; // 4 bytes
		byte[] fileBuf = bytedLogFile.getFileByteBuf();
		int len = fileBuf.length - fixPacketHeaderByteCount;
		for (int i = currentIndex; i < len; i++) {
			if(fileBuf[i] == PKT_FLAG_BUF[0] 
					&& fileBuf[i + 1] == PKT_FLAG_BUF[1]
					&& fileBuf[i + 2] == PKT_FLAG_BUF[2]
					&& fileBuf[i + 3] == PKT_FLAG_BUF[3]) {
				// packet flag found
				nextPktFlagPos = i;
				break;
			}
		}
		return nextPktFlagPos;
	}

	@Override
	public void remove() {
		throw new RuntimeException("Unsupported operation!");		
	}

}
