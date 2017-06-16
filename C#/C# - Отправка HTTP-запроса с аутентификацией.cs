// создание запроса
HttpWebRequest httpWReq = (HttpWebRequest)WebRequest.Create("http://someserver.net:1000/somehttphandler.ashx");
// тут логин и пароль. Если аутентификация не требуется, то всю строку выкинуть
httpWReq.Credentials = new NetworkCredential("domen\\login", "password");

UTF8Encoding encoding = new UTF8Encoding();
string postData = textBox1.Text; // ну тут предполагается, что на форме есть текстовое поле
byte[] data = encoding.GetBytes(postData);
httpWReq.Method = "POST";
httpWReq.ContentType = "application/x-www-form-urlencoded";
httpWReq.ContentLength = data.Length;
using (Stream stream = httpWReq.GetRequestStream())
{
    stream.Write(data, 0, data.Length);
}
HttpWebResponse response = (HttpWebResponse)httpWReq.GetResponse();
string responseString = new StreamReader(response.GetResponseStream()).ReadToEnd();

textBox1.Text = responseString;