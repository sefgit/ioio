package ioio.examples.hello;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;

import ioio.lib.api.DigitalOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

/**
 * This is the main activity of the HelloIOIO example application.
 * 
 * It displays a toggle button on the screen, which enables control of the
 * on-board LED. This example shows a very simple usage of the IOIO, by using
 * the {@link IOIOActivity} class. For a more advanced use case, see the
 * HelloIOIOPower example.
 */
public class MainActivity extends IOIOActivity implements OnClickListener {
	private ToggleButton btnA;
	private ToggleButton btnB;
	private ToggleButton btnC;
	private ToggleButton btnD;
	private ToggleButton buttonF;
	private ToggleButton buttonFL;
	private ToggleButton buttonFR;
	private ToggleButton buttonL;
	private ToggleButton buttonO;
	private ToggleButton buttonR;
	private ToggleButton buttonBL;
	private ToggleButton buttonB;
	private ToggleButton buttonBR;
	private final int REQUEST_ENABLE_BT = 202;
	private byte[] buffer = null;
	private int maxsize = 0;
	private boolean loaded = false;
	private boolean reset = false;
	private Context ctx = null;

	
	/**
	 * Called when the activity is first created. Here we normally initialize
	 * our GUI.
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		btnA = (ToggleButton) findViewById(R.id.ToggleButtonA);
		btnA.setOnCheckedChangeListener(listener);
		btnA.setOnClickListener(this);
		btnB = (ToggleButton) findViewById(R.id.ToggleButtonB);
		btnB.setOnClickListener(this);
		btnB.setOnCheckedChangeListener(listener);
		btnC = (ToggleButton) findViewById(R.id.ToggleButtonC);
		btnC.setOnClickListener(this);
		btnC.setOnCheckedChangeListener(listener);
		btnD = (ToggleButton) findViewById(R.id.ToggleButtonD);
		btnD.setOnClickListener(this);
		btnD.setOnCheckedChangeListener(listener);
		buttonF = (ToggleButton) findViewById(R.id.tbF);
		buttonF.setOnCheckedChangeListener(listener);
		buttonFL = (ToggleButton) findViewById(R.id.tbFL);
		buttonFL.setOnCheckedChangeListener(listener);
		buttonFR = (ToggleButton) findViewById(R.id.tbFR);
		buttonFR.setOnCheckedChangeListener(listener);
		buttonL = (ToggleButton) findViewById(R.id.tbL);
		buttonL.setOnCheckedChangeListener(listener);
		buttonO = (ToggleButton) findViewById(R.id.tbO);
		buttonR = (ToggleButton) findViewById(R.id.tbR);
		buttonR.setOnCheckedChangeListener(listener);
		buttonBL = (ToggleButton) findViewById(R.id.tbBL);
		buttonBL.setOnCheckedChangeListener(listener);
		buttonB = (ToggleButton) findViewById(R.id.tbB);
		buttonB.setOnCheckedChangeListener(listener);
		buttonBR = (ToggleButton) findViewById(R.id.tbBR);
		buttonBR.setOnCheckedChangeListener(listener);
		ctx = getBaseContext();
        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter != null) {
        	if (!mBluetoothAdapter.isEnabled()) {
        	    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        	    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        	}
        }	        
		
	}

	/**
     * Implemented method - onClick.
     * Show the user a piece of toast. 
     */
	@Override
	public void onClick(View v) {
		String filename = null;
		switch(v.getId()) {
			case R.id.ToggleButtonA: filename = "A"; break;
			case R.id.ToggleButtonB: filename = "B"; break;
			case R.id.ToggleButtonC: filename = "C"; break;
			case R.id.ToggleButtonD: 
				reset = true; 
				break;
		}
		if (filename!=null) {
			// Create a piece of toast.
			Toast pieceToast = Toast.makeText(getApplicationContext(), "uj loading...", Toast.LENGTH_SHORT);
			try {
				File sdDir = Environment.getExternalStorageDirectory();
			    FileInputStream fis = new FileInputStream(sdDir.getAbsolutePath() + "/ioio/"+filename+".class");
				buffer = new byte[1024];
				maxsize = fis.read(buffer, 0, 1024);
				fis.close();
				pieceToast = Toast.makeText(getApplicationContext(), "uj loading..." + maxsize, Toast.LENGTH_SHORT);
				filename=null;
				loaded = true;
			} catch (Exception e) {
				Toast.makeText(getBaseContext(), e.getMessage(),
						Toast.LENGTH_SHORT).show();
				pieceToast = Toast.makeText(getApplicationContext(), "uj loading failed...", Toast.LENGTH_SHORT);
			}
			pieceToast.show();
		}
	}
				
				
				
	void checkB() {
		if (buttonBL.isChecked()) { buttonBL.setChecked(false); }
		if (buttonB.isChecked()) { buttonB.setChecked(false); }
		if (buttonBR.isChecked()) { buttonBR.setChecked(false); }		
	}
	void checkF() {
		if (buttonFL.isChecked()) { buttonFL.setChecked(false); }
		if (buttonF.isChecked()) { buttonF.setChecked(false); }
		if (buttonFR.isChecked()) { buttonFR.setChecked(false); }		
	}
	void checkL() {
		if (buttonFL.isChecked()) { buttonFL.setChecked(false); }
		if (buttonL.isChecked()) { buttonL.setChecked(false); }
		if (buttonBL.isChecked()) { buttonBL.setChecked(false); }		
	}
	void checkR() {
		if (buttonFR.isChecked()) { buttonFR.setChecked(false); }
		if (buttonR.isChecked()) { buttonR.setChecked(false); }
		if (buttonBR.isChecked()) { buttonBR.setChecked(false); }		
	}
	
	 OnCheckedChangeListener listener = new OnCheckedChangeListener() {
	        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {	        	
	        	if (isChecked) {
	        		switch (buttonView.getId()) {
	        		case R.id.tbF:
		        		checkB();
		        		break;
	        		case R.id.tbFR:
	        			checkB();
	        			checkL();
	        			break;
	        		case R.id.tbFL:
	        			checkB();
	        			checkR();
	        			break;
	        		case R.id.tbB:
	        			checkF();
	        			break;
	        		case R.id.tbBR:
	        			checkF(); 
	        			checkL();
	        			break;
	        		case R.id.tbBL:
	        			checkF();
	        			checkR();
	        			break;	        				
	        		case R.id.tbR:
	        			checkL();
	        			break;
	        		case R.id.tbL:
	        			checkR();
	        			break;
	        		case R.id.ToggleButtonA:
	        		case R.id.ToggleButtonB:
	        		case R.id.ToggleButtonC:
	        		case R.id.ToggleButtonD:
	        			if (buttonView != btnA) 
	        				btnA.setChecked(false);
	        			if (buttonView != btnB) 
	        				btnB.setChecked(false);
	        			if (buttonView != btnC) 
	        				btnC.setChecked(false);
	        			if (buttonView != btnD) 
	        				btnD.setChecked(false);
	        			break;
	        		}
	        	}
	        }	        
	  };
	/**
	 * This is the thread on which all the IOIO activity happens. It will be run
	 * every time the application is resumed and aborted when it is paused. The
	 * method setup() will be called right after a connection with the IOIO has
	 * been established (which might happen several times!). Then, loop() will
	 * be called repetitively until the IOIO gets disconnected.
	 */
	class Looper extends BaseIOIOLooper {
		/** The on-board LED. */
		private DigitalOutput led_;
		private DigitalOutput fw_;
		private DigitalOutput bw_;
		private DigitalOutput lf_;
		private DigitalOutput rg_;

		/**
		 * Called every time a connection with IOIO has been established.
		 * Typically used to open pins.
		 * 
		 * @throws ConnectionLostException
		 *             When IOIO connection is lost.
		 * 
		 * @see ioio.lib.util.AbstractIOIOActivity.IOIOThread#setup()
		 */
		@Override
		protected void setup() throws ConnectionLostException {
			led_ = ioio_.openDigitalOutput(0, true);
			lf_ = ioio_.openDigitalOutput(3,true);
			rg_ = ioio_.openDigitalOutput(4,true);
			fw_ = ioio_.openDigitalOutput(5,true);
			bw_ = ioio_.openDigitalOutput(6,true);
		}

		/**
		 * Called repetitively while the IOIO is connected.
		 * 
		 * @throws ConnectionLostException
		 *             When IOIO connection is lost.
		 * 
		 * @see ioio.lib.util.AbstractIOIOActivity.IOIOThread#loop()
		 */
		@Override
		public void loop() throws ConnectionLostException {
			//ioio_.beginBatch();
			if (reset) {
				reset = false;
				ioio_.ujReset();				
			}
			if (loaded) {
				loaded = false;
				ioio_.ujPayload(maxsize, buffer);
				ioio_.ujInit();
			}
			led_.write(!buttonO.isChecked());
			fw_.write(!(buttonF.isChecked()||buttonFL.isChecked()||buttonFR.isChecked()));
			bw_.write(!(buttonB.isChecked()||buttonBL.isChecked()||buttonBR.isChecked()));
			lf_.write(!(buttonL.isChecked()||buttonFL.isChecked()||buttonBL.isChecked()));
			rg_.write(!(buttonR.isChecked()||buttonFR.isChecked()||buttonBR.isChecked()));
			//led_.write(!button_.isChecked());
			//ioio_.endBatch();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		}
	}

	/**
	 * A method to create our IOIO thread.
	 * 
	 * @see ioio.lib.util.AbstractIOIOActivity#createIOIOThread()
	 */
	@Override
	protected IOIOLooper createIOIOLooper() {
		return new Looper();
	}
}