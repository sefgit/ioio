public class B {
	/**
	 * @param args
	 */
	public static void main() {
		// TODO Auto-generated method stub
		while(true) {
			UC.toggleLED(-1);
			for(int i=10;i>0;--i)
				for(int j=1000;j>0;--j)
					UC.msDelay(1);
		}

	}
}
