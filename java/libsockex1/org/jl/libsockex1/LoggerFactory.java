package org.jl.libsockex1;

public interface LoggerFactory {
	public Logger getInstance(String name);
	public Logger.Level logLevel();
	public void logLevel(Logger.Level level);
}
