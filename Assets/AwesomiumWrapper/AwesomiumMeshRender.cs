using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

/**
 * Apply on GUIText GameObject
 * */
public class AwesomiumMeshRender : MonoBehaviour
{
    public int width, height;
    private GUITexture gui;
    //public int width, height;

    private Color[] m_pixels;
    private GCHandle m_pixelsHandler;
    private Texture2D m_texture;
    private int m_TextureID;

    private bool showBrowser = true;

    public bool isAwesomiumInit;

    private ControlWindow controlWindow;
    private BrowserGUIEvents browserEventHandler;

    private AwesomiumWrapper.SetPixelsFunc m_setPixelsFunction;
    private AwesomiumWrapper.ApplyPixelsFunc m_applyPixelsFunction;


    // Use this for initialization
    void Start()
    {
        browserEventHandler = GetComponent<BrowserGUIEvents>();
        controlWindow = GameObject.Find("ControlWindow").GetComponent<ControlWindow>();
        InitAwesomium(width, height);        
        
        //// Create texture in ARGB32 format
        //m_texture = new Texture2D(width, height, TextureFormat.ARGB32, true);
        ////Get Color[] (pixels) from texture 
        //m_pixels = m_texture.GetPixels(0);
        //// Create window handle id - future usage
        //m_TextureID = m_texture.GetInstanceID();
        //// assign m_texture to this GUITexture texture
        //gameObject.renderer.material.mainTexture = m_texture;
        //// Create GCHandle - Allocation of m_pixels in memory. 
        //m_pixelsHandler = GCHandle.Alloc(m_pixels, GCHandleType.Pinned);

        ////Map delegates
        ////m_setPixelsFunction = this.SetPixels;
        ////m_applyPixelsFunction = this.ApplyPixels;
        ////AwesomiumWrapper.SetDelegates(m_setPixelsFunction, m_applyPixelsFunction);

        //isAwesomiumInit = true;
        //AwesomiumWrapper.init(m_pixelsHandler.AddrOfPinnedObject(), width, height);       
    }

    public void InitAwesomium(int width, int height)
    {
        Debug.Log("init awseomium");
        this.width = width;
        this.height = height;
        m_texture = new Texture2D(width, height, TextureFormat.ARGB32, true);
        //Get Color[] (pixels) from texture 
        m_pixels = m_texture.GetPixels(0);
        // Create window handle id - future usage
        m_TextureID = m_texture.GetInstanceID();
        // assign m_texture to this GUITexture texture
        gameObject.renderer.material.mainTexture = m_texture;
        // Create GCHandle - Allocation of m_pixels in memory. 
        m_pixelsHandler = GCHandle.Alloc(m_pixels, GCHandleType.Pinned);
        AwesomiumWrapper.init(m_pixelsHandler.AddrOfPinnedObject(), width, height);
        isAwesomiumInit = true;
    }


    // Update is called once per frame
    void Update()
    {
        if (isAwesomiumInit == true && controlWindow.showBrowser)
        {
            AwesomiumWrapper.update();
            // Check to see if render flag is set in unmanaged code. Need changing to delegates instead
            if (AwesomiumWrapper.isDirtyBuffer())
            {
                m_texture.SetPixels(m_pixels, 0);
                m_texture.Apply();
            }
        }
        
      
    }

    public BrowserGUIEvents getEventHandler()
    {
        return browserEventHandler;
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

    // Methods should be moved to new class

    public void Loadfile(string filePath)
    {
        AwesomiumWrapper.loadFile(filePath);
    }

    public void DestroyAwesomium()
    {
        try
        {            
            isAwesomiumInit = false;
            AwesomiumWrapper.Destroy();
            m_pixelsHandler.Free();
        }
        catch (System.Exception e)
        {
            Debug.Log(e);
        }
        
        
    }


}
