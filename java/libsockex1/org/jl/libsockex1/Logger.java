package org.jl.libsockex1;

public class Logger {
	public static LoggerFactory factory;

	protected Logger(String name) {}
	public void log(Level level, String tag, String msg) {}
	public void v(String tag, String msg) {log(Level.VERBOSE, tag, msg);}
	public void d(String tag, String msg) {log(Level.DEBUG, tag, msg);}
	public void i(String tag, String msg) {log(Level.INFO, tag, msg);}
	public void e(String tag, String msg) {log(Level.ERROR, tag, msg);}

	public static Logger getInstance(String name) {
		return factory != null ? factory.getInstance(name) : new Logger(name);
	}

	public static Logger.Level logLevel() {
		return factory != null ? factory.logLevel() : Level.INFO;
	}

	public static void logLevel(Logger.Level level) {
		if (factory != null) factory.logLevel();
	}

	public static enum Level {
		ERROR, INFO, DEBUG, VERBOSE;

		public Level verbose(int inc) {
			Level[] arr = values();
			int val = ordinal();
			while (inc > 0) {
				if (val >= arr.length) break;
				val++;
				inc--;
			}
			while (inc < 0) {
				if (val <= 0) break;
				val--;
				inc++;
			}
			return arr[val];
		}
	}
}
