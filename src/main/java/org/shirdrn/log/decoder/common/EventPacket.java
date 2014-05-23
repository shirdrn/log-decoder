package org.shirdrn.log.decoder.common;

import org.shirdrn.log.decoder.EventDataPacket.ParamValue;

public interface EventPacket {

	int getEventId();
	void setEventId(int eventId);
	String getEventName();
	void setEventName(String eventName);
	String getEventTime();
	void setEventTime(String eventTime);
	
	byte[] getBytes();
	
	int getStartPos();
	void setStartPos(int startPos);
	int getEndPos();
	void setEndPos(int endPos);
	
	void setAddition(Addition addition);
	Addition getAddition();
	
	ParamValue getParamHeader();
	void addParamValue(ParamValue paramValue);
}
