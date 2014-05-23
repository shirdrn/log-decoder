package org.shirdrn.log.decoder.common;


public class Addition extends AdditionType {

	private String value;
	
	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}
	
	@Override
	public int hashCode() {
		return super.hashCode() + 31 * value.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		Addition other = (Addition) obj;
		return super.equals(other)
				&& other.value.equals(this.value);
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
			.append("eventId=").append(eventId).append(",")
			.append("type=").append(type).append(",")
			.append("value=").append(value)
			.append("]");
		return sb.toString();
	}
}
