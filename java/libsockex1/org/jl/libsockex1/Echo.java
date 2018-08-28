package org.jl.libsockex1;

import java.io.IOException;
import java.net.Socket;

public class Echo extends Connection {
	public Echo(Socket socket) {
		super(socket);
		TAG = String.format("Echo(%s)", socket.getRemoteSocketAddress());
	}

	@Override
	protected void onReceive(byte[] data, int pos, int len)
			throws IOException {
		super.onReceive(data, pos, len);
		ost.write(data, pos, len);
	}
}