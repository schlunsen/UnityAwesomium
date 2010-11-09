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

    private bool isInit;


    private AwesomiumWrapper.SetPixelsFunc m_setPixelsFunction;
    private AwesomiumWrapper.ApplyPixelsFunc m_applyPixelsFunction;


    // Use this for initialization
    void Start()
    {

        //gui = GetComponent(typeof(GUITexture)) as GUITexture;
        // Center texture
        //gui.pixelInset = new Rect(-(gui.pixelInset.width / 2), -gui.pixelInset.height / 2, gui.pixelInset.width, gui.pixelInset.height);
        // Create texture in ARGB32 format
        m_texture = new Texture2D(width, height, TextureFormat.ARGB32, true);
        //Get Color[] (pixels) from texture 
        m_pixels = m_texture.GetPixels(0);
        // Create window handle id - future usage
        m_TextureID = m_texture.GetInstanceID();
        // assign m_texture to this GUITexture texture
        gameObject.renderer.material.mainTexture = m_texture;
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
        AwesomiumWrapper.init(m_pixelsHandler.AddrOfPinnedObject(), width, height);       
    }


    // Update is called once per frame
    void Update()
    {

        if (isInit == true && showBrowser)
        {
            AwesomiumWrapper.update();
            // Check to see if render flag is set in unmanaged code. Need changing to delegates instead
            if (AwesomiumWrapper.isDirtyBuffer())
            {
                m_texture.SetPixels(m_pixels, 0);
                m_texture.Apply();
            }
        }
        //messy keyhandle - for testing
      
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
