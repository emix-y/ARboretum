using UnityEditor.ShaderKeywordFilter;
using UnityEngine;
using WebSocketSharp;
public class WsClient : MonoBehaviour
{
    WebSocket ws;

    public float humidity = 1.0f;
    public float temp = 1.0f;

    public string url = "192.168.8.101:80";
    private void Start()
    {
        ws = new WebSocket("ws://"+url);
        ws.Connect();
        ws.OnMessage += (sender, e) =>
        {
            string[] StringValues = e.Data.Split(new string[] { "," }, System.StringSplitOptions.RemoveEmptyEntries);
            humidity = float.Parse(StringValues[0]);
            temp = float.Parse(StringValues[1]);
        };
    }
    private void Update()
    {
        if (ws == null)
        {
            return;
        }
        if (Time.frameCount % 2500 ==0)
        {
    
            ws.Send("Hello");
        }
    }
}