package org.jl.sockex1;

import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import org.jl.libsockex1.Echo;
import org.jl.libsockex1.Heartbeat;
import org.jl.libsockex1.Logger;
import org.jl.libsockex1.Server;

public class Sockex1 {
	private static Class<?> clz = Sockex1.class;
	private static Logger logger = new LoggerFactoryImpl().getLogger(
			clz.getSimpleName());
	protected static String TAG = clz.getSimpleName();

	public static void main(String[] args) {
		boolean showHelp = false;
		String connSvr = null;
		int connSvrPort = -1;

		for (int i = 0; i < args.length; i++) {
			if ("-h".equals(args[i]) || "--help".equals(args[i])) {
				showHelp = true;
				continue;
			}
			if ("-v".equals(args[i]) || "--verbose".equals(args[i])) {
				Logger.logLevel(Logger.logLevel().verbose(1));
				continue;
			}
			if (connSvr == null) connSvr = args[i];
			else if (connSvrPort < 0) connSvrPort = Integer.parseInt(args[i]);
		}

		if (showHelp) {
			showHelp();
			return;
		}

		if (connSvr == null) {
			new Thread(new Server() {
				@Override
				protected void onAccept(Socket clientSocket) {
					logger.i(TAG, String.format("Accept %s",
							clientSocket.getRemoteSocketAddress()));
					new Thread(new Echo(clientSocket)).start();
				}
			}).start();
			return;
		}

		SocketAddress svrAddr;
		if (connSvrPort < 0) {
			String[] tok = connSvr.split(":");
			if (tok.length != 2) {
				logger.e(TAG, String.format("Invalid server address"));
				showHelp();
				return;
			}
			connSvr = tok[0];
			connSvrPort = Integer.parseInt(tok[1]);
		}
		svrAddr = new InetSocketAddress(connSvr, connSvrPort);
		new Thread(new Heartbeat(svrAddr)).start();
	}

	public static void showHelp() {
		System.out.println("\n"
		+ "USAGE\n"
		+ "  COMMAND [-h | --help] [SERVER]\n"
		+ "\n"
		+ "OPTIONS\n"
		+ "  SERVER=IP:PORT  Connect to server address\n");
	}
}
