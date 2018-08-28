package org.jl.libsockex1;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.SocketAddress;

public abstract class Connection extends Looper {
	private static Class<?> clz = Connection.class;
	private static Logger logger = Logger.getInstance(clz.getSimpleName());
	protected static String TAG = clz.getSimpleName();
	protected Socket socket;
	protected SocketAddress connAddr;
	protected InputStream ist;
	protected OutputStream ost;
	protected byte[] recvBuf;

	public Connection(Socket socket) {this.socket = socket;}
	public Connection(SocketAddress connAddr) {this.connAddr = connAddr;}

	@Override
	protected void onCreate() {}

	@Override
	protected void onStart() throws Exception {
		if (socket == null) {
			socket = new Socket();
			logger.i(TAG, String.format("Connecting to %s", connAddr));
			socket.connect(connAddr);
			logger.i(TAG, String.format("Connected to %s", connAddr));
		}
		ist = socket.getInputStream();
		ost = socket.getOutputStream();
		if (recvBuf == null) recvBuf = new byte[2000];
	}

	@Override
	protected void onStop() {
		try {
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void onLoop() throws Exception {
		int recvLen = ist.read(recvBuf);
		if (recvLen <= 0) {
			onDisconnect();
			return;
		}
		onReceive(recvBuf, 0, recvLen);
	}

	protected void onDisconnect() {
		logger.i(TAG, String.format("Disconnected %s",
				socket.getRemoteSocketAddress()));
		try {
			close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	protected void onReceive(byte[] data, int pos, int len)
			throws IOException {
		String recvStr = new String(data, pos, len);
		logger.i(TAG, String.format("Received '%s'", recvStr));
	}
}