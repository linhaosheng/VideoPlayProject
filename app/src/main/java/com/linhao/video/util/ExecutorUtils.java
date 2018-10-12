package com.linhao.video.util;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ExecutorUtils {

	static ExecutorService cachedThreadPool = Executors.newCachedThreadPool();
	
	public static void execute(Runnable run){
		
		if(run!=null){
			cachedThreadPool.execute(run);
		}
	}
}
