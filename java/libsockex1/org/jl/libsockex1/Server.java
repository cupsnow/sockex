package org.jl.libsockex1;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends Looper {
	private static Class<?> clz = Server.class;
	private static Logger logger = Logger.getInstance(clz.getSimpleName());
	protected static String TAG = clz.getSimpleName();
	protected ServerSocket socket;

	@Override
	protected void onCreate() { }

	@Override
	protected void onStart() throws Exception {
		socket = new ServerSocket(0, 10);
		logger.i(TAG, String.format("Start server %s",
				socket.getLocalSocketAddress()));
	}

	@Override
	protected void onStop() {
		try {
			logger.i(TAG, String.format("Stop server %s",
					socket.getLocalSocketAddress()));
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void onLoop() throws Exception {
		Socket clientSocket = socket.accept();
		onAccept(clientSocket);
	}

	protected void onAccept(Socket clientSocket) {}
}