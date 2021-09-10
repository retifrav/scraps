## GitHub GraphQL

<!-- MarkdownTOC -->

- [List all user repositories sorted by creation date](#list-all-user-repositories-sorted-by-creation-date)

<!-- /MarkdownTOC -->

## List all user repositories sorted by creation date

``` graphql
{
  repositoryOwner(login: "retifrav") {
    repositories(orderBy: {field: CREATED_AT, direction: DESC}, first: 50) {
      edges {
        node {
          name
          createdAt
        }
      }
    }
  }
}
```
