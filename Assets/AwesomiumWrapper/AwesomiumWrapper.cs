using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class AwesomiumWrapper {



    public delegate void SetPixelsFunc();

    public delegate void ApplyTextureFunc();


    [DllImport("UnityAwesomium")]
    public static extern bool CreateAwesomiumWebView(int uniqueID, IntPtr pixelBuffer, int width, int height, SetPixelsFunc setPixelsFunc, ApplyTextureFunc applyTextureFunc);

    [DllImport("UnityAwesomium")]
    public static extern bool DestroyAwesomiumWebView(int uniqueID);

    [DllImport("UnityAwesomium")]
    public static extern void Init();

    [DllImport("UnityAwesomium")]
    public static extern void Destroy();

    [DllImport("UnityAwesomium")]
    public static extern void CloseFileStream();


    [DllImport("UnityAwesomium")]
    public static extern void LoadURL(int uniqueId, string url);

    [DllImport("UnityAwesomium")]
    public static extern void LoadFile(int uniqueId, string url);    


    [DllImport("UnityAwesomium")]
    public static extern void Update();    

    [DllImport("UnityAwesomium")]
    public static extern bool isDirty(int uniqueId);

    [DllImport("UnityAwesomium")]
    public static extern void EnabledWindow(int uniqueId, bool isEnabled);   

    //Mouse functions
    [DllImport("UnityAwesomium")]
    public static extern void MouseMove(int uniqueId, int x, int y);

    // 0 = left mouse, 1 = right mouse
    [DllImport("UnityAwesomium")]
    public static extern void MouseDown(int uniqueId, int mouseBtn);

    [DllImport("UnityAwesomium")]
    public static extern void MouseUp(int uniqueId, int mouseBtn);

    [DllImport("UnityAwesomium")]
    public static extern void ScrollWheel(int uniqueId, int amount);

    // end Mouse func
    
    // Key func
    [DllImport("UnityAwesomium")]
    public static extern void InjectKeyboard(int uniqueId, int msg, uint wParam, long lParam);    


}
