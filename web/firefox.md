# Firefox

- [Make a screenshot](#make-a-screenshot)
- [about:config](#aboutconfig)
  + [Trust certificates](#trust-certificates)

## Make a screenshot

```
:screenshot --selector "#NODE-ID" --dpr 1
```

## about:config

### Trust certificates

If you have some certificate in the system that you have added yourself, for instance in order to debug your web-requests via proxy, Firefox won't trust it, so you need to set this value:

```
security.enterprise_roots.enabled | true
```
