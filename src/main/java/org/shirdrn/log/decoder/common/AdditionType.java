package org.shirdrn.log.decoder.common;


public class AdditionType {

	protected int eventId;
	protected String type;
	
	public int getEventId() {
		return eventId;
	}
	public void setEventId(int eventId) {
		this.eventId = eventId;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	
	@Override
	public int hashCode() {
		return 31 * eventId + 31 * type.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		AdditionType other = (AdditionType) obj;
		return other.eventId == this.eventId 
				&& other.type.equals(this.type);
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[eventId=").append(eventId).append(", ")
		.append("type=").append(type).append("]");
		return sb.toString();
	}
}
