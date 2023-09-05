## npm

<!-- MarkdownTOC -->

- [Custom registry](#custom-registry)
- [List available package versions](#list-available-package-versions)

<!-- /MarkdownTOC -->

### Custom registry

For example, you have an npm feed in your JFrog Artifactory instance. To authenticate your npm client with it, add the following to your `~/.npmrc`:

```
@some:registry=https://artifactory.YOUR.HOST/artifactory/api/npm/SOME-FEED/
//artifactory.YOUR.HOST/artifactory/api/npm/SOME-FEED/:_password=YOUR-API-KEY-ENCODED-IN-BASE64
//artifactory.YOUR.HOST/artifactory/api/npm/SOME-FEED/:username=YOUR-USERNAME
//artifactory.YOUR.HOST/artifactory/api/npm/SOME-FEED/:email=NPM-REQUIRES-EMAIL-TO-BE-SET-BUT-DOESNT-USE-THAT-VALUE
//artifactory.YOUR.HOST/artifactory/api/npm/SOME-FEED/:always-auth=true
```

Yes, `email` is required to be set, but isn't used for anything, as it seems. Or, perhaps, that depends on the registry. For instance, Azure DevOps Artifacts doesn't use any values aside from `_password`.

To [encode](https://github.com/retifrav/scraps/blob/master/_linux/index.md#encode) your API Key to Base64:

``` sh
$ echo -n "YOUR-API-KEY" | base64
```

Finaly, `@some` is your [scope](https://docs.npmjs.com/about-scopes), so you wouldn't get packages with the same name from the default npm registry.

Now, to install a package from your registry:

``` sh
$ npm install @some/super-useful-package

added 1 package in 2s
```

### List available package versions

To list all available versions of a package:

``` sh
$ npm view @some/super-useful-package versions
[ '0.9.1', '0.9.2', '0.9.3' ]
```

...and then to install a specific version:

``` sh
$ npm install @some/super-useful-package@0.9.2

changed 1 package, and audited 2 packages in 2s
```
