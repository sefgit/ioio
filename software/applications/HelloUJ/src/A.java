public class A {
	/**
	 * @param args
	 */
	public static void main() {
		// TODO Auto-generated method stub
		while(true) {
			UC.toggleLED(1);
			for(int i=1000;i>0;--i)
				UC.msDelay(1);
			UC.toggleLED(0);
			for(int i=1000;i>0;--i)
				UC.msDelay(1);
		}

	}
}
