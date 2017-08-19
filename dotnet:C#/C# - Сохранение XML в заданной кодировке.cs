// строковое представление XML-документа, теги и все дела. И без первой строки (преамбулы)
XDocument doc = new XDocument(someXMLstringWithoutPreambula);
// например, хотим сохранить в кодировке Windows-1251
using (TextWriter sw = new StreamWriter(
    Path.Combine(@"C:\temp\", "somefile.xml"),
        false,
        Encoding.GetEncoding(1251)
        ))
{
	// преамбулу он сам напишет. Например: <?xml version="1.0" encoding="windows-1251"?>
    doc.Save(sw);
}