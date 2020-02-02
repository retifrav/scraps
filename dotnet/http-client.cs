using System.Net.Http;
using Newtonsoft.Json;
using System.Threading.Tasks;

// ...

public static async Task<Tuple<int, string>> SendHttpRequest(
    HttpClient httpClient,
    HttpMethod httpMethod,
    string host,
    string query,
    dynamic msg
    )
{
    httpClient.BaseAddress = new Uri(host);

    HttpRequestMessage request = new HttpRequestMessage(httpMethod, query);

    request.Content = new StringContent(
        JsonConvert.SerializeObject(msg),
        Encoding.UTF8,
        "application/json"
        );

    var httpResponse = await httpClient.SendAsync(request);
    var httpContent = await httpResponse.Content.ReadAsStringAsync();

    return new Tuple<int, string>((int)httpResponse.StatusCode, httpContent);
}

// ...

static void Main(string[] args)
{
    var msg = new {
            attachments = new [] {
                new {
                    title = "Test",
                    color = "#CCCCCC",
                    fallback = "Test message",
                    text = "Some test message"
                        .Replace("&", "&amp;")
                        .Replace("<", "&lt;")
                        .Replace(">", "&gt;")
                }
            }
    };

    Task<Tuple<int, string>> task = SendHttpRequest(
        new HttpClient(),
        HttpMethod.Post,
        "https://hooks.slack.com",
        "/services/T4DLK36N5/B4A59TLM3/iBbx5ox9FJCPpFOJo4N9AmaQ",
        msg
    );

    Console.WriteLine($"{task.Result.Item1}: {task.Result.Item2}");
}
