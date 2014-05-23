package org.shirdrn.log.decoder.common;


public final class EventType extends ParamType {

	private boolean additionExisted = false;
	private EventType next;
	private Addition addition;
	
	public EventType(int eventId, String eventName) {
		super(eventId, eventName);
	}
	
	public boolean isAdditionExisted() {
		return additionExisted;
	}

	public void setAdditionExisted(boolean additionExisted) {
		this.additionExisted = additionExisted;
	}

	public Addition getAddition() {
		return addition;
	}

	public void setAddition(Addition addition) {
		this.addition = addition;
	}

	public EventType getNext() {
		return next;
	}

	public void setNext(EventType next) {
		this.next = next;
	}
	
	@Override
	public int hashCode() {
		int hc = id * 31;
		if(additionExisted) {
			hc += 31 * addition.getValue().hashCode();
		}
		return hc;
	}
	
	@Override
	public boolean equals(Object obj) {
		EventType other = (EventType) obj;
		boolean eq = this.id == other.id;
		if(additionExisted) {
			eq = eq && this.addition.getValue().equals(other.addition.getValue());
		}
		return  eq;
	}

	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
			.append("eventId=").append(id).append(", ")
			.append("eventName=").append(name).append(", ")
			.append("paramCount=").append(paramCount).append(", ")
			.append("fieldInfos=").append(fieldInfos);
		if(addition != null) {
			sb.append(",").append("addition=").append(addition);
		}
		sb.append("]");
		return sb.toString();
	}
	
}

