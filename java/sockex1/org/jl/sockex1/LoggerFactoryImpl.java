package org.jl.sockex1;

import java.util.Date;

import org.jl.libsockex1.Logger;

public class LoggerFactoryImpl implements Logger.Factory {
	Logger.Level logLevel = Logger.Level.INFO;

	public LoggerFactoryImpl() {
		Logger.factory = this;
	}

	@Override
	public Logger getLogger(String name) {
		return new Logger(name) {
			@Override
			public void log(Logger.Level level, String tag, String msg) {
				if (logLevel.compareTo(level) < 0) return;
				String outMsg = String.format("%s %s %s %s",
						new Date(), level, tag, msg);
				(Logger.Level.ERROR.compareTo(level) >= 0 ? System.err : System.out)
						.println(outMsg);
			}
		};
	}

	@Override
	public Logger.Level logLevel() {return logLevel;}

	@Override
	public void logLevel(Logger.Level level) {logLevel = level;}

}
