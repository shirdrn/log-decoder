package org.shirdrn.log.decoder;

import org.shirdrn.log.decoder.common.Addition;
import org.shirdrn.log.decoder.common.Context;
import org.shirdrn.log.decoder.common.EventPacket;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;

public class EventDataPacket implements EventPacket {

	private int eventId = - Integer.MAX_VALUE;
	private String eventName = "";
	private String eventTime = "";
	private final byte[] fullByteBuffer;
	private int startPos;
	private int endPos;
	private ParamValue linkedListHeader;
	private ParamValue currentNode;
	private Addition addition;
	
	public EventDataPacket(Context context) {
		this.fullByteBuffer = context.getFileByteBuf();
	}

	@Override
	public int getEventId() {
		return eventId;
	}

	@Override
	public void setEventId(int eventId) {
		this.eventId = eventId;
	}
	
	@Override
	public String getEventName() {
		return eventName;
	}

	@Override
	public void setEventName(String eventName) {
		this.eventName = eventName;		
	}
	
	@Override
	public String getEventTime() {
		return eventTime;
	}

	@Override
	public void setEventTime(String eventTime) {
		this.eventTime = eventTime;		
	}
	
	@Override
	public byte[] getBytes() {
		int len = endPos - startPos + 1;
		byte[] buf = new byte[len];
		System.arraycopy(fullByteBuffer, startPos, buf, 0, len);
		return buf;
	}
	
	@Override
	public ParamValue getParamHeader() {
		return linkedListHeader;
	}
	
	@Override
	public void addParamValue(ParamValue paramValue) {
		if(linkedListHeader == null) {
			linkedListHeader = paramValue;
			currentNode = linkedListHeader;
		} else {
			currentNode.setNext(paramValue);
			currentNode = paramValue;
		}
	}
	
	@Override
	public int getStartPos() {
		return startPos;
	}

	@Override
	public void setStartPos(int startPos) {
		this.startPos = startPos;
	}
	
	@Override
	public int getEndPos() {
		return endPos;
	}

	@Override
	public void setEndPos(int endPos) {
		this.endPos = endPos;
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
		.append("eventId = ").append(eventId).append(", ")
		.append("startPos = ").append(startPos).append(", ")
		.append("endPos = ").append(endPos)
		.append("]");
		return sb.toString();
	}
	
	
	public static class ParamValue {
		
		private ByteValueHandler<?> handler;
		private byte[] fileByteBuffer;
		private String type;
		private int start;
		private int end;
		private int totalByteCount;
		private ParamValue next;
		private boolean existsTerminateChar;
		
		public ParamValue(String type, int start, int end) {
			this.type = type;
			this.start = start;
			// inlucdes '\0' terminate character
			this.end = end;
		}
		
		public Object valueOf() {
			byte[] buf = new byte[totalByteCount];
			System.arraycopy(fileByteBuffer, start, buf, 0, totalByteCount);
			return handler.handle(buf);
		}
		
		public void setNext(ParamValue next) {
			this.next = next;
		}
		
		public ParamValue getNext() {
			return this.next;
		}

		public String getType() {
			return type;
		}

		public int getStart() {
			return start;
		}

		public int getEnd() {
			return end;
		}

		public int getTotalByteCount() {
			return totalByteCount;
		}

		public void setTotalByteCount(int totalByteCount) {
			this.totalByteCount = totalByteCount;
		}
		
		public boolean isExistsTerminateChar() {
			return existsTerminateChar;
		}

		public void setExistsTerminateChar(boolean existsTerminateChar) {
			this.existsTerminateChar = existsTerminateChar;
		}
		
		@Override
		public String toString() {
			StringBuffer sb = new StringBuffer();
			sb.append("[")
			.append("type = ").append(type).append(", ")
			.append("start = ").append(start).append(", ")
			.append("end = ").append(end).append(", ")
			.append("totalByteCount = ").append(totalByteCount)
			.append("]");
			return sb.toString();
		}

		public void setFileByteBuffer(byte[] fileByteBuffer) {
			this.fileByteBuffer = fileByteBuffer;
		}

		public void setHandler(ByteValueHandler<?> handler) {
			this.handler = handler;
		}
	}


	@Override
	public void setAddition(Addition addition) {
		this.addition = addition;		
	}

	@Override
	public Addition getAddition() {
		return addition;
	}


}
