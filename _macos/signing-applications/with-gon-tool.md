# Using gon tool

<!-- MarkdownTOC -->

- [Signing](#signing)
- [Notarization](#notarization)

<!-- /MarkdownTOC -->

All the same steps from the [main article](./index.md#signing) can be executed [using gon tool](https://github.com/mitchellh/gon/blob/master/README.md), and that will also include automatic querying Apple server for notarization status until it's ready. And while it is convenient, I'd still recommended to go with the manual steps described in the main article.

If you decided to use `gon` tool, it can be installed with Homebrew:

``` sh
$ brew tap mitchellh/gon
$ brew install mitchellh/gon/gon
```

## Signing

Create `conf-sign.json` config:

``` json
{
    "source" : ["/path/to/some.app"],
    "bundle_id" : "com.your-company.some",
    "apple_id": {
        "username": "buildbot@your-company.com",
        "password": "BUILDBOT-PASSWORD-HERE"
    },
    "sign" :{
        "application_identity" : "G5PQECV1C0O922L3CFE4K2E3P30E2BF1233E0DDF"
    },
    "zip" :{
        "output_path" : "some.zip"
    }
}
```

The `application_identity` value is the `Developer ID Application` certificate ID.

Now run:

``` sh
$ gon -log-level=info ./conf-sign.json
```

## Notarization

Apparently, this is already done in signing, so separate notarization might not be needed. If it is needed, then create `conf-notarize.json` config:

``` json
{
  "notarize": [{
    "path": "/path/to/some.zip",
    "bundle_id": "com.your-company.some",
    "staple": true
  }],
  "apple_id": {
     "username": "buildbot@your-company.com",
     "password": "BUILDBOT-PASSWORD-HERE"
  }
}
```

And run:

``` sh
$ gon -log-level=info ./conf-notarize.json
```
