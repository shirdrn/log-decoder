package org.shirdrn.log.decoder.common;


public interface LogDecoder {

	void setVersion(String version) throws Exception;
	void loadFile();
	void decode();
	
}
