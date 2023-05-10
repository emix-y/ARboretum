using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using NativeWebSocket;

public class Connection : MonoBehaviour
{
    WebSocket websocket;
    public float temp;
    public float humid;
    public GameObject TheGaurdian;
    private GameObject sphere;

    // Start is called before the first frame update
    async void Start()
    {
        websocket = new WebSocket("ws://192.168.8.100:80");

        websocket.OnOpen += () =>
        {
            Debug.Log("Connection open!");
        };

        websocket.OnError += (e) =>
        {
            Debug.Log("Error! " + e);
        };

        websocket.OnClose += (e) =>
        {
            Debug.Log("Connection closed!");
        };

        websocket.OnMessage += (bytes) =>
        {
            Debug.Log("OnMessage!");
            Debug.Log(bytes);

            // getting the message as a string
            var message = System.Text.Encoding.UTF8.GetString(bytes);
            // Debug.Log("OnMessage! " + message);

            string[] StringValues = message.Split(new string[] { "," }, System.StringSplitOptions.RemoveEmptyEntries);
            humid = float.Parse(StringValues[0]);
            temp = float.Parse(StringValues[1]);
        };

        // Instantiate the sphere object
        sphere = Instantiate(TheGaurdian, Vector3.zero, Quaternion.identity);

        // waiting for messages
        await websocket.Connect();
    }

    void Update()
    {
        // Map the temperature to the RGB color values
        float red = Mathf.Clamp(temp / 50f, 0f, 1f);
        float green = Mathf.Clamp(1f - (temp / 50f), 0f, 1f);
        float blue = 0f;
        sphere.GetComponent<Renderer>().material.color = new Color(red, green, blue);

        // Map the humidity to the scale of the sphere
        float scale = Mathf.Lerp(0.5f, 2f, humid / 100f);
        sphere.transform.localScale = new Vector3(scale, scale, scale);

#if !UNITY_WEBGL || UNITY_EDITOR
        websocket.DispatchMessageQueue();
#endif
    }

    async void SendWebSocketMessage()
    {
        if (websocket.State == WebSocketState.Open)
        {
            // Sending plain text
            await websocket.SendText("Hello");
        }
    }

    private async void OnApplicationQuit()
    {
        // Destroy the sphere object
        Destroy(sphere);

        await websocket.Close();
    }
}
