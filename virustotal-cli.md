## VirusTotal CLI

<!-- MarkdownTOC -->

- [Get the last analysis of the file](#get-the-last-analysis-of-the-file)
- [Scan the file](#scan-the-file)

<!-- /MarkdownTOC -->

### Get the last analysis of the file

``` sh
$ shasum ~/Applications/PCalc.app/Contents/MacOS/PCalc | head -c 40 | xargs -I {} vt file {} --include=date --include=last_analysis_date --include=meaningful_name --include=times_submitted --include=last_analysis_stats

- last_analysis_date: 1648409985  # 2022-03-27 21:39:45 +0200 CEST
  last_analysis_stats:
    confirmed-timeout: 0
    failure: 0
    harmless: 0
    malicious: 0
    suspicious: 0
    timeout: 1
    type-unsupported: 14
    undetected: 58
  meaningful_name: "PCalc"
  times_submitted: 2
```

### Scan the file

Upload the file for scanning and get the task ID:

``` sh
$ vt scan file ~/Applications/PCalc.app/Contents/MacOS/PCalc

/Users/YOUR-NAME/Applications/PCalc.app/Contents/MacOS/PCalc MDQ0MmVjNTY1OTMzZDdmZjVjOGZiYjVkYzk0NTNkNDg6MTY0ODQwOTk4NQ==
```

Then get the task results:

``` sh
$ vt analysis --include=date --include=stats --include=status MDQ0MmVjNTY1OTMzZDdmZjVjOGZiYjVkYzk0NTNkNDg6MTY0ODQwOTk4NQ==

- date: 1648457549  # 2022-03-28 10:52:29 +0200 CEST
  stats:
    confirmed-timeout: 0
    failure: 0
    harmless: 0
    malicious: 0
    suspicious: 0
    timeout: 1
    type-unsupported: 14
    undetected: 58
  status: "completed"
```
