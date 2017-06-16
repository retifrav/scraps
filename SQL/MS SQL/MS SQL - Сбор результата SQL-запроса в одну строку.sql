-- топорно
DECLARE @result nvarchar(max)
SET @result = '' -- обязательно присвоить пустую строку сначала
    SELECT @result = @result + CAST(IdUser AS nvarchar(20)) + ','
    FROM Users.Levels
    WHERE IdUser = 324
SELECT substring(@result, 0, len(@result) - 1) -- удаление хвостовой запятой

-- и ещё вариант
SELECT CAST(IdUser AS nvarchar(20)) + ', '
FROM Users.Levels
WHERE IdUser = 324
FOR XML PATH('') -- вот где собака порылась

-- и ещё он же, но не в XML
SELECT CAST(
(SELECT CAST(IdUser AS nvarchar(20)) + ', '
FROM Users.Levels
WHERE IdUser = 324
FOR XML PATH(''))
AS VARCHAR(MAX))