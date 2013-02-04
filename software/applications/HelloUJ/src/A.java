public class A {
	/**
	 * @param args
	 */
	public static void main() {
		// TODO Auto-generated method stub
		while(true) {
			UC.toggleLED(1);
			UC.msDelay(1000);
			UC.toggleLED(0);
			UC.msDelay(1000);
		}

	}
}
