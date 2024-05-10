## NuGet

<!-- MarkdownTOC -->

- [Update Azure DevOps Artifacts source with a new access token](#update-azure-devops-artifacts-source-with-a-new-access-token)

<!-- /MarkdownTOC -->

### Update Azure DevOps Artifacts source with a new access token

``` sh
$ nuget sources update -Name "YOUR-SOURCE-NAME" -Username "DOESNTMATTER" -Password "NEW-TOKEN-HERE"
```

if that doesn't help, complaining something like this when you try to do something with the source:

``` sh
Unable to load the service index for source https://pkgs.dev.azure.com/YOUR-ORGANIZATION/SOME-ID/_packaging/YOUR-PROJECT/nuget/v3/index.json
Key not valid for use in specified state.
```

then just delete and re-add that source:

``` sh
$ nuget sources remove -Name "YOUR-SOURCE-NAME"
$ nuget sources add -Name "YOUR-SOURCE-NAME" -Source "https://pkgs.dev.azure.com/YOUR-ORGANIZATION/SOME-ID/_packaging/YOUR-PROJECT/nuget/v3/index.json" -Username "DOESNTMATTER" -Password "NEW-TOKEN-HERE"
```
