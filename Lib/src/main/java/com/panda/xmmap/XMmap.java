package com.panda.xmmap;

/**
 * @author panda
 * created at 2021/6/25 11:15 AM
 */
public class XMmap {
	private final long nativePtr;

	static {
		System.loadLibrary("xmmap");
	}

	public XMmap(String filePath) {
		this(filePath, false);
	}
	public XMmap(String filePath, boolean append) {
		nativePtr = nativeInit(filePath, append);
	}
	public boolean write(byte[] data) {
		return nativeWrite(nativePtr, data);
	}
	public byte[] read() {
		return nativeRead(nativePtr);
	}
	public boolean close() {
		return nativeClose(nativePtr);
	}

	private static native long nativeInit(String filePath, boolean append);
	private static native boolean nativeWrite(long nativePtr, byte[] data);
	private static native byte[] nativeRead(long nativePtr);
	private static native boolean nativeClose(long nativePtr);
}
