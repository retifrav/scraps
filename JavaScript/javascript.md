## JavaScript

- [XMLHttpRequest](#xmlhttprequest)
- [Create new element](#create-new-element)
  - [From objects and methods](#from-objects-and-methods)
  - [From raw HTML](#from-raw-html)

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

### Create new element

#### From objects and methods

```
var someParentDiv = document.getElementById("some-div");

let p = document.createElement("p");
p.innerText = "ololo";

someParentDiv.appendChild(p);
```

#### From raw HTML

```
function htmlToElement(html)
{
    let template = document.createElement("template");
    html = html.trim();
    template.innerHTML = html;
    return template.content.firstChild;
}

var someParentDiv = document.getElementById("some-div");

let p = "<p>ololo</p>";

someParentDiv.appendChild(htmlToElement(p));
```
