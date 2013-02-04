public class HelloUJ {
	/**
	 * @param args
	 */
	public static void main() {
		// TODO Auto-generated method stub
		int i = 3;
		boolean flag = true;
		while(true) {
			UC.toggleLED(-1);
			UC.msDelay(500);
			UC.SetDigitalOutLevel(i, flag?1:0);
			if (i++>5) {
				i = 3;
				flag = !flag;
			}
			UC.msDelay(500);
		}

	}
}
