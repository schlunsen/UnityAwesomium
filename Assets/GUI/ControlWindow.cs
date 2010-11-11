using UnityEngine;
using System.Collections;

public class ControlWindow : MonoBehaviour
{

    public bool showBrowser = true;
    //Window releated;
    private int controlWindowId = 1;
    private Rect controlWinRect = new Rect(0, 0, 400, 200);
    private string txtString = "http://www.dr.dk";
    private Vector3 startPosition;
    public GameObject gui;
    public AwesomiumMeshRender meshRender;


    private string textWidth = "500", textHeight = "500", loadFileTxt = "html/test.html";


    // Use this for initialization
    void Start()
    {
        startPosition = gui.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
    }

    void OnGUI()
    {
        controlWinRect = GUI.Window(controlWindowId, controlWinRect, ControlWindowFunc, "Control window");

    }

    public void ControlWindowFunc(int winId)
    {
        GUI.DragWindow(new Rect(0, 0, 500, 25));

        //GUILayout.BeginArea(new Rect(,20,controlWinRect.width ,controlWinRect.height));
        
        DrawTopLeft();        
        
        //GUILayout.EndArea();

    }

    private void DrawTopLeft()
    {        
        GUILayout.BeginHorizontal();
        GUI.SetNextControlName("urlInputField");
        
        txtString = GUILayout.TextField(txtString);

        if (Event.current.isKey && Event.current.keyCode == KeyCode.Return && GUI.GetNameOfFocusedControl() == "urlInputField")
            meshRender.LoadURL(txtString);

        if (GUILayout.Button("Go"))
        {
            meshRender.LoadURL(txtString);            
        }
        GUILayout.EndHorizontal();
        GUILayout.Space(4);
        GUILayout.BeginVertical();
        if (showBrowser)
        {
            HideBrowserButton();
        }
        else
        {
            ShowBrowserButton();
        }
        
        //GUI.SetNextControlName("textWidth");
        //textWidth = GUILayout.TextField(textWidth, GUILayout.Width(10));
        GUILayout.EndVertical();

        DrawFileLoad();
        drawChangeWindowSize();
        drawDestroyInitComponents();

    }

    private void drawDestroyInitComponents()
    {
        if (meshRender.isAwesomiumInit)
        {
            if (GUILayout.Button("Destroy"))
            {
                meshRender.DestroyAwesomiumWindow();
            }
        }
        else
        {
            if (GUILayout.Button("init"))
            {
                try
                {
                    int aweWidth = int.Parse(textWidth);
                    int aweHeight = int.Parse(textHeight);
                    meshRender.InitAwesomium(aweWidth,aweHeight);
                }
                catch (System.Exception)
                {
                    Debug.Log("Error parsing width and height");   
                }               
            }
        }
    }

    private void DrawFileLoad()
    {
        GUILayout.BeginVertical();

        GUILayout.BeginHorizontal();

        GUILayout.Label("File: ");
        loadFileTxt = GUILayout.TextField(loadFileTxt);
        if (GUILayout.Button("Load"))
        {
            meshRender.Loadfile(loadFileTxt);
        }

        GUILayout.EndHorizontal();

        GUILayout.EndVertical();
    }

    private void drawChangeWindowSize()
    {

        GUILayout.BeginHorizontal();
        GUILayout.Label("Width");        
        textWidth = GUILayout.TextField(textWidth);
        GUILayout.Label("Height");        
        textHeight = GUILayout.TextField(textHeight);        
        if (GUILayout.Button("Resize"))
        {
            meshRender.DestroyAwesomiumWindow();
            try
            {
                int aweWidth = int.Parse(textWidth);
                int aweHeight = int.Parse(textHeight);
                meshRender.InitAwesomium(aweWidth, aweHeight);
                meshRender.getEventHandler().setDimensions(aweWidth, aweHeight);
            }
            catch (System.Exception)
            {
                Debug.Log("Error parsing width and height");
            }  

        }
        GUILayout.EndHorizontal();

    }

    

    private void ShowBrowserButton()
    {

        if (GUILayout.Button("Show Browser"))
        {
            showBrowser = true;
            //iTween.Stop(gui);
            iTween.MoveTo(gui, iTween.Hash("position", startPosition, "time", 2f));
            iTween.FadeTo(gui.gameObject, 1, 1);
        }


    }

    private void HideBrowserButton()
    {
        if (GUILayout.Button("Hide Browser"))
        {
            //startPosition = gameObject.transform.position;
            showBrowser = false;
            iTween.FadeTo(gui.gameObject, 0, 1);
            iTween.MoveAdd(gui, new Vector3(20,0,0),2);

        }
    }

}
