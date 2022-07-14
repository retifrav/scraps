## Artifactory Query Language

<!-- MarkdownTOC -->

- [Executing queries](#executing-queries)
    - [With cURL](#with-curl)
    - [With JFrog CLI](#with-jfrog-cli)
- [Deleting old packages](#deleting-old-packages)

<!-- /MarkdownTOC -->

The Artifactory REST API allows to execute queries written in [Artifactory Query Language](https://www.jfrog.com/confluence/display/JFROG/Artifactory+Query+Language) (AQL).

### Executing queries

#### With cURL

Search queries can be executed with `cURL`, either inline (*without `Content-Type:text/plain` you'll get `415 Unsupported Media Type`*):

``` sh
$ curl -X POST -u USERNAME:API-TOKEN -H "Content-Type:text/plain" https://artifactory.your.company/artifactory/api/search/aql --data 'items.find({"repo":"some-npm","modified":{"$before":"6mo"}}).include("name","created").sort({"$desc":["created"]})'
```

or you can execute query from a file, for example `q.query`:

``` json
items.find(
    {
        "repo": "some-npm",
        "modified": { "$before": "6mo" }
    }
)
.include("name", "created")
.sort({"$desc" : ["created"]})
```

and then:

``` sh
$ curl -X POST -u USERNAME:API-TOKEN https://artifactory.your.company/artifactory/api/search/aql -T ./q.query
```

#### With JFrog CLI

You can also execute queries with [JFrog CLI](https://jfrog.com/getcli/) tool. Some operations can be performed only with the tool, such as [deleting packages](#deleting-old-packages) (*although it's likely possible to do with `cURL` too, but sadly there doesn't seem to be an easy way*).

First you'd need to establish connection to the Artifactory:

``` sh
$ jf config add YourCompany --interactive=false --artifactory-url="https://artifactory.your.company/artifactory" --user="USERNAME" --password="API-TOKEN"
```

That way it will verify the connection and save its settings to a local file in `~/.jfrog`, so you won't need to provide those parameters for every query.

### Deleting old packages

In order not to run out of disk space, you'd need to automatically delete old (*let's say, those published half a year ago*) packages/artifacts. Surprisingly enough, JFrog Artifactory doesn't have an out-of-the-box functionality for automatic artifacts retention/removal, so you'll need to crutch around that yourself.

You might have master/development packages versioned as `0.0.*` and stable/release packages versioned as `1.*.*`. While master packages should be safe to delete, the stable ones you should only delete if they have 0 downloads, so they were never used by any of your users.

But then there seems to be no (*simple*) way to query packages by versions (*at least without setting-up build repository*), so the version would need to be extracted from the package path/name. And since every package type has a different layout, the query becomes rather complicated:

``` json
items.find(
{
    "$or":
    [
        {
            "$and": [{
                "repo": "some-conan",
                "$or":[
                    {"path": {"$match":"YourCompany/some/0.0.*/*"}},
                    {"path": {"$match":"YourCompany/another/0.0.*/*"}},
                    {"path": {"$match":"YourCompany/yetAnother/0.0.*/*"}}
                ],
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-conan",
                "$or":[
                    {"path": {"$match":"YourCompany/some/1.*.*/*"}},
                    {"path": {"$match":"YourCompany/another/1.*.*/*"}},
                    {"path": {"$match":"YourCompany/yetAnother/1.*.*/*"}}
                ],
                "stat.downloads": {"$eq": null},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-npm",
                "name": {"$match":"some-*-0.0.*.*"},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-npm",
                "name": {"$match":"some-*-1.*.*.*"},
                "stat.downloads": {"$eq": null},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-nuget",
                "name": {"$match":"*.*.0.0.*.nupkg"},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-nuget",
                "name": {"$match":"*.*.1.*.*.nupkg"},
                "stat.downloads": {"$eq": null},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-maven",
                "path": {"$match":"net/YourCompany/*/0.0.*"},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-maven",
                "path": {"$match":"net/YourCompany/*/1.*.*"},
                "stat.downloads": {"$eq": null},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-deb",
                "name": {"$match":"*_0.0.*_*.deb"},
                "modified": { "$before": "6mo" }
            }]
        },
        {
            "$and": [{
                "repo": "some-deb",
                "name": {"$match":"*_1.*.*_*.deb"},
                "stat.downloads": {"$eq": null},
                "modified": { "$before": "6mo" }
            }]
        }
    ]
})
.include("name", "type", "path", "created", "stat.downloads")
.sort({"$asc" : ["created"]})
```

Note that to get packages with 0 downloads, you need to compare `stat.downloads` with `null` and not `0`.

Now when we have the search query, packages can be deleted with [JFrog CLI tool](#with-jfrog-cli):

``` sh
$ jf rt del --spec ./find-old-packages.spec --quiet --dry-run
```

Note the `--dry-run` option here.

The specification file (`find-old-packages.spec`) is the same query as above, just wrapped a bit for JFrog CLI tool:

``` json
{
  "files": [
    {
      "aql": {
        "items.find":
        {
            "$or":
            [
                {
                    "$and": [{
                        "repo": "some-conan",
                        "$or":[
                            {"path": {"$match":"YourCompany/some/0.0.*/*"}},
                            {"path": {"$match":"YourCompany/another/0.0.*/*"}},
                            {"path": {"$match":"YourCompany/yetAnother/0.0.*/*"}}
                        ],
                        "modified": { "$before": "6mo" }
                    }]
                },

                {
                    ...
                },

                {
                    "$and": [{
                        "repo": "some-deb",
                        "name": {"$match":"*_1.*.*_*.deb"},
                        "stat.downloads": {"$eq": null},
                        "modified": { "$before": "6mo" }
                    }]
                }
            ]
        }
      }
    }
  ]
}
```
