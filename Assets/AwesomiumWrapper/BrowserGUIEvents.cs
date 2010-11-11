using UnityEngine;
using System.Collections;

public class BrowserGUIEvents : MonoBehaviour
{

    public bool interactive = true;
    private int width, height;

    private string output;

    private bool mouseOver = false;
    public AwesomiumMeshRender mesh;
    

    void Start()
    {
        width = mesh.width;
        height = mesh.height;
        
        
    }

    public void setDimensions(int width, int height){
        this.width = width;
        this.height = height;
    }

    void OnGUI()
    {
        // Inject input into the page when the GUI doesn't have focus
        if (Event.current.isKey && GUIUtility.keyboardControl == 0)
        {
            // Insert character
            //Debug.Log(Event.current.keyCode);
            //AwesomiumWrapper.InjectKeyboard(mesh.m_TextureID, (int)Event.current.keyCode, 2, 0);
        
            
            
            //UnityBerkelium.Window.textEvent(m_TextureID, Event.current.character);

            //KeyCode key = Event.current.keyCode;
            //bool pressed = (Event.current.type == EventType.KeyDown);

            //// Special case for backspace
            //if (key == KeyCode.Backspace)
            //    UnityBerkelium.Window.keyEvent(m_TextureID, pressed, 0, 08, 0);
            //// Special case for enter
            //else if (key == KeyCode.Return)
            //    UnityBerkelium.Window.keyEvent(m_TextureID, pressed, 0, 13, 0);

            // TODO Handle all keys
            /*int mods = 0;
            int vk_code = UnityBerkelium.convertKeyCode(Event.current.keyCode);
            int scancode = 0;
            UnityBerkelium.Window.keyEvent(m_TextureID, pressed, mods, vk_code, scancode);
            print("Key event: " + pressed + ", " + Event.current.keyCode);*/
        }

    }

    void Update()
    {

        float scroll = Input.GetAxis("Mouse ScrollWheel");
        if (scroll != 0)
            AwesomiumWrapper.ScrollWheel(mesh.m_TextureID, (int)scroll);

        handleKeys();
    }

    private void handleKeys()
    {

        int dyScroll = (int)Input.GetAxis("Vertical") * 100;
        if (dyScroll != 0)
        {
            AwesomiumWrapper.ScrollWheel(mesh.m_TextureID, dyScroll);
        }
        
    }
    void OnMouseDown()
    {
       
        // Only when interactive is enabled
        if (!interactive)
            return;        
        RaycastHit hit;
        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
        {           

            int x = /*width -*/ (int)(hit.textureCoord.x * width);
            int y = height - (int)(hit.textureCoord.y * height);

            AwesomiumWrapper.MouseMove(mesh.m_TextureID, x, y);
            AwesomiumWrapper.MouseDown(mesh.m_TextureID, 0);                   
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
            AwesomiumWrapper.MouseMove(mesh.m_TextureID, x, y);
            AwesomiumWrapper.MouseUp(mesh.m_TextureID, 0);
        }
    }
    

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
            AwesomiumWrapper.MouseMove(mesh.m_TextureID, x, y);           
        }
    }


    void OnMouseExit()
    {
        mouseOver = false;

    }
}
