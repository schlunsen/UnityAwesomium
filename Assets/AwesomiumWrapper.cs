

using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class AwesomiumWrapper {



    public delegate void SetPixelsFunc();

    public delegate void ApplyPixelsFunc();

    [DllImport("UnityAwesomium", EntryPoint = "init")]
    public static extern void init(IntPtr m_Pixels, int width, int height);

    [DllImport("UnityAwesomium")]
    public static extern void closeFileStream();


    [DllImport("UnityAwesomium")]
    public static extern void gotoURL(string url);


    [DllImport("UnityAwesomium")]
    public static extern void main();


    [DllImport("UnityAwesomium")]
    public static extern void update();

    [DllImport("UnityAwesomium")]
    public static extern void scrollWheel(int amount);

    [DllImport("UnityAwesomium")]
    public static extern bool isDirtyBuffer();
    


    //[DllImport("UnityAwesomium")]
    //public static extern void SetDelegates(SetPixelsFunc setPixelsFunc, ApplyPixelsFunc applyPixelsFunc);


    public class Window
    {
        [DllImport("UnityAwesomium")]
        public static extern void update();


    }

}
