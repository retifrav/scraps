DECLARE @userID int
-- сам курсор
DECLARE @CURSOR CURSOR

-- временная таблица
DECLARE @t table (login nvarchar(10), fio nvarchar(250))  

-- заполнение курсора
SET @CURSOR  = CURSOR SCROLL
FOR
SELECT un.IdUser
FROM empl AS e
     JOIN users AS un
          ON e.id = un.idempl
WHERE workplace IN
(
    SELECT id
    FROM voc
    WHERE someid IN (11000, 12000, 13000, 14000)
)
-- теперь проход по нему
OPEN @CURSOR
    FETCH NEXT FROM @CURSOR INTO @userID
    WHILE @@FETCH_STATUS = 0
    BEGIN
        insert @t -- заполняем временную таблицу
        exec getDudes @userID -- результатами выполнения процедуры
        -- следующая запись курсора
        FETCH NEXT FROM @CURSOR INTO @userID
    END
CLOSE @CURSOR

-- а это просто смотрим результаты, что он насобирал
SELECT DISTINCT(login), fio
FROM @t
ORDER BY fio