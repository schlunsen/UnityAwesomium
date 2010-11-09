using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

/**
 * Apply on GUIText GameObject
 * */
public class AwesomiumGUITextRender : MonoBehaviour
{
    private GUITexture gui;
    public int width, height;
    private string txtString = "http://www.dr.dk";
    private Color[] m_pixels;
    private GCHandle m_pixelsHandler;
    private Texture2D m_texture;
    private int m_TextureID;

    private bool showBrowser = true;

    private bool isInit;

    //Window releated;
    private int controlWindowId = 1;
    private Rect controlWinRect = new Rect(0,0,300,300); 


    private AwesomiumWrapper.SetPixelsFunc m_setPixelsFunction;
    private AwesomiumWrapper.ApplyPixelsFunc m_applyPixelsFunction;


    // Use this for initialization
    void Start()
    {
        
        gui = GetComponent(typeof(GUITexture)) as GUITexture;
        // Center texture
        gui.pixelInset = new Rect(-(gui.pixelInset.width / 2), -gui.pixelInset.height / 2 , gui.pixelInset.width, gui.pixelInset.height);
        
        
        
        // Create texture in ARGB32 format
        m_texture = new Texture2D((int)gui.pixelInset.width, (int)gui.pixelInset.height, TextureFormat.ARGB32, true);
        //Get Color[] (pixels) from texture 
        m_pixels = m_texture.GetPixels(0);        
        // Create window handle id - future usage
        m_TextureID = m_texture.GetInstanceID();       
        
       
        gui.texture = m_texture;



        // Create GCHandle - Allocation of m_pixels in memory. 
        m_pixelsHandler = GCHandle.Alloc(m_pixels, GCHandleType.Pinned);
        

        //Map delegates
        //m_setPixelsFunction = this.SetPixels;
        //m_applyPixelsFunction = this.ApplyPixels;
        //AwesomiumWrapper.SetDelegates(m_setPixelsFunction, m_applyPixelsFunction);

        initAwesomium();

    }

    private void initAwesomium()
    {
        isInit = true;
        //AwesomiumWrapper.init(m_pixelsHandler.AddrOfPinnedObject(), (int)gui.pixelInset.width, (int)gui.pixelInset.height);
        AwesomiumWrapper.init(m_pixelsHandler.AddrOfPinnedObject(), width, height);
        Debug.Log(gui.pixelInset.width);
    }


    // Update is called once per frame
    void Update()
    {

       
        if (isInit == true && showBrowser)
        {
            AwesomiumWrapper.update();

            if (AwesomiumWrapper.isDirtyBuffer())
            {
                
                m_texture.SetPixels(m_pixels, 0);
                m_texture.Apply();
            }
        }

        handleKeys();
    }

    private void handleKeys()
    {
        if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            Debug.Log("Down");
            AwesomiumWrapper.scrollWheel(-100);
        }
        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            Debug.Log("Down");
            AwesomiumWrapper.scrollWheel(100);
        }
    }   


    void OnGUI()
    {      
        controlWinRect = GUILayout.Window(controlWindowId, controlWinRect, ControlWindowFunc, "Control window");

    }

    public void ControlWindowFunc(int winId)
    {
        GUI.DragWindow(new Rect(0, 0, 500, 25));
        DrawTopLeft();

    }

    private void DrawTopLeft()
    {

        GUILayout.BeginHorizontal();
        txtString = GUILayout.TextField(txtString);

        if (GUILayout.Button("Go"))
        {
            AwesomiumWrapper.gotoURL(txtString);
        }
        GUILayout.EndHorizontal();

        if (showBrowser)
        {
            fadeDownButton();
        }
        else
        {
            fadeUpButton();
        }      
        GUILayout.Label("m_pixels size (Color RGBA): " + m_pixels.Length);   

    }

    private void fadeUpButton()
    {

        if (GUILayout.Button("Fade Up Browser"))
        {
            showBrowser = true;
            iTween.FadeTo(gui.gameObject, 1, 1);
            
        }

        
    }

    private void fadeDownButton()
    {
        if (GUILayout.Button("Fade Down Browser"))
        {
            showBrowser = false;
            

            iTween.FadeTo(gui.gameObject, 0, 1);

        }
    }

    
    void OnApplicationQuit()
    {
        Debug.Log("quit");
        AwesomiumWrapper.closeFileStream();

    }


    public void SetPixels()
    {
       
    }

    public void ApplyPixels()
    {

    }


}
