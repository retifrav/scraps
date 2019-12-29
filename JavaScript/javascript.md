## JavaScript

### XMLHttpRequest

```
let xhr = new XMLHttpRequest();
xhr.responseType = "application/json";
xhr.open("GET", "/path/to/some.json");
xhr.send();

xhr.onload = function()
{
    if (xhr.status != 200)
    {
        console.error("Error, status ", xhr.status);
    }
    else
    {
        let rez = JSON.parse(xhr.response);
        console.log(rez);
    }
};

xhr.onerror = function()
{
    console.error("Couldn't send the request");
};
```
