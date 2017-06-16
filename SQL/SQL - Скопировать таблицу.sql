Сначала создать клон таблицы по её схеме (те же поля, те же типы).

Потом запрос:

INSERT INTO table_new
SELECT * FROM table_original; 