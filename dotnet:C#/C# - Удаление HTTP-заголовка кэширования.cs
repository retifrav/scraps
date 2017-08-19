// При загрузке файла в HTTP-заголовок может писаться запрет кэширования, из-за чего в IE файл может не загружаться. Чтобы так не было, надо добавить следующий заголовок

context.Response.AddHeader("cache-control", "private");

// например

byte[] buff = file.datastream.ToArray();
string name = file.name;
string ext = name.Split('.').Last();
context.Response.Buffer = true;
context.Response.Clear();
context.Response.ClearContent();
context.Response.ClearHeaders();
context.Response.ContentType = Stuff.getMIME(ext);
context.Response.AddHeader("cache-control", "private");
context.Response.AddHeader(
    "Content-Disposition",
    @"attachment;filename=" + System.Web.HttpUtility.UrlPathEncode(name)
    );
context.Response.BinaryWrite(buff);