package org.shirdrn.log.decoder;

import java.io.File;

import org.junit.Before;
import org.junit.Test;
import org.shirdrn.log.decoder.LogDecoderImpl;
import org.shirdrn.log.decoder.common.LogDecoder;

public class TestLogDecoderImpl {

	LogDecoder decoder;
	
	@Before
	public void initialize() {
		String file = "E:\\Documents\\20130528104859.bin";
		decoder = new LogDecoderImpl(new File(file));
	}
	
	@Test
	public void decode() {
		decoder.loadFile();
		decoder.decode();
	}
}
