-- это просто переменные, в которые можно делать выборку
DECLARE @id int,
        @isActive tinyint

-- вот объявили курсор
DECLARE ccfc_cursor CURSOR LOCAL FOR
-- это запрос для него
SELECT *
FROM ololo
-- открыли курсор
OPEN ccfc_cursor
-- выбираем первую запись
FETCH NEXT FROM ccfc_cursor
-- в наши переменные
INTO @id, @isActive
-- и поехали выбирать все остальные
WHILE @@FETCH_STATUS = 0
BEGIN
    -- на каждой записи будем выводить текст
    PRINT 'ololo'
    -- и переходить на следующую запись, иначе будет бесконечный цикл по одной
    FETCH NEXT FROM ccfc_cursor
END 
-- когда всё поделали, закрываем курсор
CLOSE ccfc_cursor
DEALLOCATE ccfc_cursor

-- тут можно вывести последные выбранные значения
SELECT @id, @isActive;