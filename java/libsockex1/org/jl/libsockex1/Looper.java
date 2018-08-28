package org.jl.libsockex1;

import java.io.Closeable;
import java.io.IOException;

public abstract class Looper implements Runnable, Closeable {
	protected boolean closed;

	public Looper() {onCreate();}

	@Override
	public void close() throws IOException {closed = true;}

	@Override
	public void run() {
		try {
			onStart();
			while (!closed && !Thread.interrupted()) onLoop();
		} catch(Exception e) {
			e.printStackTrace();
		}
		onStop();
	}

	protected abstract void onCreate();
	protected abstract void onStart() throws Exception;
	protected abstract void onStop();
	protected abstract void onLoop() throws Exception;

}
