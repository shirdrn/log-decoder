package org.shirdrn.log.decoder.metadata;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.common.ArrayType;
import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.common.ParamType;
import org.shirdrn.log.decoder.common.TypeParser;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;
import org.shirdrn.log.decoder.utils.ParserUtils;

public class ParamTypeMetadata extends AbstractMetadata<Integer, ParamType> {

	private static final Log LOG = LogFactory.getLog(ParamTypeMetadata.class);
	private static final Metadata<Integer, ParamType> INSTANCE = new ParamTypeMetadata();
	private static final String EVENT_PARAM_TYPE_FILE = "param_types";
	
	private Metadata<String, ByteValueHandler<?>> primitiveTypeMetadata;
	
	private ParamTypeMetadata() {
		super(EVENT_PARAM_TYPE_FILE);
	}
	
	public static Metadata<Integer, ParamType> newInstance() {
		return INSTANCE;
	}
	
	@Override
	protected void initialize() {
		this.container = new HashMap<Integer, ParamType>();	
		typeParser = new ParamTypeParser();
		primitiveTypeMetadata = PrimitiveTypeMetadata.newInstance();
	}
	
	@Override
	protected void load(String file) {
		LOG.info("Load event param type configuration...");
		InputStream in = null;
		BufferedReader reader = null;
		try {
			in = this.getClass().getClassLoader().getResourceAsStream(file);
			reader = new BufferedReader(new InputStreamReader(in));
			String line = null;
			while((line = reader.readLine()) != null) {
				line = line.trim();
				if(line.length() > 0 && !line.startsWith("#")) {
					String[] a = line.split("\\s*,\\s*");
					if(ParserUtils.isValid(a, 4)) {
						int id = Integer.parseInt(a[0]);
						String name = a[1].toUpperCase();
						int paramCount = Integer.parseInt(a[2]);
						if(a.length == paramCount + 3) {
							ParamType paramType = container.get(id);
							if(paramType == null) {
								paramType = typeParser.parse(a, id, name, 3, paramCount);
								container.put(id, paramType);
							} else {
								LOG.warn("Duplicated param type: id = " + id + ", name = " + name);
							}
						} else {
							LOG.warn("Bad line format: line = " + line);
						}
					} else {
						LOG.warn("Bad line format: line = " + line);
					}
				}
			}
		} catch (IOException e) {
			LOG.error("Fail to read file " + file, e);
		} finally {
			if(reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					LOG.error(e);
				}
			}
		}
		LOG.debug("EVent param type metadata: " + container);		
	}
	
	@Override
	public ParamType get(Integer key) {
		return container.get(key);
	}
	
	class ParamTypeParser implements TypeParser<ParamType> {

		@Override
		public ParamType parse(String[] a, int id, String name, int fromIndex, int paramCount) {
			ParamType paramType = new ParamType(id, name.toUpperCase());
			paramType.setParamCount(paramCount);
			for(int i = fromIndex; i < a.length; i++) {
				String type = a[i].toLowerCase();
				if(type.indexOf('[') != -1) {
					ArrayType at = ArrayType.parseArrayType(type);
					paramCount += (at.getLength() - 1);
					paramType.setParamCount(paramCount);
					for (int j = 0; j < at.getLength(); j++) {
						ByteValueHandler<?> handler = primitiveTypeMetadata.get(at.getType());
						paramType.addField(at.getType(), handler.byteCount());
					}
				} else {
					ByteValueHandler<?> handler = primitiveTypeMetadata.get(type);
					paramType.addField(type, handler.byteCount());
				}
			}
			return paramType;
		}
		
	}

	@Override
	public Iterator<Entry<Integer, ParamType>> iterator() {
		return container.entrySet().iterator();
	}

}
