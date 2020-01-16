# App Center CLI tool

- [Distribution](#distribution)
  - [Distribute release](#distribute-release)
  - [Get the application latest release ID](#get-the-application-latest-release-id)
  - [Add more distribution destinations](#add-more-distribution-destinations)

## Distribution

### Distribute release

```
appcenter distribute release -f /path/to/your-application.ipa -g some-group -a organization/application -r "Some new release" --token APP-CENTER-API-TOKEN
```

here:

- `-g some-group` - distribution group name
- `-a organization/application` - you can get those from the application URL in the App Center
- `-r "Some new release"` - release message
- `--token APP-CENTER-API-TOKEN` - App Center API token from the environment variable

### Get the application latest release ID

```
appcenter distribute releases list -a organization/application --token APP-CENTER-API-TOKEN | grep ID | head -1 | tr -s ' ' | cut -f2 -d ' '
```

or with Python:

```
appcenter distribute releases list -a organization/application --token APP-CENTER-API-TOKEN --output json | python -c 'import json,sys;obj=json.load(sys.stdin);print obj[0]["id"]'
```

### Add more distribution destinations

```
appcenter distribute releases add-destination -d Collaborators -t group -a organization/application --token APP-CENTER-API-TOKEN -r RELEASE-ID -s
```

here:

- `-d Collaborators` - name of the group or whatever
- `-t group` - type of whatever
- `-r RELEASE-ID` - application release ID to distribute, usually you'd want to use the [latest](#get-the-application-latest-release-id)
- `-s` - hopefully, that is for not notifying subscribers
