Проблема заключается в следующем. Иногда наступают такие ситуации, когда откуда-нибудь приходит огромаднейший CSV-файл, в котором несколько миллионов строк, и надо его содержимое внести в некоторую таблицу нашей БД.

Такой файл даже ничем не открыть (кроме Excel, который, впрочем, сможет показать несколько тысяч записей из него).

Но и ладно, нам открывать его не надо, а чтением записей и их вставкой в БД займётся программа, которую мы напишем.

Платформа такая:

|------------------:|:----------------------|
| ОС                | Windows 8.1 SP1 x64   |
| язык              | C#                    |
| библиотеки        | .NET Framework 4      |
| среда разработки  | Visual Studio 2012    |

Сразу суть: есть специальный класс по массовой вставке записей из CSV в таблицу БД - SqlBulkCopy. Но сложность в том, что если прочитать сразу всё содержимое CSV-файла (который может достигать размера в несколько гигабайт) и попытаться это вставить в БД, то очень легко оперативки не хватит и всё записнет. Выходом будет использовать некоторый буфер:

1. В буфер считывается некоторая порция CSV-файла;
2. Прочитанное разбирается и вставляется в БД;
3. Буфер очищается и по новой.

Допустим, наша таблица назначения состоит из трёх полей (колонок):

1. Первичный ключ, он нас не интересует, генерируется автоинкрементом;
2. Поле first;
3. Поле second.

Файл CSV имеет такой вид:

whore,howmuch
Jolly,300
Fox,900
Lawrence,400
Johansson,1500

Первая строка - "шапка таблицы". Как видим, названия колонок отличаются от наших, потому автобиндинг использовать не будем - далее в коде он закомментирован, а имена колонок прописываются руками.

Вот моё решение.

/// <summary>
/// Чтение CSV-файла и сохранение записей в таблицу БД
/// </summary>
/// <returns>
/// 0 - всё прошло без ошибок
/// 1 - не удалось сохранить записи в БД
/// 2 - ещё что-то не удалось
/// </returns>
int readCSVandSave2DB()
{
    // задаём размер буфера, пусть будет 9000 (строк CSV-файла)
    int csvBufferSize = 9000;
    // указываем путь до файла
    string path2file = @"C:\temp\hugefile.csv";
    // сюда будет читаться CSV-файл
    DataTable csvData = new DataTable();
    try // пробуем читать
    {
        // для TextFieldParser надо подключить библиотеку Microsoft.VisualBasic
        // он вовсе не обязателен, CSV в DataTable можно разбирать как угодно
        using (TextFieldParser csvReader = new TextFieldParser(path2file))
        {
            csvReader.SetDelimiters(new string[] { "," });
            csvReader.HasFieldsEnclosedInQuotes = true;
            string[] colFields = csvReader.ReadFields();
            //foreach (string column in colFields) // автобиндинг
            //{
                // названия колонок в таблице
                DataColumn datecolumnSer = new DataColumn("first");
                datecolumnSer.AllowDBNull = true;
                csvData.Columns.Add(datecolumnSer);
                DataColumn datecolumnNum = new DataColumn("second");
                datecolumnNum.AllowDBNull = true;
                csvData.Columns.Add(datecolumnNum);
            //}
            int buffer_wannabe = 0;
            while (!csvReader.EndOfData)
            {
                string[] fieldData = csvReader.ReadFields();
                csvData.Rows.Add(fieldData);

                buffer_wannabe++;
                // начитали 9000 строк, заносим их в базу и сбрасываем буфер на 0
                if (buffer_wannabe == csvBufferSize)
                {
                    buffer_wannabe = 0;

                    // функция вставки записей описана ниже
                    if (!insertCurrentBunchOfRecs(csvData)) return 1;

                    csvData.Rows.Clear();
                }
            }
            // в последнем чтении в буфере меньше 9000 записей, их тоже надо занести
            if (buffer_wannabe != 0)
            {
                if (!insertCurrentBunchOfRecs(csvData)) return 1;
                csvData.Rows.Clear();
            }
        }
    }
    catch (Exception ex)
    {
        return 2;
    }
    
    // всё огонь
    return 0;
}

/// <summary>
/// Сохранение новых записей во временную таблицу в БД
/// </summary>
/// <param name="csvData">строки из CSV</param>
/// <returns></returns>
static private bool insertCurrentBunchOfRecs(DataTable csvData)
{
    try
    {
        using (SqlConnection dbConnection
            = new SqlConnection("ПОДКЛЮЧЕНИЕКВАШЕЙБД"))
        {
            dbConnection.Open();
            // вся соль вот в этом классе - SqlBulkCopy - он делает всю магию
            using (SqlBulkCopy s = new SqlBulkCopy(dbConnection))
            {
                // если таблица в какой-то схеме, то указать это, например Meschem.Minetable
                s.DestinationTableName = "ИМЯТАБЛИЦЫБД";
                foreach (var column in csvData.Columns)
                {
                    s.ColumnMappings.Add(column.ToString(), column.ToString());
                }
                s.WriteToServer(csvData);
            }
        }
    }
    catch (Exception ex) { return false; }
    
    return true;
}