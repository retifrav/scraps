На самом деле, оно всё так же модальное, но запускается в отдельном потоке, потому не мешает выполнению дальнейших действий:

new Thread(new ThreadStart(delegate
{
    MessageBox.Show
    (
      "Не пингуется.",
      "А вот хрен",
      MessageBoxButtons.OK,
      MessageBoxIcon.Error
    );
})).Start();