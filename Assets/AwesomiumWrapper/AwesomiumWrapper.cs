

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
    public static extern void Destroy();

    [DllImport("UnityAwesomium")]
    public static extern void closeFileStream();


    [DllImport("UnityAwesomium")]
    public static extern void gotoURL(string url);

    [DllImport("UnityAwesomium")]
    public static extern void loadFile(string url);


    [DllImport("UnityAwesomium")]
    public static extern void main();


    [DllImport("UnityAwesomium")]
    public static extern void update();

    

    [DllImport("UnityAwesomium")]
    public static extern bool isDirtyBuffer();

    //Mouse functions

    [DllImport("UnityAwesomium")]
    public static extern void mouseMove(int x,int y);

    // 0 = left mouse, 1 = right mouse
    [DllImport("UnityAwesomium")]
    public static extern void mouseDown(int mouseBtn);

    [DllImport("UnityAwesomium")]
    public static extern void mouseUp(int mouseBtn);

    [DllImport("UnityAwesomium")]
    public static extern void scrollWheel(int amount);

    // end Mouse func
    
    // Key func
    [DllImport("UnityAwesomium")]
    public static extern void injectKeyboard(int msg, uint wParam, long lParam);


    


    //public class Window
    //{
    //    [DllImport("UnityAwesomium")]
    //    public static extern void update();


    //}

}
