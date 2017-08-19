// чтобы это запилилось, надо в Добавить ссылку найти расширения от Офиса
using Excel = Microsoft.Office.Interop.Excel;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    private void btn_openXLS_Click(object sender, RoutedEventArgs e)
    {
        Excel.Application excel;
        Excel.Workbook book;
        Excel.Worksheet sheet;

        OpenFileDialog openDialog = new OpenFileDialog();
        openDialog.Filter = "Файл Excel|*.XLSX;*.XLS";
        openDialog.ShowDialog();
        try
        {
            // приложение самого Excel
            excel = new Excel.Application();
            // книга
            book = excel.Workbooks.Open(openDialog.FileName);
            // таблица
            sheet = excel.ActiveSheet as Microsoft.Office.Interop.Excel.Worksheet;
            // ячейки
            Microsoft.Office.Interop.Excel.Range range = sheet.UsedRange;

            List<FSRARrecord> cells = new List<FSRARrecord>();

            // ну тут просто типа данные начинаются с 4 строки и будем читать всего 15 строк
            for (int i = 4; i < 15; i++ )
            {
                try
                {
                    cells.Add(
                        new FSRARrecord() {
                            orgName_val = (range.Cells[i, 1] as Excel.Range).Value.ToString().Trim(),
                            INN_val = (range.Cells[i, 2] as Excel.Range).Value.ToString().Trim(),
                            addressJuridical_val = (range.Cells[i, 4] as Excel.Range).Value.ToString().Trim(),
                            addressFact_val = (range.Cells[i, 6] as Excel.Range).Value.ToString().Trim(),
                            subject_val = (range.Cells[i, 9] as Excel.Range).Value.ToString().Trim(),
                            comments_val = "ФСРАР. Список организаций, имеющих лицензии "
                            + "на розничную торговлю алкогольной продукции, в отношении "
                            + "которых приняты решения о приостановлении действия лицензий "
                            + "по состоянию на " + DateTime.Today.ToShortDateString()
                            + "; КПП орг-ии: " + (range.Cells[i, 7] as Excel.Range).Value.ToString().Trim()
                            + "; e-mail организации: " + (range.Cells[i, 5] as Excel.Range).Value.ToString().Trim()
                            + "; вид лиц-мой деят-ности: " + (range.Cells[i, 10] as Excel.Range).Value.ToString().Trim()
                            + "; Дата выдачи лиц-зии: " + (range.Cells[i, 12] as Excel.Range).Value.ToString().Trim()
                            + "; Номер лиц-зии: " + (range.Cells[i, 14] as Excel.Range).Value.ToString().Trim()
                            + "; Основание изменения свед-ний о лиц-зии: " + (range.Cells[i, 17] as Excel.Range).Value.ToString().Trim()
                            + "; История изменения свед-ний: " + (range.Cells[i, 18] as Excel.Range).Value.ToString().Trim(),
                            dateModification_val = DateTime.Parse((range.Cells[i, 16] as Excel.Range).Value.ToString().Trim()),
                            dateActualization_val = DateTime.Now,
                            operator_val = "ФСРАР",
                            sourceStatus_val = "официальная"
                        });
                }
                catch { }
            }
            dataGrid_xlsContents.ItemsSource = cells; // где-то на форме есть DataGrid
        }
        catch (Exception ex) { MessageBox.Show("Что-то случилось: " + ex.Message, "Ошибка"); }
    }
}

// а дело в том, что DataGrid заполняется из источника, просто так насовать в него строки у меня не получилось, потому вот надо создать класс и присвоить источником список его объектов
public class FSRARrecord
{
    public string orgName_val               { get; set; }
    public string INN_val                   { get; set; }
    public string addressJuridical_val      { get; set; }
    public string addressFact_val           { get; set; }
    public string subject_val               { get; set; }
    public string comments_val              { get; set; }
    public DateTime dateModification_val    { get; set; }
    public DateTime dateActualization_val   { get; set; }
    public string operator_val              { get; set; }
    public string sourceStatus_val          { get; set; }
}