package org.shirdrn.log.decoder.common;

import java.util.LinkedHashMap;

import org.shirdrn.log.decoder.common.handler.ByteValueHandler;

public class ParamType {

	protected final int id;
	protected String name;
	protected final LinkedHashMap<Integer, FieldInfo> fieldInfos;
	protected int paramCount;
	private int currentIndex;
	
	public ParamType(int id, String name) {
		this.id = id;
		this.name = name;
		this.currentIndex = 0;
		fieldInfos = new LinkedHashMap<Integer, FieldInfo>();
	}
	
	public void addField(String type, int byteCount) {
		addField(type, byteCount, FieldType.NORMAL, null);
	}
	
	public void addField(String type, int byteCount, FieldType ft) {
		addField(type, byteCount, ft, null);
	}
	
	public void addField(String type, int byteCount, 
			FieldType ft, FieldArray fa) {
		FieldInfo fi = new FieldInfo(type, byteCount);
		fieldInfos.put(currentIndex++, fi);
		fi.setFieldType(ft);
		fi.setExtra(fa);
	}
	
	public FieldInfo get(int index) {
		return fieldInfos.get(index);
	}
	
	public int getParamCount() {
		return paramCount;
	}

	public void setParamCount(int paramCount) {
		this.paramCount = paramCount;
	}

	public int getId() {
		return id;
	}

	public String getName() {
		return name;
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
			.append("id=").append(id).append(", ")
			.append("name=").append(name).append(", ")
			.append("fieldInfos=").append(fieldInfos)
			.append("]");
		return sb.toString();
	}
	

	public static class FieldArray {
		
		protected ByteValueHandler<?> handler;
		protected int length;
		
		public ByteValueHandler<?> getHandler() {
			return handler;
		}
		
		public void setHandler(ByteValueHandler<?> handler) {
			this.handler = handler;
		}
		
		public int getLength() {
			return length;
		}
		
		public void setLength(int length) {
			this.length = length;
		}
		
		@Override
		public String toString() {
			StringBuffer sb = new StringBuffer();
			sb.append("[")
				.append("handler=").append(handler.getClass().getSimpleName().toLowerCase()).append(", ")
				.append("length=").append(length)
				.append("]");
			return sb.toString();
		}
	}
	
	public static class FieldPointer extends FieldArray {
		
		private ParamType paramType;

		public ParamType getParamType() {
			return paramType;
		}

		public void setParamType(ParamType paramType) {
			this.paramType = paramType;
		}
		
		@Override
		public String toString() {
			StringBuffer sb = new StringBuffer();
			sb.append("[")
				.append("handler=").append(handler.getClass().getSimpleName().toLowerCase()).append(", ")
				.append("length=").append(length);
			if(paramType != null) {
				sb.append(",").append("paramType=").append(paramType);
			}
			sb.append("]");
			return sb.toString();
		}
	}
	
	public static class FieldInfo {
		
		private String type;
		private int byteCount;
		private FieldType fieldType = FieldType.NORMAL;
		private FieldArray extra;
		
		public FieldArray getExtra() {
			return extra;
		}
		public void setExtra(FieldArray extra) {
			this.extra = extra;
		}
		public FieldInfo(String type, int byteCount) {
			super();
			this.type = type;
			this.byteCount = byteCount;
		}
		public String getType() {
			return type;
		}
		public int getByteCount() {
			return byteCount;
		}
		
		public FieldType getFieldType() {
			return fieldType;
		}
		public void setFieldType(FieldType fieldType) {
			this.fieldType = fieldType;
		}
		@Override
		public String toString() {
			StringBuffer sb = new StringBuffer();
			sb.append("[")
				.append("type=").append(type).append(", ")
				.append("bytes=").append(byteCount);
				if(extra != null) {
					sb.append(", ").append("extra=").append(extra);
				}
				
				sb.append("]");
			return sb.toString();
		}
	}

	public void setName(String name) {
		this.name = name;
	}
}
