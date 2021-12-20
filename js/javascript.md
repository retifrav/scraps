## JavaScript

<!-- MarkdownTOC -->

- [HTTP request](#http-request)
    - [XMLHttpRequest](#xmlhttprequest)
    - [fetch](#fetch)
- [Create new element](#create-new-element)
    - [From objects and methods](#from-objects-and-methods)
    - [From raw HTML](#from-raw-html)

<!-- /MarkdownTOC -->

### HTTP request

#### XMLHttpRequest

``` js
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

#### fetch

``` js
/*let someTable = document.getElementById("some-table");
while (someTable.firstChild)
{
    someTable.removeChild(someTable.firstChild);
}*/

/*document.getElementById("alert-error").style.display = "none";
document.getElementById("some-btn").style.display = "none";
document.getElementById("loading-animation").style.display = "block";*/

let params = {
    "some": "ololo",
    "another": 2
};
fetch(
    "/path/to/remote/endpoint",
    {
        method: "POST",
        headers:
        {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(params)
    }
)
.then(
    response => {
        console.debug(response.ok, response.status);
        if (!response.ok) {
            throw new Error(`Status code: ${response.status}`);
        }
        return response.json();
    }
)
.then(
    data => {
        console.debug(data);
        /*for (let d in data)
        {
            let tr = `<tr>
                <td>${data[d].one}</td>
                <td>${data[d].two}</td>
                <td>${data[d].three}</td>
                <td>${data[d].four}</td>
            </tr>`;
            someTable.appendChild(htmlToElement(tr));
        }*/
    }
)
.catch(
    error => {
        let errorMsg = `Request error. ${error.message}`;
        console.error(errorMsg);
        /*document.getElementById("alert-error").innerHTML = errorMsg;
        document.getElementById("alert-error").style.display = "block";*/
    }
)
.finally(
    function()
    {
        console.debug("Something to do after request has either failed or succeeded");
        /*document.getElementById("loading-animation").style.display = "none";
        document.getElementById("some-btn").style.display = "block";*/
    }
);
````

### Create new element

#### From objects and methods

```
var someParentDiv = document.getElementById("some-div");

let p = document.createElement("p");
p.innerText = "ololo";

someParentDiv.appendChild(p);
```

#### From raw HTML

``` js
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
