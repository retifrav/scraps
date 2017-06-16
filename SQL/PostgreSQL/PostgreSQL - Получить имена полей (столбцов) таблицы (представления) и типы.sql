-- эти и ещё многие другие полезные вещи можно почерпнуть из документации:
-- http://www.postgresql.org/docs/8.1/static/infoschema-columns.html

SELECT column_name
FROM information_schema.columns
WHERE information_schema.columns.table_schema = 'your_schema' AND information_schema.columns.table_name = 'your_table_name';

-- или без префиксов, потому что и так понятно, что оно берётся из information_schema.columns

SELECT column_name
FROM information_schema.columns
WHERE table_schema = 'your_schema' AND table_name = 'your_table_name';

-- А вот ещё можно получить и типы данных тоже:

SELECT ordinal_position, column_name, data_type
FROM information_schema.columns
WHERE table_name = 'your_table_name'
ORDER BY 1;