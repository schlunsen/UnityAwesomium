using UnityEngine;
using System.Collections;

public class BrowserGUIEvents : MonoBehaviour
{

    public bool interactive = true;
    public int width, height;

    private string output;

    void Start()
    {

        
    }

    void OnMouseDown()
    {
       
        // Only when interactive is enabled
        if (!interactive)
            return;
        
        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {

            Debug.Log("Mousedown");

            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);

            // Focus the window
            //UnityBerkelium.Window.focus(m_TextureID);

            //UnityBerkelium.Window.mouseMove(m_TextureID, x, y);
            //UnityBerkelium.Window.mouseDown(m_TextureID, 0);

            Debug.Log("x " + x + " y: " + y);
        }
    }

    void OnMouseUp()
    {
        // Only when interactive is enabled
        if (!interactive)
            return;

        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {
            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);          
        }
    }
    //UnityBerkelium.Window.mouseMove(m_TextureID, x, y);
    //UnityBerkelium.Window.mouseUp(m_TextureID, 0);

    void OnMouseOver()
    {

        //Debug.Log("Mouse Over");
    }
}
