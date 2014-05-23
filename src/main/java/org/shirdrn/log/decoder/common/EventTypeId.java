package org.shirdrn.log.decoder.common;


public class EventTypeId {

	private int eventId;
	private Addition addition;
	
	public int getEventId() {
		return eventId;
	}
	
	public void setEventId(int eventId) {
		this.eventId = eventId;
	}
	
	public Addition getAddition() {
		return addition;
	}
	
	public void setAddition(Addition addition) {
		this.addition = addition;
	}
	
	@Override
	public int hashCode() {
		int hc = eventId * 31;
		if(addition != null) {
			hc += 31 * addition.getValue().hashCode();
		}
		return hc;
	}
	
	@Override
	public boolean equals(Object obj) {
		EventTypeId other = (EventTypeId) obj;
		boolean eq = (this.eventId == other.eventId);
		if(addition != null) {
			eq = eq && (this.addition.getValue().equals(other.addition.getValue()));
		}
		return eq;
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
			.append("eventId=").append(eventId);
		if(addition != null) {
			sb.append(", ").append("addition=").append(addition.getValue());
		}
		sb.append("]");
		return sb.toString();
	}
	
}
