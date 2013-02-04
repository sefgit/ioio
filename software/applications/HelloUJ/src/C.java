public class C {
	/**
	 * @param args
	 */
	public static void main() {
		// TODO Auto-generated method stub
		int i = 3;
		boolean flag = true;
		UC.toggleLED(1);
		while(true) {
			UC.SetDigitalOutLevel(i, flag?1:0);
			UC.msDelay(1000);
			UC.msDelay(1000);
			UC.SetDigitalOutLevel(i, 0);
			if (i++>5) {
				i = 3;
				flag = !flag;
				UC.toggleLED(-1);
			}
		}

	}
}
