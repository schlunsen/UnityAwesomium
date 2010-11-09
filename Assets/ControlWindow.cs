using UnityEngine;
using System.Collections;

public class ControlWindow : MonoBehaviour
{

    public bool showBrowser = true;
    //Window releated;
    private int controlWindowId = 1;
    private Rect controlWinRect = new Rect(0, 0, 300, 300);
    private string txtString = "http://www.dr.dk";

    public GameObject gui;

    // Use this for initialization
    void Start()
    {        
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

    }

    private void fadeUpButton()
    {

        if (GUILayout.Button("Fade Up Browser"))
        {
            showBrowser = true;
            iTween.FadeTo(gui, 1, 1);

        }


    }

    private void fadeDownButton()
    {
        if (GUILayout.Button("Fade Down Browser"))
        {
            showBrowser = false;
            iTween.FadeTo(gui, 0, 1);

        }
    }

}
