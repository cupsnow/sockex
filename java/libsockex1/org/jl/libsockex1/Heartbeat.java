package org.jl.libsockex1;

import java.io.IOException;
import java.net.SocketAddress;
import java.util.Date;

public class Heartbeat extends Connection {
	private static Class<?> clz = Heartbeat.class;
	private static Logger logger = Logger.getInstance(clz.getSimpleName());
	protected long heartbeatInterval = 60000 / 65;
	protected Looper sender;

	public Heartbeat(SocketAddress connAddr) {
		super(connAddr);
		TAG = "Heartbeat";
	}

	@Override
	protected void onStart() throws Exception {
		super.onStart();
		TAG = String.format("Heartbeat(%s)", socket.getLocalSocketAddress());

		new Thread(sender = new Looper() {
			@Override
			protected void onCreate() {}

			@Override
			protected void onStart() throws Exception {}

			@Override
			protected void onStop() {
				try {
					Heartbeat.this.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

			@Override
			protected void onLoop() throws Exception {
				onHeartbeat();
				Thread.sleep(heartbeatInterval);
			}
		}).start();
	}

	@Override
	protected void onStop() {
		try {
			sender.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		super.onStop();
	}

	@Override
	protected void onDisconnect() {
		try {
			close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	protected void onHeartbeat() throws IOException {
		String msg = String.format("%s", new Date());
		logger.i(TAG, String.format("Send '%s'", msg));
		ost.write(msg.getBytes());

		logger.d(TAG, String.format("Sleeping %sms", heartbeatInterval));
	}

}