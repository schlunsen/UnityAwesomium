using UnityEngine;
using System.Collections;

public class BrowserGUIEvents : MonoBehaviour
{

    public bool interactive = true;
    public int width, height;

    private string output;

    private bool mouseOver = false;

    void Start()
    {

        
    }


    void Update()
    {

        float scroll = Input.GetAxis("Mouse ScrollWheel");
        if (scroll != 0)
            AwesomiumWrapper.scrollWheel((int)scroll);

        handleKeys();
    }

    private void handleKeys()
    {

        int dyScroll = (int)Input.GetAxis("Vertical") * 100;
        if (dyScroll != 0)
        {
            AwesomiumWrapper.scrollWheel(dyScroll);
        }
        
    }
    void OnMouseDown()
    {
       
        // Only when interactive is enabled
        if (!interactive)
            return;
        Debug.Log("Mousedown");
        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {           

            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);

            // Focus the window
            //UnityBerkelium.Window.focus(m_TextureID);

            AwesomiumWrapper.mouseMove(x, y);
            AwesomiumWrapper.mouseDown(0);            

       
        }
    }

    void OnMouseUp()
    {
        // Only when interactive is enabled
        if (!interactive)
            return;
        Debug.Log("Mouse up 1");
        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {
            Debug.Log("Mouse up");
            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);
            AwesomiumWrapper.mouseMove(x, y);
            AwesomiumWrapper.mouseUp(0);
        }
    }
    //UnityBerkelium.Window.mouseMove(m_TextureID, x, y);
    //UnityBerkelium.Window.mouseUp(m_TextureID, 0);

    void OnMouseOver()
    {
        mouseOver = true;
        // Only when interactive is enabled
        if (!interactive)
            return;

      
        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {
            
            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);
            Debug.Log("x " + x + " y: " + y + " width:" + width);
            AwesomiumWrapper.mouseMove(x, y);

            
        }
    }


    void OnMouseExit()
    {
        mouseOver = false;

    }
}
