## Json.NET

<!-- MarkdownTOC -->

- [Creating an object](#creating-an-object)

<!-- /MarkdownTOC -->

### Creating an object

<https://www.newtonsoft.com/json/help/html/CreatingLINQtoJSON.htm#CreatingLINQ>

``` cs
var someJSONobject = new JObject(
    new JProperty("Libraries",
        new JObject(
            new JProperty("SomeLibrary",
                new JObject(
                    new JProperty("icon", "books.svg"),
                    new JProperty("links",
                        new JObject(
                            new JProperty("Release", "/some-library/release/202103/someLibrary.zip"),
                            new JProperty("Master", "/some-library/master/someLibrary.zip")
                        )
                    )
                )
            )
        )
    )
);
```

will result in:

``` json
{
  "Libraries": {
    "SomeLibrary": {
      "icon": "books.svg",
      "links": {
        "Release": "/some-library/release/202103/someLibrary.zip",
        "Master": "/some-library/master/someLibrary.zip"
      }
    }
  }
}
```
