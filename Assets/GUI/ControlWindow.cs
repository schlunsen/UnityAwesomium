using UnityEngine;
using System.Collections;

public class ControlWindow : MonoBehaviour
{

    public bool showBrowser = true;
    //Window releated;
    private int controlWindowId = 1;
    private Rect controlWinRect = new Rect(0, 0, 300, 150);
    private string txtString = "http://www.dr.dk";
    private Vector3 startPosition;
    public GameObject gui;

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
        GUI.SetNextControlName("urlInputField");
        
        txtString = GUILayout.TextField(txtString);

        if (Event.current.isKey && Event.current.keyCode == KeyCode.Return && GUI.GetNameOfFocusedControl() == "urlInputField")
            AwesomiumWrapper.gotoURL(txtString);

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

    }

    private void fadeUpButton()
    {

        if (GUILayout.Button("Show Browser"))
        {
            showBrowser = true;
            //iTween.Stop(gui);
            iTween.MoveTo(gui, iTween.Hash("position", startPosition, "time", 2f));
            iTween.FadeTo(gui.gameObject, 1, 1);
        }


    }

    private void fadeDownButton()
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
