## Database CRUD scaffolding

Generate models and database context:

```
$ dotnet ef dbcontext scaffold "Server=localhost;Port=5432;Database=DATABASE;User Id=USER;Password=PASSWORD;" Npgsql.EntityFrameworkCore.PostgreSQL --context-dir Data -o Models -t hardware -t kinds -t specifications -t owners -d -f
```

Copy `DbSet`'s and `OnModelCreating()` code from default context class to yours and delete this one. This line is likely to be changed too:

```
base.OnModelCreating(modelBuilder);
```

Generate controllers and views:

```
$ dotnet aspnet-codegenerator controller -name HardwaresController -m Hardware -dc ApplicationDbContext --relativeFolderPath Controllers --useDefaultLayout
$ dotnet aspnet-codegenerator controller -name OwnersController -m Owner -dc ApplicationDbContext --relativeFolderPath Controllers --useDefaultLayout
$ dotnet aspnet-codegenerator controller -name KindsController -m Kind -dc ApplicationDbContext --relativeFolderPath Controllers --useDefaultLayout
$ dotnet aspnet-codegenerator controller -name SpecificationsController -m Specification -dc ApplicationDbContext --relativeFolderPath Controllers --useDefaultLayout
```