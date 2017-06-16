-- запрос -----------------------------------------------------------------------------------------

DECLARE
    @name nvarchar(250),
    @IdGroup int,
    @isAllowed bit

PRINT '-------- Vendor Products Report --------';

DECLARE prods_cursor CURSOR FOR 
    SELECT
        lsp.[name],
        chng.IdGroup,
        lsp.[isAllowed]
    FROM Products AS lsp
         JOIN Groups AS chng
           ON chng.owner = lsp.owner
    WHERE lsp.price = 2749

OPEN prods_cursor

FETCH NEXT FROM prods_cursor 
INTO
    @name,
    @IdGroup,
    @isAllowed

WHILE @@FETCH_STATUS = 0
BEGIN
    PRINT ' '
    SELECT @message = '----- Products From Vendor: ' + @name

    PRINT @message

    FETCH NEXT FROM prods_cursor 
    INTO
        @name,
        @IdGroup,
        @isAllowed
END 
CLOSE prods_cursor;
DEALLOCATE prods_cursor;

-- результат --------------------------------------------------------------------------------------
 
----- Products From Vendor: prodval00002
 
----- Products From Vendor: prodval00003
 
----- Products From Vendor: prodval00002
 
----- Products From Vendor: prodval00008
 
----- Products From Vendor: prodval00014
 
----- Products From Vendor: prodval00020
 
----- Products From Vendor: prodval00026
 
----- Products From Vendor: prodval00032
 
----- Products From Vendor: prodval00038
 
----- Products From Vendor: prodval00044
 
----- Products From Vendor: prodval00050
 
----- Products From Vendor: prodval00056
 
----- Products From Vendor: prodval00063