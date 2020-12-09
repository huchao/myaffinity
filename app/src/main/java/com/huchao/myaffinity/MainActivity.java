package com.huchao.myaffinity;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("native-lib");
    }

    private boolean mIsEnd;

    private EditText mEtCPU;
    private Button mBtnStartLoop;
    private Button mBtnEndLoop;
    private Button mBtnGet;
    private Button mBtnSet;
    private Button mBtnGetNcnn;
    private Button mBtnSetNcnn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mEtCPU = findViewById(R.id.et_cpu);
        mBtnStartLoop = findViewById(R.id.btn_start_loop);
        mBtnEndLoop = findViewById(R.id.btn_end_loop);
        mBtnGet = findViewById(R.id.btn_get);
        mBtnSet = findViewById(R.id.btn_set);
        mBtnGetNcnn = findViewById(R.id.btn_get_ncnn);
        mBtnSetNcnn = findViewById(R.id.btn_set_ncnn);

        mBtnStartLoop.setOnClickListener(this);
        mBtnEndLoop.setOnClickListener(this);
        mBtnGet.setOnClickListener(this);
        mBtnSet.setOnClickListener(this);
        mBtnGetNcnn.setOnClickListener(this);
        mBtnSetNcnn.setOnClickListener(this);

        mBtnEndLoop.setEnabled(false);
    }

    @Override
    public void onClick(View v) {
        if (v.equals(mBtnStartLoop)) {
            mBtnEndLoop.setEnabled(true);

            Toast.makeText(this, "StartLoop", Toast.LENGTH_SHORT).show();
            int cores = Runtime.getRuntime().availableProcessors();
            for (int index = 0; index < cores; index++) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (true) {
                            if (mIsEnd) {
                                return;
                            }
                        }
                    }
                }).start();
            }

        } else if (v.equals(mBtnEndLoop)) {
            mBtnStartLoop.setEnabled(true);

            Toast.makeText(this, "StopLoop", Toast.LENGTH_SHORT).show();
            mIsEnd = true;

        } else if (v.equals(mBtnGet)) {
            long mask = getCurrentThreadAffinityDebug();
            android.util.Log.i("CrashReportT", "getCurrentThreadAffinityDebug mask: " + mask);

        } else if (v.equals(mBtnSet)) {
            long mask = Long.valueOf(mEtCPU.getText().toString(), 2);
            if (setCurrentThreadAffinityDebug(mask)) {
                android.util.Log.i("CrashReportT", "setCurrentThreadAffinityDebug succ, mask: " + mask);
            } else {
                android.util.Log.i("CrashReportT", "setCurrentThreadAffinityDebug fail, mask: " + mask);
            }

        } else if (v.equals(mBtnGetNcnn)) {
            getCurrentThreadAffinity2();
            getCurrentThreadAffinityNcnn();

        } else if (v.equals(mBtnSetNcnn)) {
            getCurrentThreadAffinity2();
            attachToCpuNcnn();
            getCurrentThreadAffinity2();
        }
    }

    public static native void getCurrentThreadAffinity();
    public static native void attachToCpu(int cpu_index);

    public static native void getCurrentThreadAffinity2();
    public static native void attachToCpu2(int cpu_index);

    public static native void getCurrentThreadAffinityNcnn();
    public static native void attachToCpuNcnn();

    public static native long getCurrentThreadAffinityDebug();
    public static native boolean setCurrentThreadAffinityDebug(long mask);
}
