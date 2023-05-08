# IRC

<!-- MarkdownTOC -->

- [Nicknames](#nicknames)
    - [Nickname registration](#nickname-registration)
- [Channels](#channels)
    - [Channel registration](#channel-registration)
    - [Greeting message and topic](#greeting-message-and-topic)

<!-- /MarkdownTOC -->

## Nicknames

```
/msg NICKSERV HELP
/msg NICKSERV INFO
/msg NICKSERV STATUS
```

### Nickname registration

```
/msg NICKSERV REGISTER YOUR-PASSWORD YOUR-EMAIL
/msg NICKSERV SET ENFORCE
```

## Channels

```
/msg CHANSERV HELP
/msg CHANSERV INFO #channel
/msg CHANSERV STATUS #channel
```

### Channel registration

```
/msg CHANSERV REGISTER #channel
/msg CHANSERV SET #channel EMAIL your@email
```

If there is a chance that channel might be empty (*for example, if you disconnected, being the only member on the channel*), then enable guarding the channel to preserve configured [modes](https://libera.chat/guides/channelmodes):

```
/msg CHANSERV SET #channel GUARD ON
/msg CHANSERV SET #channel KEEPTOPIC ON
```

### Greeting message and topic

```
/msg CHANSERV SET #channel ENTRYMSG YOUR-GREETING-MESSAGE-WITH-SPACES-OR-NOT
/msg CHANSERV TOPIC #channel YOUR-TOPIC
/msg CHANSERV SET #channel TOPICLOCK ON
```
